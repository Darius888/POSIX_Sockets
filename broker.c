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

#define MAX_LINE 256
#define MAX_LINE_TEXTO 1024
#define NUM_THREADS 2
#define OP_SUB 1
#define OP_UNSUB 2
#define OP_PUBLISH 3
#define OP_QUIT 4

void* clientFunction(void *arguments);

pthread_mutex_t mutex;
pthread_cond_t thread_ready;
int notReady = 1;

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

	int sd, sc;
	struct sockaddr_in client_addr, server_addr;
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

	socklen_t size = sizeof(client_addr);

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

void* clientFunction(void *arguments){
	int topic, text;
	ssize_t response;
	ssize_t operation;
	char tema[256] = "";
	char texto[256] = "";
	char op_buff[256];

	pthread_mutex_lock(&mutex);
	int sc = *((int*)arguments);
	notReady = 0;
	pthread_cond_signal(&thread_ready);
	pthread_mutex_unlock(&mutex);
	
	printf("CONNECTED\n");

	while(1){
		operation = read(sc, op_buff, MAX_LINE);
		printf("buffer = %s\n", op_buff);

		if(op_buff[0] == OP_SUB){
			printf("OPERATION CODE RECEIVED : SUBSCRIBE\n");
			printf("OPERATION CODE APPLIED\n");
			char byte[1];
			byte[0] = OP_SUB;
			response = write(sc, byte, MAX_LINE);
			
		} else if(op_buff[0] == OP_PUBLISH){
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

			send(sc, (char *) tema, topic+1, 0);
			send(sc,(char *) &topic, sizeof(int) ,0);
			send(sc, (char *) texto, text+1, 0);
			send(sc,(char *) &text, sizeof(int) ,0);
		} else if(op_buff[0] == OP_QUIT){
			printf("OPERATION CODE RECEIVED : QUIT\n");
			printf("OPERATION CODE APPLIED\n");
			break;
		}
		memset(op_buff, 0, sizeof(op_buff));
	}
	close(sc);
	pthread_exit(0);
}