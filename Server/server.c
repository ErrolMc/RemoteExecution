#include "utils.h"
#include "put.h"



// Function designed for chat between client and server. 
void ChatToClient(int socketDescriptor) 
{ 
	char buffCopy[MAX];
	char res[MAX];
	char buff[MAX]; 
	int n; 

    // infinite loop for chat 
	for (;;) 
    { 
		bzero(buff, MAX);
		bzero(res, MAX);

		// first read (PAUSES UNTIL THERES DATA)
		read(socketDescriptor, buff, sizeof(buff)); 
		strcpy(buffCopy, buff);

		printf("client > %s\n", buff);

		char* token = strtok(buffCopy, " ");
		
		if (strcmp(token, "put") == 0)
		{
			ProcessPut(socketDescriptor, buff, res);
		}
		else if (strcmp(token, "get") == 0)
		{			
			ProcessGet(socketDescriptor, buff, res);
		}
		else if (strcmp(token, "sys") == 0)
		{
			strcpy(res, "sys processed");
			write(socketDescriptor, res, MAX);
		}
		else if (strcmp(token, "exit") == 0)
		{
			strcpy(res, "exit");
			write(socketDescriptor, res, MAX);

			if (strcmp(res, "exit") == 0)
			{
				printf("Server exit\n");
				break;
			}
		}
		else
		{
			strcpy(res, "unknown command");
			write(socketDescriptor, res, sizeof(res));
		}
	} 
} 

// Driver function 
int main() 
{ 
	const int BACKLOG = 5;

	int socketDescriptor, connfd, len; 
	struct sockaddr_in serverAddress, cli; 

	// socket create and verification 
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
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // "host byte order" to "network byte order" long
	serverAddress.sin_port = htons(PORT); // "host byte order" to "network byte order" short

	// Binding newly created socket to given IP and verification 
	if ((bind(socketDescriptor, (SA*)&serverAddress, sizeof(serverAddress))) != 0) 
    { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(socketDescriptor, BACKLOG)) != 0) 
    { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
    
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(socketDescriptor, (SA*)&cli, &len); 
	if (connfd < 0) 
    {
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	ChatToClient(connfd); 

	// After chatting close the socket 
	close(socketDescriptor); 
} 