#ifndef UTILS_H
#define UTILS_H

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

char** GetArguments(int* arguments);
int DoesFileExist(const char * filename);

#endif