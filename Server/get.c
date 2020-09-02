#include "get.h"

void ProcessGet(int socketDescriptor, char* buff, char* res)
{
    int numArguments = 0;
    char** arguments = GetArguments(&numArguments);

    if (numArguments < 2)
    {
        strcpy(res, "done");
        write(socketDescriptor, res, MAX);
        return;
    }

    // see if the directory exists
    char directory[MAX] = "./";
    strcat(directory, arguments[0]);

    struct stat st;
    if (stat(directory, &st) == -1)
    {
        strcpy(res, "done");
        write(socketDescriptor, res, MAX);
        return;
    }

    strcat(directory, "/");
    strcat(directory, arguments[1]);
    int exists = DoesFileExist(directory);

    if (!exists)
    {
        strcpy(res, "done");
        write(socketDescriptor, res, MAX);
    }
    else
    {
        FILE* file = fopen(directory, "r");

        int curLines = 0;
        while (fgets(res, MAX, file))
        {
            write(socketDescriptor, res, MAX);
            curLines++;
            
            if (curLines == GET_LINE_COUNT)
            {
                curLines = 0;

                strcpy(res, "wait");
                write(socketDescriptor, res, MAX);

                // wait for a key to be pressed
                read(socketDescriptor, buff, MAX);
            }
        }

        printf("Done get\n");

        strcpy(res, "done");
        write(socketDescriptor, res, MAX);
    }
}