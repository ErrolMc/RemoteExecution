#include <netdb.h> 
#include <stdlib.h> 
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
			int numArguments = 0;
			char** arguments = GetArguments(&numArguments);

			if (numArguments > 0)
			{
				// send the line indicating the files
				write(socketDescriptor, buff, sizeof(buff)); 

				// wait for a response
				read(socketDescriptor, buff, sizeof(buff)); 

				if (strcmp(buff, "ready") == 0)
				{
					FILE* curFile;
					struct stat fileStat;

					// send the files 1 by 1
					for (int i = 0; i < numArguments; i++)
					{
						// make sure we arent the -f arg
						if (strcmp(arguments[i], "-f") == 0) 
							continue;

						bzero(res, sizeof(res)); 
						bzero(buff, sizeof(buff));
						
						// open the file
						curFile = fopen(arguments[i], "r"); 

						if (curFile == -1)
							continue;

						// get the file stats
						int statres = fstat(fileno(curFile), &fileStat);
						if (statres < 0) 
							continue;
						
						sprintf(res, "%d", fileStat.st_size);

						// write the file size
						write(socketDescriptor, res, sizeof(res));

						// read if we should send the file
						read(socketDescriptor, buff, sizeof(buff));
						
						if (strcmp(buff, "read") != 0)
							continue;
						
						while (fgets(buff, MAX, curFile) != NULL)
						{
							if (send(socketDescriptor, buff, sizeof(buff), 0) == -1)
							{
								printf("Error sending data\n");
								break;
							}
						}
						
						fclose(curFile);
					}
				}
			}
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

void test()
{
	// open the file
	FILE* curFile = fopen("test.txt", "r"); 
	struct stat fileStat;

	int fd = fileno(curFile);

	printf("File\n");
	char c = fgetc(curFile); 
	while (c != EOF) 
	{ 
		printf ("%c", c); 
		c = fgetc(curFile); 
	} 
	printf("\n");

	// get the file stats
	int statres = fstat(fd, &fileStat);
	printf("stat: %d\n", statres);

	
}
int main() 
{ 
	//test();
	//return 1;



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