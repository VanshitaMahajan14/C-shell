#include "headers.h"
#include "pwd.h"
extern char curr[1000];
void pwd()
{ 
    getcwd(curr,1000);   //get path of current directort using getcwd command
    printf("%s\n",curr);
}
