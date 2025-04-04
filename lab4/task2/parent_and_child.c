#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int global;

int main(int argc, char const *argv[])
{
    if(argc < 2){
        printf("No directory specified\n");
        return 0;
    }

    printf("Program name: %s\n", argv[0]);

    
    pid_t child_pid;
    int local = 0, child_status, parent_status, status;

    child_pid = fork();

    if(child_pid == 0){
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n",  (int)getpid(), (int)getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        child_status = execl("/bin/ls", "ls", argv[1], NULL);

        exit(child_status);

    } else {
        wait(&status);
        parent_status = WEXITSTATUS(status);
        printf("\nparent process\n");
        printf("parent pid = %d, child pid = %d\n",  (int)getpid(), (int)child_pid);
        printf("Child exit code: %d\n", parent_status);
        printf("Parent's local = %d, parent's global = %d\n", local, global);

        return parent_status;
    }


    return 0;
}
