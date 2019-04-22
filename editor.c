#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "lines.h"

#define MAX_LINE 256
#define MAX_LINE_TEXTO 1024

void print_usage() {
	printf("Usage: editor -h host -p puerto -t \"tema\" -m \"texto\"\n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char host[256]= "";
	char puerto[256]= "";
	char tema[256]= "";
	char texto[256]= "";
	char op_buff[256];

	struct sockaddr_in server_addr;
	struct hostent *hp;
	int sd;

	int topic, text, operation, response;

	while ((option = getopt(argc, argv,"h:p:t:m:")) != -1) {
		switch (option) {
		        case 'h' : 
				strcpy(host, optarg);
		    		break;
		    	case 'p' : 
				strcpy(puerto, optarg);
		    		break;
		    	case 't' : 
				strcpy(tema, optarg);
		    		break;
		    	case 'm' : 
				strcpy(texto, optarg);
		    		break;
		    	default: 
				print_usage(); 
		    		exit(-1);
		}
	}
	if (strcmp(host,"")==0 || strcmp(puerto,"")==0 ||
		strcmp(tema,"")==0 || strcmp(texto,"")==0){
		print_usage(); 
		exit(-1);
	}

	printf("Host: %s\n", host);
	printf("Puerto: %s\n", puerto);
	printf("Tema: %s\n", tema);
	printf("texto: %s\n", texto);

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bzero((char *)&server_addr, sizeof(server_addr));
	hp = gethostbyname(argv[2]);

	memcpy(&(server_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(atoi(puerto));

	//establish connection
	if(connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr))==-1)
	{
		printf("Error in the connection to the server %s : %s\n", host, puerto);
	} else
	{
		printf("Connection works !\n");
	}

	while(1) {
		operation = readLine(0, op_buff, MAX_LINE);
		send(sd, (char *) op_buff, operation+1, 0);

		recv(sd, &response, sizeof(int), 0);
		printf("RECEIVED RESPONSE %d\n", response ); /*PROBLEM HERE*/
		if(response == 15)
		{
			printf("RESPONSE = 15\n");
			topic = readLine(0, tema, MAX_LINE);
			text = readLine(0, texto, MAX_LINE);

			send(sd, (char *) tema, topic+1, 0);
			send(sd, (char *) texto, text+1, 0);

			printf("Message sent !\n");
		}
	}
	close(sd);

	exit(0);
	//return 0;
}
	
