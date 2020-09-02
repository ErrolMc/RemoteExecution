#include "run.h"

void ProcessRun(int socketDescriptor, char* buff, char* res)
{
    write(socketDescriptor, buff, MAX);

    while (1)
    {
        bzero(buff, MAX);
        read(socketDescriptor, buff, MAX);

        if (strcmp(buff, "done") == 0)
        {
            break;
        }
        else
        {
            printf("%s", buff);
        }
    }
}