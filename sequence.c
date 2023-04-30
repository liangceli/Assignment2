#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(){

    char line[256];

    char* cmd;

    char* cmd_array[100]={};

    while(scanf("%[^\n]%*c",line)!=EOF){

        int i=0;
        cmd=strtok(line," ");

        while(cmd!=NULL){
            cmd_array[i]=cmd;
            cmd=strtok(NULL," ");
            i=i+1;
        }
    

    pid_t id=fork();
    

    if(id<0){
        printf("Fail to fork!\n");
        exit(1);
    }else if(id==0){
        execvp(cmd_array[0],cmd_array);
        exit(0);
    }else{
        wait(NULL);
    }
    }

    return 0;
}

