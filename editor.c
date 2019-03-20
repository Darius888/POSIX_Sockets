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
	char buffer[MAX_LINE];

	struct sockaddr_in client_addr, server_addr;
	struct hostent *hp;
	int sd, sc;

	int tm, tx, t;


	
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

	while(1)
	{
		tm = readLine(0, buffer, MAX_LINE);
	
		if(tm==-1)
		{
			perror("Fail");
		} else
		{
			send(sd, (char *) buffer, tm+1, 0);
			readLine(sd, buffer, tm+1);
			printf("Message sent !\n");
		}
	}
	close(sd);

	exit(0);
	//return 0;
}
	
