#include "sys.h"

void ProcessSys(int socketDescriptor, char* buff, char* res)
{
    struct utsname unameData;
    if (uname(&unameData) == -1)
    {
        strcpy(buff, "error");
        write(socketDescriptor, buff, MAX);
    }
    else
    {
        sprintf(buff, "system name: %s\n", unameData.sysname);
        write(socketDescriptor, buff, MAX);
        sprintf(buff, "node name: %s\n", unameData.nodename);
        write(socketDescriptor, buff, MAX);
        sprintf(buff, "release: %s\n", unameData.release);
        write(socketDescriptor, buff, MAX);
        sprintf(buff, "version: %s\n", unameData.version);
        write(socketDescriptor, buff, MAX);
        sprintf(buff, "machine: %s\n", unameData.machine);
        write(socketDescriptor, buff, MAX);

        strcpy(buff, "done");
        write(socketDescriptor, buff, MAX);
    }
}