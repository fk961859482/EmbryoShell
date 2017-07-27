/*************************************************************************
	> File Name: dup.c
	> Author: fangkang
	> Mail: 
	> Created Time: Thu 27 Jul 2017 17:23:59 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    int pfds[2];
    // create the pipe Filedescription pfds[0],pfds[1],
    // because pipe() is earlier than fork(),so that father and child do not share the pfds.
    if(pipe(pfds)==0)
    {
        // in the chlid process
        if(fork()==0)
        {
            //close the stdout 
            close(1);
            //dup the stdout to pfds[1]
            dup2(pfds[1],1);
            //close the pfds[0] as it doesn't been used
            close(pfds[0]);
            //to run "ls -l",the output will go into pfds[1],not stdout
            execlp("ls","ls","-l",NULL);
        }
        // in the father process
        else  
        {
            // almost the same in child process
            close(0);
            dup2(pfds[0],0);
            close(pfds[1]);
            //to run "wc -l" ,the input comes from pfds[0],not stdin
            execlp("wc","wc","-l",NULL);
        }
    }
    return 0;
}
/*
        在上述代码中，首先在第9行代码中建立一个管道，然后将应用程序分成两个进程：一个

子进程和一个父进程）。接下来，在子进程中首先关闭stdout描述符，然后提供了ls –1命令功能，

不过它不是写到stdout，而是写到我们建立的管道的输入端，这是通过dup函数来完成重定向的。

使用dup2 函数把stdout重定向到管道（pfds[1]）。之后，马上关掉管道的输入端。然后，使用

execlp函数把子进程的映像替换为命令ls –1的进程映像，一旦该命令执行，它的任何输出都将发

给管道的输入端。
        
	现在来研究一下管道的接收端。从代码中可以看出，管道的接收端是由父进程来担当的。

首先关闭stdin描述符，因为我们不会从机器的键盘等标准设备文件来接收数据的输入，而是从其它

程序的输出中接收数据。然后，再一次用到dup2函数，让stdin变成管道的输出端，这是通过让文件

描述符0（即常规的stdin）等于pfds[0]来实现的。关闭管道的stdout端（pfds[1]），因为在这里

用不到它。最后，使用 execlp函数把父进程的映像替换为命令wc -1的进程映像，命令wc -1把管道

的内容作为它的输入。*/
