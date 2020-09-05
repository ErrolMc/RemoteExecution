#include "list.h"

void ProcessList(int socketDescriptor, char* buff, char* res)
{
    int numArguments = 0;
    char** arguments = GetArguments(&numArguments);

    char* directory;
    int longList = 0;
    for (int i = 0; i < numArguments; i++)
    {
        if (strcmp(arguments[i], "-l") == 0)
            longList = 1;
        else
            directory = arguments[i];
    }

    struct dirent *ep;
    struct stat t_stat;
    DIR *dp = opendir(directory);

    char fileName[MAX];
    char filePerms[MAX];
    char fileSize[MAX];

    if (dp != NULL)
    {
        while (ep = readdir(dp))
        {
            if (ep->d_name[0] == '.')
                continue;

            strcpy(fileName, directory);
            strcat(fileName, "/");
            strcat(fileName, ep->d_name);

            stat(fileName, &t_stat);

            if (longList == 1)
            {
                struct tm * timeinfo = localtime(&t_stat.st_ctime);
                strcpy(buff, ep->d_name);   
                strcat(buff, " | ");

                int size = t_stat.st_size;
                sprintf(fileSize, "%d", size);
                strcat(buff, fileSize);

                strcat(buff, " | ");

                strcpy(filePerms, (S_ISDIR(t_stat.st_mode)) ? "d" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IRUSR) ? "r" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IWUSR) ? "w" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IXUSR) ? "x" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IRGRP) ? "r" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IWGRP) ? "w" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IXGRP) ? "x" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IROTH) ? "r" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IWOTH) ? "w" : "-");
                strcat(filePerms, (t_stat.st_mode & S_IXOTH) ? "x" : "-");   

                strcat(buff, filePerms);
                
                strcat(buff, " | ");
                strcat(buff, asctime(timeinfo));
            }
            else
            {
                strcpy(buff, ep->d_name);
                strcat(buff, "\n");
            }

            write(socketDescriptor, buff, MAX);
        }

        strcpy(buff, "list_done");
        (void)closedir(dp);
    }
    else
    {
        strcpy(buff, "cant_open_file");
    }

    write(socketDescriptor, buff, MAX);
}