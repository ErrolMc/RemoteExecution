#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define MAX 256 
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

int DoesFileExist(const char * filename)
{
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r"))
	{
        fclose(file);
        return 1;
    }
    return 0;
}

// Function designed for chat between client and server. 
void ChatToClient(int sockfd) 
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
		read(sockfd, buff, sizeof(buff)); 
		strcpy(buffCopy, buff);

		printf("client > %s\n", buff);

		char* token = strtok(buffCopy, " ");
		
		if (strcmp(token, "put") == 0)
		{
			int numArguments = 0;
			char** arguments = GetArguments(&numArguments);

			// check if we are overriding
			int overrite = 0;
			int numFiles = 0;
			for (int i = 0; i < numArguments; i++)
			{
				if (strcmp(arguments[i], "-f") == 0)
					overrite = 1;
				else
					numFiles++;
			}

			// send a ready response
			strcpy(res, "ready");
			write(sockfd, res, sizeof(res));

			bzero(res, sizeof(res));
			bzero(buff, sizeof(buff));

			FILE* curFile;
			for (int i = 0; i < numFiles; i++)
			{
				read(sockfd, res, sizeof(res));
				int fileSize = atoi(res);

				printf("size: %d\n", fileSize);

				int exists = DoesFileExist(arguments[i]);
				int shouldRead = !(exists && !overrite);

				
				strcpy(res, shouldRead ? "read" : "continue");

				write(sockfd, res, sizeof(res));
				if (!shouldRead)
					continue;

				curFile = fopen(arguments[i], "w+");
				
				ssize_t remainingData = fileSize;
				while (remainingData > 0)
				{
					ssize_t n = recv(sockfd, buff, MAX, 0);
					remainingData -= n;
					fprintf(curFile, "%s", buff);
				}

				printf("read file\n");

				fclose(curFile);
			}
			
			free(arguments);
		}
		else if (strcmp(token, "sys") == 0)
		{
			strcpy(res, "sys processed");
		}
		else if (strcmp(token, "exit") == 0)
		{
			strcpy(res, "exit");
		}
		else
		{
			strcpy(res, "unknown command");
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