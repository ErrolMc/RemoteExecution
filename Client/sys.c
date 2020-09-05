#include "sys.h"

void ProcessSys(int socketDescriptor, char* buff, char* res)
{
    // send the command
    strcpy(buff, "sys");
    write(socketDescriptor, buff, MAX);

    while (1)
    {
        read(socketDescriptor, res, MAX);

        if (strcmp(res, "error") == 0)
        {
            printf("server > sys error\n");
            break;
        }
        else if (strcmp(res, "done") == 0)
        {
            break;
        }
        else
        {
            printf("%s", res);
        }
    }
}