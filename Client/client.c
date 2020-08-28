#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void ChatWithServer(int socketDescriptor) 
{ 
	char buffCopy[MAX];
	char buff[MAX]; 
	int n; 
	for (;;) 
    { 
		bzero(buff, sizeof(buff)); 

        // get the command to send
        printf(" > ");
        gets(buff);

        if (strcmp(buff, "") == 0)
            continue;

        // send the data
        write(socketDescriptor, buff, sizeof(buff)); 

        // wait for a response
		bzero(buff, sizeof(buff)); 
		read(socketDescriptor, buff, sizeof(buff)); 

        // copy the buffer for processing of the command
        strcpy(buffCopy, buff);

        // process the response
        char* token;
		token = strtok(buffCopy, " ");
        
        if (strcmp(token, "exit") == 0)
        {
            printf("Client exit\n");
            break;
        }

        printf("server > %s\n", buff);
	} 
} 

int main() 
{ 
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
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	//serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
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