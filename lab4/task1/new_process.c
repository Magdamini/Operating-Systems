#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if(argc < 2){
        printf("Specify the number of new processes\n");
        return 0;
    }

    pid_t child_pid;
    int n = atoi(argv[1]);

    for(int i = 0; i < n; i++){
        child_pid = fork();
        if(child_pid == 0){
            printf("PID: %d, PPID: %d\n", (int)getpid(), (int)getppid());
            return 0;
        }
    }


    while (wait(-1) != -1);

    printf("Numer of new processes: %d\n", n);
    return 0;
}
