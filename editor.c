#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

void print_usage() {
	    printf("Usage: editor -h host -p puerto -t \"tema\" -m \"texto\"\n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char host[256]= "";
	char puerto[256]= "";
	char tema[256]= "";
	char texto[1024]= "";

	struct sockaddr_in client_addr, server_addr;
	struct hostent *hp;
	int sd, sc;
	int size, val;


	
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
	hp = host;

	memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(puerto);

	//establish connection
	connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));


	//send request
	send(sd, (char *) tema, sizeof(char), 0);
	send(sd, (char *) texto, sizeof(char), 0);

	//receive response
	//recv(sd, &res, sizeof(int), 0);

	//printf("Result = %d\n", res);

	close(sd);


	return 0;
}
	
