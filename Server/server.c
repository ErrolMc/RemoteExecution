#include "utils.h"
#include "put.h"
#include "get.h"
#include "run.h"
#include "list.h"
#include "sys.h"

// Function designed for chat between client and server. 
void ChatToClient(int socketDescriptor) 
{ 
	char buffCopy[MAX];
	char res[MAX];
	char buff[MAX]; 

	struct timespec ts;

    // infinite loop for chat 
	for (;;) 
    { 
		bzero(buff, MAX);
		bzero(res, MAX);

		// first read (PAUSES UNTIL THERES DATA)
		read(socketDescriptor, buff, sizeof(buff)); 
		strcpy(buffCopy, buff);

		printf("client > %s\n", buff);
		TimerStart(&ts);

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
			write(socketDescriptor, res, MAX);
		}

		// report the time taken
		int timeTaken = TimerStop(&ts);
		sprintf(res, "server > time taken: %d\n", timeTaken);
		write(socketDescriptor, res, MAX);
	} 
} 

void ZombieKill(int sig)
{
	int status;
	if (waitpid(-1, &status, WNOHANG) < 0)
		printf("Zombie Process Killed\n");
}

// Driver function 
int main() 
{ 
	const int BACKLOG = 5;

	int socketFD, connFD; 
	struct sockaddr_in serverAddress, clientAdd; 
	pid_t cpid;

	signal(SIGCHLD, ZombieKill);

	// socket create and verification 
	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketFD == -1) 
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
	if ((bind(socketFD, (SA*)&serverAddress, sizeof(serverAddress))) != 0) 
    { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(socketFD, BACKLOG)) != 0) 
    { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
    
	while (1)
	{
		// Accept the data packet from client and verification 
		int clientLen = sizeof(clientAdd); 
		connFD = accept(socketFD, (SA*)&clientAdd, &clientLen); 
		if (connFD < 0) 
			exit(0);
		else
			printf("Server acccepted a client...\n"); 

		// accept multiple clients
		cpid = fork();
		if (cpid < 0)
		{
			printf("fork failed\n");
			exit(0);
		}
		else if (cpid == 0)
		{
			close(socketFD);

			// Function for chatting between client and server 
			ChatToClient(connFD); 

			close(connFD);
		}
	}

	// After chatting close the socket 
	close(socketFD); 
} 