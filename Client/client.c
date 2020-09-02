#include "utils.h"
#include "put.h"

void ChatWithServer(int socketDescriptor) 
{ 
	char buffCopy[MAX];
	char buff[MAX]; 
	char res[MAX];
	int n; 

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
			ProcessPut(socketDescriptor, &buff, &res);
		}
		else if (strcmp(token, "get") == 0)
		{
			// send the command
			write(socketDescriptor, buff, sizeof(buff));

			while (1)
			{
				bzero(buff, sizeof(buff));
				read(socketDescriptor, buff, sizeof(buff));

				if (strcmp(buff, "wait") == 0)
				{
					printf("Enter any character to continue > ");
					gets();
					strcpy(res, "continue");
					write(socketDescriptor, res, sizeof(res));
				}
				else if (strcmp(buff, "done") == 0)
				{
					break;
				}
				else
				{
					printf("%s", buff);
				}
			}

			printf("\n");
			continue;
		}
		else if (strcmp(token, "run") == 0)
		{

		}
		else
		{
			// send the data
			write(socketDescriptor, buff, sizeof(buff)); 
		}

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