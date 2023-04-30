#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(){

    char *** all_cmd;//建立一个大的三维数组来储存所有的command

    int i,j;

    all_cmd=malloc(100*sizeof(char **));

    for(i=0; i<100; i++){
        all_cmd[i]=malloc(20*sizeof(char *));
        for(j=0; j<20; j++){
            all_cmd[i][j]=malloc(156*sizeof(char));
        }
    }

    char curr_line[156];
    int cmd_index;

    while(scanf("%[^\n]%*c",curr_line)!=EOF){//当输入扫到\n的时候不是文件的结尾
        cmd_index++;//每次检索到换行符的时候 而且又不是文件的结尾 cmd_index+1（cmd_index代表的是总行数）
        char * single_cmd=strtok(curr_line," ");
        char * cmd_array[]={};
        int count=0;
        while(single_cmd!=NULL){
            single_cmd=strtok(NULL," ");
            count=count+1;//计算一行一共有多少single command
        }

        for(int m=0; m<cmd_index; m++){
            strcpy(all_cmd[cmd_index][m],cmd_array[m]);
        }

        all_cmd[cmd_index-1][count]=NULL;//将数组的最后一行的最后一个元素设为空，让while在合适的条件下停止运行
    }

    int new_pipe[2];//为新管道建立array
    int prev_pipe[2];//为旧管道建立array

    for(int l=0; l<cmd_index; l++){

        //如果不是最后一行，那就建立新管道
        if(l!=cmd_index-1){
            pipe(new_pipe);
        }

        //建立fork（）函数，开始产生子进程
        pid_t id=fork();

        //如果fork（）函数打开不成功，停止运行，并打印出来
        if(id<0){
            printf("Fail to fork!");
            exit(1);
        }else if(id==0){ //当fork（）函数打开成功，并且正处于子进程之中时

        if(l!=0){ //if this is not the first command:

        close(prev_pipe[1]);//Redirect iutput to prev_pipe
        dup2(prev_pipe[0],0);
        close(prev_pipe[0]);
        }

        if(l!=cmd_index-1){ //if this is not the final command:

            close(new_pipe[0]);  //Redirect output to new_pipe
            dup2(new_pipe[1],1);
            close(new_pipe[1]);
        }

        //execute command
        execvp(all_cmd[l][0],all_cmd[l]);

        exit(0);

        }else{ //当处于父进程的时候

        //if this is not the first command:
        if(l!=0){
            close(prev_pipe[0]); //close prev_pipe
            close(prev_pipe[1]);
        }

        //if this is not the final command:

        if(l!=cmd_index-1){
          //prev_pipe = new_pipe
          prev_pipe[0]=new_pipe[0];
          prev_pipe[1]=new_pipe[1];
          wait(NULL);//Wait until child process finish
        }

        close(new_pipe[0]); //close any remaining pipes, clean up
        close(new_pipe[1]);
        }
    }

    //Delete the 3D array

        for(i=0; i<100; i++){
            for(j=0; j<20; j++){
                free(all_cmd[i][j]);
            }
        }

        free(all_cmd);

    return 0;
}