#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>

int main(){

     // 在创建子进程之前创建管道，这样管道的读入端和写出端的文件描述符才会与
     // 创建的子进程相同
     int pipefd[2];
     int ret = pipe(pipefd);

     // 创建子进程
     pid_t pid = fork();

     if(pid > 0){
         // 父进程
         // 从管道的输出端读取数据
        char buf[1024] = {0};
        int len = read(pipefd[0], buf, sizeof(buf));
        printf("parent recv : %s, pid : %d\n", buf, pid);
     }else if(pid == 0){
         // 子进程
         char * str = "child process~~";
         write(pipefd[1], str, strlen(str));
     }else{
         // 创建进程错误
         perror("fork");
         exit(0);
     }


    return 0;
}