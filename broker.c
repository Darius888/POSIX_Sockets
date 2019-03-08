#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage() {
	    printf("Usage: broker -p puerto \n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char puerto[256] = "";
	char tema[256] = "";
	char texto[256] = "";
	

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

	bzero((char *)&host, sizeof(host));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_addr.s_addr = host;
	server_addr.sin_port 		= htons(puerto);

	bind(sd, &server_addr, sizeof(server_addr));

	listen(sd, 5);

	size = sizeof(client_addr);

	while(1)
	{
		printf("WAITING FOR CONNECTION\n");

		sc = accept(sd, (struct sockaddr *) &client_addr, &size);

		recv(sc, &texto, sizeof(char), 0);
		recv(sc, &tema, sizeof(char), 0);

		close(sc);
	}

	printf("%s\n", texto );
	printf("%s\n", tema );
	close(sd);

	return 0;
}
	
