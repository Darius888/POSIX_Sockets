/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "storage.h"

#define MAXCHAR 1024

struct Tuple
{
	char* topic;
	char* text;
};

/* Storage initialization function which returns 1 if everything is as expected */
bool_t
init_1_svc(int *result, struct svc_req *rqstp)
{
	bool_t retval;

	*result = 1;
	
	retval = TRUE;

	return retval;
}


/* Topic and text storing function. */
bool_t
put_1_svc(char *topic, char *text, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	FILE *fp;

	fp=fopen("storage.txt","ab"); // opens file with mode to add more without overwriting

	if(fp == NULL)
   	{
      printf("Error!");   
      exit(1);             
   	}


   	if(fprintf(fp, "%s\n", topic ) < 0 || fprintf(fp, "%s\n", text ) <  0) 
   	{
   		perror("Can't write to a file");
   		*result = 0;
   	} else
   	{
   		*result = 1;
   	}
   
	fclose(fp);

	retval = TRUE;

	return retval;
}

/* Function which retrieves text from the storage by topic */
bool_t
get_1_svc(char *topic, char *text, char **result,  struct svc_req *rqstp)
{
	bool_t retval;
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	char line[512];
	int i=1;	

	text = malloc(500*sizeof(char *));
	
	if((fp = fopen("storage.txt", "r")) == NULL) { // opens file in reading mode
		return(-1);
	}

	while(fgets(temp, 512, fp) != NULL) {
		line_num++;
		 i++;
		if((strstr(temp, topic)) != NULL) {
			printf("A match found on line: %d\n", line_num);
			printf("\n%s\n", temp);
			
			while (fgets(line, 512, fp) != NULL) { /* If the topic is found start reading until the match+1 line */
	       		i++;
	        	if(i == line_num+1)
	       		 {
	           		 
	           		 strcpy(text,line); 
	       		 }
	        
    			}
			find_result++;
			
		}
		
		
	}


	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
	}

	if(fp) {
		fclose(fp);
	}	
	*result = text;
	printf("TEXT RELATED TO TOPIC IS:  %s", *result);

	retval = TRUE;
	return retval;
}

int
storage_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
