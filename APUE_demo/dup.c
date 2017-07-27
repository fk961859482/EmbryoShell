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
