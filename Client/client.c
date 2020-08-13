#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 128

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

int main()
{
    while (1)
    {
        char command[MAX];
        gets(command);

        if (strcmp(command, "") == 0)
            continue;

        char* token;
        token = strtok(command, " ");

        if (strcmp(token, "put") == 0)
        {
            int numArguments = 0;
            char** arguments = GetArguments(&numArguments);

        }
        else if (strcmp(token, "sys") == 0)
        {
            printf("sys\n");
        }
        else if (strcmp(token, "quit") == 0)
        {
            exit(0);
        }
    }
}
