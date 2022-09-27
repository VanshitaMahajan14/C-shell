// handles the flags: . | .. | ~ | - | folder_name | folder_path

#include "headers.h"
#include "cd.h"

extern char *directory[50];
extern char home_path[1000];
extern char prev_dir[1000];
extern char curr_dir[1000];

int cdArgs(char *token) // this function counts number of arguments after cd command
{
    int count = 0;
    while (token != NULL)
    {
        directory[count] = token;
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}

void check(int a)
{
    if (a == 0) // only cd - go to home directory
    {
        chdir(home_path);
    }
    else if (a == 1) // one command after cd
    {
        if (strcmp(directory[1], ".") == 0) // stay in same directory
        {

            getcwd(curr_dir, 1000);
            chdir(curr_dir);
            getcwd(prev_dir, 1000);
        }
        if (strcmp(directory[1], "..") == 0) // go to prev directory in hierarchy
        {
            getcwd(prev_dir, 1000);
            if (chdir("..") != 0)
                perror("bash: cd:");
        }
        else if (strcmp(directory[1], "~") == 0) // go to home directory
        {
            getcwd(prev_dir, 1000);
            chdir(home_path);
            // getcwd(prev_dir, 1000);
        }
        else if (strcmp(directory[1], "-") == 0) // go to last directory visited
        {                                        // printf("%s\n", prev_dir);
            chdir(prev_dir);
            getcwd(prev_dir, 1000);
        }
        else // some folder name is provided
        {
            getcwd(prev_dir, 1000);
            // printf("%s\n",prev_dir);
            if (chdir(directory[1]) != 0)
                perror("bash: cd");
            else
                chdir(directory[1]);
        }
    }
    else // more than 1 command line argument
    {
        perror("");
    }
}
