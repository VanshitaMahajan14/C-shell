#include "headers.h"
int pid;
int diff;
extern char *bgCommand[1000];
char *arr[1000];
void callexec(char **cmd)
{
  if (execvp(cmd[0], cmd) < 0) // serach for command in
  {
    perror("Error");
    exit(1);
  }
}
void syscomm(int bg, int *proc_pid, char **cmd)
{
  pid = fork();
  if (pid > 0) // pid contains pid of child process
  {
    if (bg == 1) // process is background process
    {
      printf("%s started with PID %d\n", cmd[0], pid);
      for (int i = 0; i < 1000; i++)
      {
        if (proc_pid[i] < 1)
        {
          proc_pid[i] = pid;
          int len = strlen(cmd[0]) + 1;
          bgCommand[i] = malloc(len);
          strcpy(bgCommand[i], cmd[0]); // updating comms[i] array for displaying appropriate terminating message
          break;
        }
      }
    }
    else
    {
      time_t s = time(NULL);
      waitpid(pid, NULL, 0);
      time_t e = time(NULL);
      diff += e - s; // calculating time taken
    }
  }
  else if (pid == 0) // pid  = 0 : child process completed
  {
    if (bg > 0)
      setpgid(0, 0); // change bg ID of all processes to 0
    callexec(cmd);
  }
  else if (pid < 0) // -1 : failure
    perror("Error");
}
