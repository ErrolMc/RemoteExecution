#include "utils.h"
#include "put.h"
#include "get.h"
#include "run.h"
#include "list.h"
#include "sys.h"

void ChatWithServer(int socketDescriptor) 
{ 
	char buffCopy[MAX];
	char buff[MAX]; 
	char res[MAX];

	for (;;) 
    { 
		bzero(buff, sizeof(buff)); 
		bzero(res, sizeof(res));

        // get the command to send
        printf(" > ");
        gets(buff);

        if (strcmp(buff, "") == 0)
            continue;

		// copy the buffer for processing of the command
        strcpy(buffCopy, buff);	

		char* token = strtok(buffCopy, " ");

		if (strcmp(token, "put") == 0)
		{
			ProcessPut(socketDescriptor, buff, res);
		}
		else if (strcmp(token, "get") == 0)
		{
			ProcessGet(socketDescriptor, buff, res);
		}
		else if (strcmp(token, "run") == 0)
		{
			ProcessRun(socketDescriptor, buff, res);
		}
		else if (strcmp(token, "list") == 0)
		{
			ProcessList(socketDescriptor, buff, res);
		}
		else if (strcmp(token, "sys") == 0)
		{
			ProcessSys(socketDescriptor, buff, res);
		}
		else
		{
			// send the data
			write(socketDescriptor, buff, sizeof(buff)); 
		
			// wait for a response
			bzero(buff, sizeof(buff)); 
			read(socketDescriptor, buff, sizeof(buff)); 

			// copy the buffer for processing of the command
			strcpy(buffCopy, buff);

			// process the response
			token = strtok(buffCopy, " ");
			
			if (strcmp(token, "exit") == 0)
			{
				printf("Client exit\n");
				break;
			}

			printf("server > %s\n", buff);
		}

		// read the time taken
		read(socketDescriptor, buff, MAX);
		printf("%s", buff);
	} 
} 

int main(int argc, char* argv[]) 
{
	if (argc < 2)
	{
		printf("Please enter an ip address\n");
		exit(0);
	}

	int socketDescriptor, connfd; 
	struct sockaddr_in serverAddress, cli; 

	// socket create and varification 
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketDescriptor == -1) 
    { 
		printf("socket creation failed...\n"); 
		exit(0); 
	}
	else
		printf("Socket successfully created..\n"); 
	
	bzero(&serverAddress, sizeof(serverAddress)); 

	// assign IP, PORT 
	serverAddress.sin_family = AF_INET; 
	//serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(socketDescriptor, (SA*)&serverAddress, sizeof(serverAddress)) != 0) 
    { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	ChatWithServer(socketDescriptor); 

	// close the socket 
	close(socketDescriptor); 
} 