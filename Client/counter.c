#include "stdio.h"

int main(int argc, char* argv[])
{
    printf("argc: %d\n", argc);
    for (int i = 0; i < 10; i++)
        printf("%d\n", i);
}