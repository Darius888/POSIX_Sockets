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
	printf("Usage: ./publisher -h host -p puerto -t \"tema\" -m \"texto\"\n");
}

int main(int argc, char *argv[]){
	int  option = 0;
	char host[256]= "";
	char puerto[256]= "";
	char tema[128]= "";
	char texto[1024]= "";
	
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

	char op_buff[1024];

	struct sockaddr_in server_addr;
	struct hostent *hp;
	int sd;

	int topic, text;

	bzero((char *)&server_addr, sizeof(server_addr));
	hp = gethostbyname(argv[2]);

	memcpy(&(server_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(atoi(puerto));

	sd = socket(server_addr.sin_family, SOCK_STREAM, IPPROTO_TCP);

	/* connection to the broker */
	if(connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr))==-1)
	{
		printf("Error in the connection to the server %s : %s\n", host, puerto);
	}
	readLine(0, op_buff, MAX_LINE);
	if(strcmp(op_buff, "PUBLISH") == 0){
		send(sd, "PUBLISH\0", sizeof(char *), 0); /* identify the operation */

		topic = readLine(0, tema, MAX_LINE);
		text = readLine(0, texto, MAX_LINE); 

		strcat(tema, "\0");
		strcat(texto, "\0");

		send(sd, (char *) tema, topic+1, 0); /* send the topic name */
		send(sd, (char *) texto, text+1, 0); /* send the text associated with the topic */
	}
	
	/* close the connection */
	close(sd);
	exit(0);
}
