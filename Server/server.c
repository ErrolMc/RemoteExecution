#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

char** GetArguments(int* arguments)
{
    (*arguments) = 0;

    char* token;
    char** buff = malloc(MAX * sizeof(char*));

    token = strtok(NULL, " ");
    while (token != NULL)
    {
        buff[*arguments] = token;
        (*arguments)++;
        token = strtok(NULL, " ");
    }

    return buff;
}

// Function designed for chat between client and server. 
void ChatToClient(int sockfd) 
{ 
	char res[MAX];
	char buff[MAX]; 
	int n; 

    // infinite loop for chat 
	for (;;) 
    { 
		printf("wat\n");

		bzero(buff, MAX);
		bzero(res, MAX);

		// first read (PAUSES UNTIL THERES DATA)
		read(sockfd, buff, sizeof(buff)); 

		printf("client > %s\n", buff);

		char* token = strtok(buff, " ");

		if (strcmp(token, "put") == 0)
		{
			int numArguments = 0;
			char** arguments = GetArguments(&numArguments);

			strcpy(res, "put processed with");

			char argumentString[3];
			sprintf(argumentString, "%d", numArguments);

			strcat(res, argumentString);
			strcat(res, "arguments\n");

			// free the string array
			//for (int i = 0; i < numArguments; i++)
			//	free(arguments[i]);
			free(arguments);
		}
		else if (strcmp(token, "sys") == 0)
		{
			printf("sys processed\n");
			strcpy(res, "sys processed");
		}
		else if (strcmp(token, "exit") == 0)
		{
			strcpy(res, "exit");
		}

		// give the response
		write(sockfd, res, sizeof(res));

		if (strcmp(res, "exit") == 0)
		{
			printf("Server exit\n");
			break;
		}
	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) 
    { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	
    bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // "host byte order" to "network byte order" long
	servaddr.sin_port = htons(PORT); // "host byte order" to "network byte order" short

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
    { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) 
    { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
    
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
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
	close(sockfd); 
} 
