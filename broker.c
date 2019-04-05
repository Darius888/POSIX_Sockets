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

void print_usage() {
	    printf("Usage: broker -p puerto \n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char puerto[256] = "";
	char tema[256] = "";
	char texto[256] = "";
	char buffer[MAX_LINE];
	char command[MAX_LINE];

	struct sockaddr_in client_addr, server_addr;
	int sd, sc;
	int size,val;
	struct timeval t;
	pthread_attr_t attr;
    pthread_t thid[NUM_THREADS];
    int arrayId[NUM_THREADS];
	int topic, text,operation;
	int tw;
	int op_code, op_response;

	

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



	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	val = 1;

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port 		= htons(atoi(puerto));

	bind(sd, &server_addr, sizeof(server_addr));

	listen(sd, 5);

	size = sizeof(client_addr);

	printf("WAITING FOR CONNECTION\n");

	sc = accept(sd, (struct sockaddr *) &client_addr, &size);

	gettimeofday(&t, NULL);
    srand(t.tv_sec); 

	while(1)
	{		

		// operation = readLine(sc, command, MAX_LINE);

		// printf("OPERATION RECEIVED IS : \n");
		// printf("%s\n", command);


		
		// 	Receive command, and check if it is correct, if yes, send the response to the editor.
		


		// if (strcmp(command,"PUBLISH"))
		// {
			// op_code = 1;
			// send(sd, (char *) op_code, sizeof(int), 0);

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
		

			
		// } else if(strcmp(command, "SUBSCRIBE"))
		// {
		// 	op_code = 2;
		// } else if(strcmp(command, "UNSUBSCRIBE"))
		// {
		// 	op_code = 3;
		// }

		// switch(op_code)
		// {
		// 	case 1: printf("OPERATION IS PUBLISH\n");
			
		// 	case 2: printf("OPERATION IS SUBSCRIBE\n");
			
		// 	case 3: printf("OPERATION IS UNSUBSCRIBE\n");
			 
		// 	default: printf("WRONG COMMAND\n");
		// }
		// if (op_code == 1)
		// {
				
		// }
	
		
	}
		close(sc);
	close(sd);
	exit(0);
	}
	//return 0;
	
	
