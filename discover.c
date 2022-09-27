#include "discover.h"
#include "headers.h"
extern char *discargs[1000];
char *content[1000];
char direc[1000];
char file[1000];
char curr[1000];
int flag = 0;

int count_arg(char *token) // counts number of arguments after discover
{
    int count = 0;
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        discargs[count] = token;
        count++;
    }
    return count;
}
int co = 0;

void disc(char *direc)
{
    struct dirent *ent;
    DIR *dh = opendir(direc);
    if (!dh)
        return;
    while ((ent = readdir(dh)) != NULL)
    {
        co++;
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
        {
            char path[1000];
            strcpy(path, direc);
            strcat(path, "/");
            strcat(path, ent->d_name);
            printf("%s\n", path);
            disc(path);
        }
    }
}
void direconly(char *direc)
{
    struct dirent *ent;
    DIR *dh = opendir(direc);
    if (!dh)
        return;
    while ((ent = readdir(dh)) != NULL)
    {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
        {
            char path[1000];
            strcpy(path, direc);
            strcat(path, "/");
            strcat(path, ent->d_name);
            if (ent->d_type == DT_DIR)
            {
                flag = 1;
                printf("%s\n", path);
            }
            direconly(path);
        }
    }
}
void fileonly(char *direc)
{
    struct dirent *ent;
    DIR *dh = opendir(direc);
    if (!dh)
        return;
    while ((ent = readdir(dh)) != NULL)
    {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
        {
            char path[1000];
            strcpy(path, direc);
            strcat(path, "/");
            strcat(path, ent->d_name);
            if (ent->d_type == DT_REG)
            {
                flag = 1;
                printf("%s\n", path);
            }
            fileonly(path);
        }
    }
}
void discfind(char *direc, char *file)
{
    struct dirent *ent;
    DIR *dh = opendir(direc);
    if (!dh)
        return;
    while ((ent = readdir(dh)) != NULL)
    {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
        {
            char path[1000];
            strcpy(path, direc);
            strcat(path, "/");
            strcat(path, ent->d_name);
            if (strcmp(ent->d_name, file) == 0)
            {
                printf("%s\n", path);
                flag = 1;
            }
            discfind(path, file);
        }
    }
}
void checkcount(int count, char *args[])
{
    if (count == 0) // discover
    {
        disc(".");
    }
    else
    {
        int d = 0, f = 0, k = 0;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(args[i], "-d") == 0)
                d = 1;
            else if (strcmp(args[i], "-f") == 0)
                f = 1;
            else
            {
                content[k] = args[i];
                k++;
            }
        }           // for ends
        if (k == 0) // no file/directory name, only flags
        {
            if (d == 1 && f == 0) // discover -d
            {
                direconly(".");
                if (flag == 0)
                    printf("No directories exist in this folder\n");
                flag = 0;
            }
            else if (d == 0 && f == 1) // discover -f
            {
                fileonly(".");
                if (flag == 0)
                    printf("No files exist in this folder\n");
                flag = 0;
            }
            else if (d == 1 && f == 1 || d == 0 && f == 0) // discover -d -f : same as discover
            {
                disc(".");
                if (co == 0)
                    printf("Folder is empty\n");
                co = 0;
            }
        }
        else if (k == 1) // directory or file name provided
        {
            if (content[0][0] == '"') // file name
            {
                int len = strlen(content[0]);
                char a[len];
                for (int i = 0; i < strlen(content[0]) - 2; i++)
                {
                    a[i] = content[0][i + 1];
                }
                discfind(".", a);
            }
            else // directory name
            {
                if (d == 1 && f == 0)
                {
                    direconly(content[0]);
                    if (flag == 0)
                        printf("No directories exist in this folder\n");
                    flag = 0;
                }
                else if (d == 0 && f == 1)
                {
                    fileonly(content[0]);
                    if (flag == 0)
                        printf("No files exist in this folder\n");
                    flag = 0;
                }
                else if (d == 1 && f == 1 || d == 0 && f == 0)
                    disc(content[0]);
            }
        }
        else if (k == 2) // k==2
        {
            if (content[0][0] == '\"' && content[1][0] == '\"')
            {
                printf("Invalid Command: too many files\n");
            }
            else if (content[0][0] != '\"' && content[1][0] != '\"')
            {
                printf("Invalid Command: too many directories\n");
            }
            else
            {
                for (int i = 0; i < strlen(content[1]) - 2; i++)
                {
                    file[i] = content[1][i + 1];
                }
                strcpy(direc, content[0]);
                struct dirent *ent;
                DIR *dh = opendir(direc);
                if (!dh)
                {
                    perror("discover ");
                }
                discfind(direc, file);
                if (flag == 0)
                    fprintf(stderr, "File not found\n");
                flag = 0;
            }
        }
        else
        {
            printf("Invalid Command: too many arguments\n");
        }
    }
}