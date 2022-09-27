#include "prompt.h"
#include "headers.h"
extern char host[1000];
extern char path[1000];
extern char curr_dir[1000];
extern char home_dir[1000];
extern char user[1000];

extern int bg;
extern int diff;

void getval(int *bgPid, char **bgCommand)
{
    int status;
    for (int i = 0; i < 1000; i++)
    {
        if (bgPid[i])
        {
            if (waitpid(bgPid[i], &status, WNOHANG) > 0)
            {
                fprintf(stderr, "%s with PID  = %d exited %s\n", bgCommand[i], bgPid[i],
                        WIFEXITED(status) ? "normally" : "abnormally");

                bgPid[i] = 0;
                free(bgCommand[i]);
            }
        }
    }
}

void prompt()
{
    gethostname(host, 1000);
    uid_t uid;
    uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    strcpy(user, pw->pw_name);
    getcwd(path, 1000); // current path in stored in path

    curr_dir[0] = '~'; // forming the path after hostname

    int path_len = strlen(path);
    int home_len = strlen(home_dir);
    // printf("%d\n",home_len);
    int path_exist = 1;
    if (path_len >= home_len)
    {
        for (int i = 0; i < home_len; i++)
        {
            if (path[i] != home_dir[i])
                path_exist = 0;
        }
        if (path_exist == 1)
        {
            for (int i = 0; i < path_len; i++)
            {
                curr_dir[i + 1] = path[i];
            }
            curr_dir[path_len + 1] = '\0';
        }
    }
    else
        path_exist = 0;
    if (path_exist = 0)
        strcpy(curr_dir, path);
    printf(Cyan);
    printf("<");
    printf("%s@%s", user, host);
    printf(Pink);
    printf("%s", curr_dir);
    if (bg == 0)
    {
        printf(Green);
        printf("~took %d seconds", diff);
        diff = 0;
        printf(Cyan);
        printf(">");
        printf(White);
    }
    else
    {
        printf(Cyan);
        printf(">");
        printf(White);
    }
}
