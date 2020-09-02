#include "run.h"

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

void ProcessRun(int socketDescriptor, char* buff, char* res)
{
    int numArguments = 0;
    char** arguments = GetArguments(&numArguments);

    char* folder = arguments[0];
    char* fileName = arguments[1];

    char compileCMD[MAX] = "cc ";
    strcat(compileCMD, folder);
    strcat(compileCMD, "/");
    strcat(compileCMD, fileName);
    strcat(compileCMD, ".c -o ");
    strcat(compileCMD, folder);
    strcat(compileCMD, "/");
    strcat(compileCMD, fileName);

    FILE* compileOutput = popen(compileCMD, "w");
    fclose(compileOutput);

    char runCMD[MAX] = "./";
    strcat(runCMD, folder);
    strcat(runCMD, "/");
    strcat(runCMD, fileName);

    char* args[] = { fileName, NULL };

    int link[2];
    pid_t pid;
    char progBuff[MAX];
    bzero(progBuff, MAX);

    if (pipe(link) == -1)
        die("pipe");

    if ((pid = fork()) == -1)
        die("fork");

    if (pid == 0)
    {
        dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);

        execv(runCMD, args);
        die("execv");
    }
    else
    {
        close(link[1]);
        int nBytes = 0;
        while (0 != (nBytes = read(link[0], progBuff, MAX)))
        {
            //printf("%s", progBuff);
            write(socketDescriptor, progBuff, MAX);
            bzero(progBuff, MAX);
        }
        wait(NULL);
    }

    strcpy(res, "done");
    write(socketDescriptor, res, MAX);
    //printf("done\n");
}
