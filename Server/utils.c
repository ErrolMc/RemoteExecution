#include "utils.h"

char** GetArguments(int* arguments)
{
    (*arguments) = 0;

    char* token;
    char** buff = malloc(MAX * sizeof(char*));

    token = strtok(NULL, " ");
    while (token != NULL)
    {
        buff[*arguments] = token;
        (*arguments)++;
        token = strtok(NULL, " ");
    }

    return buff;
}

int DoesFileExist(const char * filename)
{
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r"))
	{
        fclose(file);
        return 1;
    }
    return 0;
}

void TimerStart(struct timespec *tp)
{
    clock_gettime(0, tp);
}

int TimerStop(struct timespec *start)
{
    struct timespec end;
    clock_gettime(0, &end);
    return end.tv_nsec - start->tv_nsec;
}