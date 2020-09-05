#include "list.h"

void ProcessList(int socketDescriptor, char* buff, char* res)
{
    // send the command
    write(socketDescriptor, buff, MAX);

    while (1)
    {
        bzero(res, MAX);
        read(socketDescriptor, res, MAX);

        if (strcmp(res, "list_done") == 0)
        {
            break;
        }
        else if (strcmp(res, "cant_open_file") == 0)
        {
            printf("server > cant open directory\n");
            break;
        }
        else
        {
            printf("%s", res);
        }
    }
}