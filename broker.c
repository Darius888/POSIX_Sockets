#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "lines.h"
#include "storage.h"

#define MAX_LINE 256
#define MAX_LINE_TEXTO 1024
#define NUM_THREADS 2
#define OP_SUB 1
#define OP_UNSUB 2
#define OP_PUBLISH 3
#define OP_QUIT 4

pthread_mutex_t mutex;
pthread_cond_t thread_ready;
int notReady = 1;
int sd, sc;
struct sockaddr_in client_addr, server_addr;
socklen_t size;
char temaSub[128] = "";
char textoSub[1024] = "";
char portSub[256];

void* clientFunction(void *arguments);
void initializeStorage(char* host);
int putTopicAndText(char* host,char* topic, char* text);
int getTopicAndText(char* host, char* topic, char* text);

void print_usage() {
	    printf("Usage: broker -p puerto \n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char puerto[256] = "";
	
	while ((option = getopt(argc, argv,"p:")) != -1) {
		switch (option) {
		    	case 'p' : 
					strcpy(puerto, optarg);
		    		break;
		    	default: 
					print_usage(); 
		    		exit(-1);
		    }
	}
	if (strcmp(puerto,"")==0){
		print_usage(); 
		exit(-1);
	}

	printf("Puerto: %s\n", puerto);

	int val;

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	val = 1;

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port 		= htons(atoi(puerto));

	bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	listen(sd, 5);

	printf("WAITING FOR CONNECTION\n");

	pthread_t client_thread;
	pthread_attr_t client_attr;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&thread_ready, NULL);
	pthread_attr_init(&client_attr);
	pthread_attr_setdetachstate(&client_attr, PTHREAD_CREATE_DETACHED);

	size = sizeof(client_addr);

	while(1){
		sc = accept(sd, (struct sockaddr *) &client_addr, &size);
		int * arguments = malloc(sizeof(int) * 1);
		arguments[0] = sc;
		pthread_create(&client_thread, &client_attr, (void *)clientFunction, (void *) arguments);
		pthread_mutex_lock(&mutex);
		while(notReady == 1) {
			pthread_cond_wait(&thread_ready, &mutex);
		}
		notReady = 1;
		pthread_mutex_unlock(&mutex);
	}
	close(sd);
	exit(0);
}

int sendToSubscriber(char *tp, char *text) {

	struct sockaddr_in server_addr;
	struct hostent *hp;
	int sd;
	bzero((char *)&server_addr, sizeof(server_addr));
	hp = gethostbyname("localhost");
	memcpy(&(server_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(atoi(portSub));
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr))==-1)
	{
		printf("Error in the connection to the server host : %s\n", portSub);
	} else
	{
		printf("Connection works with listener\n");
	}	
	send(sd, textoSub, sizeof(textoSub), 0);
	close(sd);

	return 0;
}

void* clientFunction(void *arguments){
	int topic, text;
	ssize_t response;
	ssize_t operation;
	char op_buff[256];
	char tema[128] = "";
	char texto[1024] = "";

	pthread_mutex_lock(&mutex);
	sc = *((int*)arguments);
	notReady = 0;
	pthread_cond_signal(&thread_ready);
	pthread_mutex_unlock(&mutex);
	
	printf("CONNECTED\n");

	while(1){
		operation = readLine(sc, op_buff, MAX_LINE);

		if(strcmp(op_buff, "SUBSCRIBE") == 0){
			printf("OPERATION CODE RECEIVED : SUBSCRIBE\n");
			printf("OPERATION CODE APPLIED\n");
			
			operation = readLine(sc, op_buff, MAX_LINE);
			char topicSub[128];
			strcpy(topicSub, op_buff);

			char byte[1];
			byte[0] = OP_SUB;
			send(sc, byte, sizeof(byte), 0);

			operation = readLine(sc, op_buff, MAX_LINE);
			strcpy(portSub, op_buff);

			initializeStorage("localhost");			
		} else if(strcmp(op_buff, "PUBLISH")==0){
			printf("OPERATION CODE RECEIVED : PUBLISH\n");
			printf("OPERATION CODE APPLIED\n");
			char byte[1];
			byte[0] = OP_PUBLISH;
			response = write(sc, byte, MAX_LINE);

			topic = readLine(sc, tema, MAX_LINE);
			text = readLine(sc, texto, MAX_LINE);

			printf("TOPIC RECEIVED\n");
			printf("%s\n", tema );
			printf("TEXT RECEIVED\n");
			printf("%s\n", texto );
			strcpy(temaSub, tema);
			strcpy(textoSub, texto);
			sendToSubscriber(temaSub, textoSub);
			printf("INITIALIZING!!!!\n" );

			initializeStorage("localhost");
			putTopicAndText("localhost",temaSub, textoSub);
		} else if(strcmp(op_buff, "QUIT")==0){
			printf("OPERATION CODE RECEIVED : QUIT\n");
			printf("OPERATION CODE APPLIED\n");
			break;
		} else if(strcmp(op_buff, "UNSUBSCRIBE")==0){
			printf("OPERATION CODE RECEIVED : UNSUSCRIBE\n");
			printf("OPERATION CODE APPLIED\n");
			operation = readLine(sc, op_buff, MAX_LINE);
			char topicUnsub[128];
			strcpy(topicUnsub, op_buff);
		} else{
			printf("Error in code\n");
			break;
		}
		memset(op_buff, 0, sizeof(op_buff));
	}
	close(sc);
	pthread_exit(0);
}

void initializeStorage(char* host)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	int result_1;
	printf("INITIALIZING\n" );

	clnt = clnt_create (host, STORAGE, STORAGEVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	retval_1 = init_1(&result_1, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	
	printf("INITIALIZED : %d\n", result_1);

	clnt_destroy (clnt);

}

int putTopicAndText(char* host, char* topic, char* text)
{

	CLIENT *clnt;

	enum clnt_stat retval_2;
	int result_2;
	char *put_1_topic;
	char *put_1_text;

	clnt = clnt_create (host, STORAGE, STORAGEVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	retval_2 = put_1(topic, text, &result_2, clnt);

	if (retval_2 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}



	printf("%d\n", result_2);

	clnt_destroy (clnt);
	return 1;
}

int getTopicAndText(char* host, char* topic, char* text)
{
	CLIENT *clnt;
	enum clnt_stat retval_3;
	int result_3;
	char *get_1_topic;
	char *get_1_text;


	clnt = clnt_create (host, STORAGE, STORAGEVER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	retval_3 = get_1(topic, text, &result_3, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}

	printf("%s\n", text );

	clnt_destroy (clnt);
}
