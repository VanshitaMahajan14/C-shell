#include "echo.h"
#include "headers.h"
void echo(char *token) 
{    
    token = strtok(NULL," ");
    while(token!=NULL)    //keep parsing on spaces till token is not null
     { 
     printf("%s", token);
     token = strtok(NULL," \n\r\t");
     if(token!=NULL)
     printf(" ");
     }
     printf("\n");   
}

