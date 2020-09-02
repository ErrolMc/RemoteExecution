#include "put.h"

void ProcessPut(int socketDescriptor, char* buff, char* res)
{
    int numArguments = 0;
    char** arguments = GetArguments(&numArguments);

    // create the directory
    char directory[MAX] = "./";
    strcat(directory, arguments[0]);

    struct stat st;
    if (stat(directory, &st) == -1)
        mkdir(directory, 0777);

    // check if we are overriding
    int overrite = 0;
    int numFiles = 0;
    for (int i = 1; i < numArguments; i++)
    {
        if (strcmp(arguments[i], "-f") == 0)
            overrite = 1;
        else
            numFiles++;
    }

    // check if there are files to even upload
    if (numFiles == 0)
    {
        strcpy(res, "error");
        write(socketDescriptor, res, MAX);
        return;
    }

    // send a ready response
    strcpy(res, "ready");
    write(socketDescriptor, res, MAX);

    bzero(res, MAX);
    bzero(buff, MAX);

    FILE* curFile;
    for (int i = 1; i <= numFiles; i++)
    {
        read(socketDescriptor, res, MAX);
        int fileSize = atoi(res);
        
        // get the file name
        char fileName[MAX];
        strcpy(fileName, directory);
        strcat(fileName, "/");
        strcat(fileName, arguments[i]);

        int exists = DoesFileExist(fileName);
        int shouldRead = !(exists && !overrite);

        strcpy(res, shouldRead ? "read" : "continue");

        write(socketDescriptor, res, MAX);
        if (!shouldRead)
            continue;

        curFile = fopen(fileName, "w+");
        
        ssize_t remainingData = fileSize;
        while (remainingData > 0)
        {
            ssize_t n = recv(socketDescriptor, buff, MAX, 0);
            remainingData -= n;
            fprintf(curFile, "%s", buff);
        }

        printf("read file\n");

        fclose(curFile);
    }
    
    free(arguments);
}