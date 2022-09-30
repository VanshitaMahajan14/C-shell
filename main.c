#include "headers.h"

char host[1000];
char path[1000];
char curr_dir[1000];
char home_dir[1000];
char user[1000];
char command[100];
char curr[1000];
char home_path[1000];
char prev_dir[1000];
char str[100];

char *backarr[1000];
char *forearr[1000];

char hisarray[20][50]; // array to store command history
char *directory[50];   // array to store cd and its flags
char *lsflag[50];      // array to store ls and its flags
char *pinfocmd[50];    // for pinfo command
char *discargs[1000];  // for discover command

char *commands[1000];  // store ; tokenised commands
char *cmd[1000];       // store space separated commands that are not built-in
int bgPid[1000];       // store process IDs of backgroud commands
char *bgCommand[1000]; // array that stores background commands

int yes = 0;
int count = 0;
int ck; // flag that differentiates whether all processes are bg or last process is fg
int bg = 1;
int *bgarray[1000];

FILE *ptr;

void readfile()
{
    ptr = fopen("his.txt", "r");
    while (fgets(str, sizeof(str), ptr))
    {
        strcpy(hisarray[count], str);
        hisarray[count][strlen(hisarray[count]) - 1] = '\0';
        count++;
    }
}
char b[100];
int main()
{
    getval(bgPid, bgCommand);
    printf("\n\n           Welcome to my shell!\n\n\n");
    getcwd(home_path, 1000); // get current path and store in home_path for future use in various commands - cd , ls ,etc.
    readfile();              // read previous commands from his.txt and store in array
    while (1)
    {
        prompt();
        getval(bgPid, bgCommand); // prints process status - terminated normally / abnormally
        bg = 1;                   // by deafult a background process unless flag becomes 0 after checking consitions
        char *a = malloc(sizeof(char) * 1000);
        fgets(a, 1000, stdin);
        if (strcmp(a, "\n") == 0)
            continue;
        strcpy(b, a);
        if (count == 0)
        {
            strcpy(hisarray[count], a);
            count++;
        }
        else
        {
            if (count == 20)
            {
                int len1 = strlen(hisarray[19]) - 1; // comparing newly entered command with previous command in history array
                if (hisarray[19][len1] == '\n')
                    hisarray[19][len1] = '\0';
                int len2 = strlen(a) - 1;
                if (a[len2] == '\n')
                    a[len2] = '\0';
                if (strcmp(hisarray[19], a) != 0)
                {
                    for (int i = 0; i <= 18; i++)
                        strcpy(hisarray[i], hisarray[i + 1]);

                    strcpy(hisarray[19], a);
                }
            }
            else
            {
                int len1 = strlen(hisarray[19]) - 1;
                if (hisarray[count - 1][len1] == '\n')
                    hisarray[count - 1][len1] = '\0';
                int len2 = strlen(a) - 1;
                if (a[len2] == '\n')
                    a[len2] = '\0';
                if (strcmp(hisarray[count - 1], a) != 0)
                {
                    strcpy(hisarray[count], a);
                    count++;
                }
            }
        }
        char *toksem = strtok(a, ";"); // tokenise based on semicolon to split multiple commands
        int counter = 0;
        while (toksem != NULL)
        {
            commands[counter] = toksem;
            toksem = strtok(NULL, ";");
            counter++;
        }
        for (int i = 0; i < counter; i++)
        {
            int amperse = 0;
            int countamp = 0;
            for (int j = 0; j < strlen(commands[i]); j++)
            {
                if (commands[i][j] == '&') // count number of &s in command
                    amperse++;
            }
            char *amp = strtok(commands[i], "&");
            while (amp != NULL)
            {
                backarr[countamp] = amp;
                amp = strtok(NULL, "&");
                countamp++;
            }
            if (amperse == countamp) // all commands are background if &-parsed token count equals amperse count
                ck = 1;
            else // last command is foreground
                ck = 0;
            for (int k = 0; k < countamp; k++) 
            {
                char *token = strtok(backarr[k], " ");
                if (strcmp(token, "echo") == 0)
                {
                    echo(token);
                }
                else if (strcmp(token, "pwd") == 0)
                {
                    pwd();
                }
                else if (strcmp(token, "cd") == 0)
                {
                    int cnt = cdArgs(token);
                    cnt--;
                    check(cnt);
                }
                else if (strcmp(token, "ls") == 0)
                {
                    int count = cntlsArgs(token);
                    count--;
                    ls(count);
                }
                else if (strcmp(token, "history") == 0)
                {
                    int min = (count < 10) ? count : 10;
                    if (min == 10)
                    {
                        for (int i = count - 10; i < count; i++)
                        {
                            int j = strlen(hisarray[i]) - 1;
                            if (hisarray[i][j] == '\n')
                                hisarray[i][j] = '\0';
                            printf("%s\n", hisarray[i]);
                        }
                    }
                    else if (min == count)
                    {
                        for (int i = 0; i < count; i++)
                        {
                            {
                                int j = strlen(hisarray[i]) - 1;
                                if (hisarray[i][j] == '\n')
                                    hisarray[i][j] = '\0';
                                printf("%s\n", hisarray[i]);
                            }
                        }
                    }
                }
                else if (strcmp(token, "exit") == 0)
                {
                    chdir(home_path);
                    ptr = fopen("his.txt", "w");
                    {
                        for (int j = 0; j < count; j++)
                            fprintf(ptr, "%s\n", hisarray[j]);
                    }
                    fclose(ptr);
                    exit(0);
                }
                else if (strcmp(token, "pinfo") == 0)
                {
                    int pins = numargs(token);
                    if (pins > 2)
                    {
                        fprintf(stderr, "pinfo: Too many arguments\n");
                    }
                    pins--;
                    checkpins(pins);
                }
                else if (strcmp(token, "discover") == 0)
                {
                    int num = count_arg(token);
                    num--;
                    checkcount(num, discargs);
                }
                else
                {
                    if (ck == 0 && k == (countamp - 1)) // process is foreground
                        bg = 0;
                    else if (ck == 0 && k < countamp - 1) // process is background
                        bg = 1;
                    else if (ck == 1)
                        bg = 1;
                    cmd[0] = token; // parsing commands based on space :  eg. sleep 2 into sleep and 2
                    int k = 1;
                    while (token != NULL)
                    {
                        token = strtok(NULL, " ");
                        cmd[k] = token;
                        k++;
                    }
                    syscomm(bg, bgPid, cmd); // function to call to run fg / bg command
                    getval(bgPid, bgCommand);
                }
            } // amperse for ends
        }     // semicolon for ends
    }         // while ends
}
