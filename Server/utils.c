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