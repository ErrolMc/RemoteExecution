#include "put.h"

void ProcessPut(int socketDescriptor, char* buff, char* res)
{
    int numArguments = 0;
    char** arguments = GetArguments(&numArguments);

    if (numArguments > 0)
    {
        // send the line indicating the files
        write(socketDescriptor, buff, MAX); 

        // wait for a response
        read(socketDescriptor, buff, MAX); 

        if (strcmp(buff, "ready") == 0)
        {
            FILE* curFile;
            struct stat fileStat;

            // send the files 1 by 1
            for (int i = 1; i < numArguments; i++)
            {
                // make sure we arent the -f arg
                if (strcmp(arguments[i], "-f") == 0) 
                    continue;

                bzero(res, MAX); 
                bzero(buff, MAX);
                
                int exists = DoesFileExist(arguments[i]);
                if (exists)
                {
                    // open the file
                    curFile = fopen(arguments[i], "r"); 

                    if (curFile == -1)
                        continue;

                    // get the file stats
                    int statres = fstat(fileno(curFile), &fileStat);
                    if (statres < 0) 
                        continue;
                    
                    sprintf(res, "%d", fileStat.st_size);

                    // write the file size
                    write(socketDescriptor, res, MAX);

                    // read if we should send the file
                    read(socketDescriptor, buff, MAX);
                    
                    if (strcmp(buff, "read") != 0)
                        continue;
                    
                    while (fgets(buff, MAX, curFile) != NULL)
                    {
                        if (send(socketDescriptor, buff, MAX, 0) == -1)
                        {
                            printf("Error sending data\n");
                            break;
                        }
                    }

                    strcpy(buff, "client_send_done");
                    send(socketDescriptor, buff, MAX, 0);

                    fclose(curFile);
                }
                else
                {
                    printf("File %s doesnt exist\n", arguments[i]);
                    strcpy(buff, "file_error");
                    write(socketDescriptor, buff, MAX);
                }
            }
        }
        else if (strcmp(buff, "error") == 0)
        {
            return;
        }
    }
}