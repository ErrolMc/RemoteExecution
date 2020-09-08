#include "run.h"

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int GetCreationDate(const char* fileName)
{
    FILE *file;
    if (file = fopen(fileName, "r"))
	{
        struct stat fileStat;
        if (fstat(fileno(file), &fileStat) < 0) 
            return 0;
        
        int creationDate = fileStat.st_ctime;

        fclose(file);
        return creationDate;
    }
    return 0;
}

void ProcessRun(int socketDescriptor, char* buff, char* res)
{
    int numArguments = 0;
    char** arguments = GetArguments(&numArguments);


    char* folder = arguments[0];
    char* fileName_extention = arguments[1];

    char filePath[MAX];
    strcpy(filePath, folder);
    strcat(filePath, "/");
    strcat(filePath, fileName_extention);

    printf("%s\n", filePath);

    int exists = DoesFileExist(filePath);
    if (!exists)
    {
        strcpy(res, "file_error");
        write(socketDescriptor, res, MAX);
        return;
    }

    char* fileName = strtok(fileName_extention, ".");

    char exePath[MAX];
    strcpy(exePath, folder);
    strcat(exePath, "/");
    strcat(exePath, fileName);

    exists = DoesFileExist(exePath);
    int shouldCompile = !exists;
    if (exists)
    {
        int fileCreationDate = GetCreationDate(filePath);
        int exeCreationDate = GetCreationDate(exePath);

        printf("file: %d, exe: %d\n", fileCreationDate, exeCreationDate);

        if (fileCreationDate > exeCreationDate)
            shouldCompile = 1;
    }

    if (shouldCompile)
    {
        printf("Compiling file\n");

        char compileCMD[MAX] = "cc ";
        strcpy(compileCMD, folder);
        strcat(compileCMD, "/");
        strcat(compileCMD, fileName);
        strcat(compileCMD, ".c -o ");
        strcat(compileCMD, folder);
        strcat(compileCMD, "/");
        strcat(compileCMD, fileName);

        FILE* compileOutput = popen(compileCMD, "w");
        fclose(compileOutput);
    }
    else
        printf("Dont need to compile file\n");

    char runCMD[MAX] = "./";
    strcpy(runCMD, folder);
    strcat(runCMD, "/");
    strcat(runCMD, fileName);
    
    // populate the arguments array
    char* args[numArguments];
    args[0] = fileName;
    for (int i = 2, ind = 1; i < numArguments; i++, ind++)
        args[ind] = arguments[i];
    args[numArguments-1] = NULL;

    // run the program
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
