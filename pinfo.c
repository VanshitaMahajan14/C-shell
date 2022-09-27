#include "headers.h"
#include "pinfo.h"
#define buflen 50
extern char home_dir[1000];
char buf[1000] = "";
pid_t cpid;
char *store;
extern char *pinfocmd[50]; // stores the arguments after pinfo
char procinfo[30][50];

char status[1000];
char memory[1000];
char proc_path[1000];

char path_stat[1000];
char path_status[1000];
char path_exe[1000];

int numargs(char *token) // counts number of arguments after pinfo command
{
    int count = 0;
    while (token != NULL)
    {
        pinfocmd[count] = token;
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}
void checkpins(int a) // return info according to value of a
{
    int c, len = 0;
    if (a == 0)          // a == 0 --- no id provided after pinfo
        cpid = getpid(); // get ID of current process using getpid
    else if (a == 1)     // id is provided
        cpid = atoi(pinfocmd[1]);

    int fore = 0;

    sprintf(path_stat, "/proc/%d/stat", cpid);
    sprintf(path_status, "/proc/%d/status", cpid);
    sprintf(path_exe, "/proc/%d/exe", cpid);

    FILE *fd = fopen(path_stat, "r");
    if (!fd)
    {
        perror(path_stat);
        return;
    }
    else
    {

        for (int i = 0; i < 23; i++)
            fscanf(fd, "%s", procinfo[i]);
        strcpy(status, procinfo[2]);
        strcpy(memory, procinfo[22]);
        int tgid = atoi(procinfo[4]);
        int pid = atoi(procinfo[7]);

        if (tgid == pid) // checking if foreground process
            fore = 1;
        if (fore == 0)
            status[1] = 0;
        else
        {
            status[1] = '+';
            status[2] = 0;
        }
        fclose(fd);
    }

    if (readlink(path_exe, buf, 1024) <= 0)
    {
        perror(path_exe);
        return;
    }

    else // forming path  - same as prompt
    {
        int path_len = strlen(buf);
        int home_len = strlen(home_dir);
        proc_path[0] = '~';
        int path_exist = 1;

        if (path_len >= home_len)
        {
            for (int i = 0; i < home_len; i++)
            {
                if (buf[i] != home_dir[i])
                    path_exist = 0;
            }
            if (path_exist == 1)
            {
                for (int i = 0; i < path_len; i++)
                    proc_path[i + 1] = buf[i];
                proc_path[path_len + 1] = '\0';
            }
        }
        else
            path_exist = 0;
        if (path_exist == 0)
            strcpy(proc_path, buf);
    }
    printf("Pid  :   %d\n", cpid);
    printf("Process Status  :  %s\n", status);
    printf("Memory  :  %s\n", memory);
    printf("Executable Path  :  %s\n", proc_path);
}
