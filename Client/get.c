#include "get.h"

void ProcessGet(int socketDescriptor, char* buff, char* res)
{
    // send the command
    write(socketDescriptor, buff, MAX);

    while (1)
    {
        bzero(buff, MAX);
        read(socketDescriptor, buff, MAX);

        if (strcmp(buff, "wait") == 0)
        {
            printf("Enter any character to continue > ");
            gets();
            strcpy(res, "continue");
            write(socketDescriptor, res, MAX);
        }
        else if (strcmp(buff, "done") == 0)
        {
            break;
        }
        else if (strcmp(buff, "file_doesnt_exist") == 0)
        {
            printf("Cant find file on server");
            break;
        }
        else
        {
            printf("%s", buff);
        }
    }

    printf("\n");
}