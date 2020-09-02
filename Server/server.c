#include "utils.h"
#include "put.h"

const int GET_LINE_COUNT = 40;

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
			int numArguments = 0;
			char** arguments = GetArguments(&numArguments);

			if (numArguments < 2)
			{
				strcpy(res, "done");
				write(socketDescriptor, res, sizeof(res));
				continue;
			}

			// see if the directory exists
			char directory[MAX] = "./";
			strcat(directory, arguments[0]);

			struct stat st;
			if (stat(directory, &st) == -1)
			{
				strcpy(res, "done");
				write(socketDescriptor, res, sizeof(res));
				continue;
			}

			strcat(directory, "/");
			strcat(directory, arguments[1]);
			int exists = DoesFileExist(directory);

			if (!exists)
			{
				strcpy(res, "done");
				write(socketDescriptor, res, sizeof(res));
			}
			else
			{
				FILE* file = fopen(directory, "r");

				int curLines = 0;
				while (fgets(res, sizeof(res), file))
				{
					write(socketDescriptor, res, sizeof(res));
					curLines++;
					
					if (curLines == GET_LINE_COUNT)
					{
						curLines = 0;

						strcpy(res, "wait");
						write(socketDescriptor, res, sizeof(res));

						// wait for a key to be pressed
						read(socketDescriptor, buff, sizeof(buff));
					}
				}

				printf("Done get\n");

				strcpy(res, "done");
			}
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
		write(socketDescriptor, res, sizeof(res));

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