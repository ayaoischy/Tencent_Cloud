/* 

    实现两个进程之间简单的聊天对话

    1.创建两个有名管道
    2.打开管道
    3.从标准输入循环写读数据
    4.程序结束关闭管道

*/
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

using namespace std;

int main(){

    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        // 判断管道文件是否已经存在
        int ret = access("fifo1", F_OK);
        if(ret == -1){
            // 管道文件不存在，需要创建管道文件
            cout << "管道不存在，创建有名管道1" << endl;
            ret = mkfifo("fifo1", 0664);
            if(ret == -1){
                perror("mkfifo");
                exit(0);
            }
        }

        // 打开管道
        int fdr = open("fifo1", O_RDONLY);
        if(fdr == -1){
            perror("open");
            exit(0);
        }
        cout << "打开管道1成功，等待读取..." << endl;

        // 读数据
        char buf[128];
        while(1){
            memset(buf, 0, 128);
            // 读数据
            ret = read(fdr, buf, 128);
            if(ret == -1){
                perror("read");
                break;
            }

            // 打印输出读取到的数据
            cout << buf << endl;
        }

        // 关闭管道文件描述符
        close(fdr);

    }else if(pid == 0){
        // 子进程
        int ret = access("fifo2", F_OK);
        if(ret == -1){
            // 管道文件不存在，需要创建管道文件
            cout << "管道不存在，创建有名管道2" << endl;
            ret = mkfifo("fifo2", 0664);
            if(ret == -1){
                perror("mkfifo");
                exit(0);
            }
        }
        int fdw = open("fifo2", O_WRONLY);
        if(fdw == -1){
            perror("open");
            exit(0);
        }
        cout << "打开管道2成功，等待写入..." << endl;

        // 从标准输入循环写数据
        char buf[128];
        while(1){
            memset(buf, 0, 128);
            // 获取标准输入的数据
            fgets(buf, 128, stdin);
            // 写数据
            ret = write(fdw, buf, strlen(buf));
            if(ret == -1){
                perror("write");
                exit(0);
            }
        }

        // 关闭管道文件的文件描述符
        close(fdw);
        
    }else{
        perror("fork");
        exit(0);
    }

    return 0;
}