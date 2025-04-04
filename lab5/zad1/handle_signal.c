#include <stdio.h>
#include <string.h>
#include <signal.h>


void handle_sig(int sig_no){
    printf("Received signal number %d\n", sig_no);
}

int main(int argc, char const *argv[])
{
    if(argc < 2){
        printf("No action specified\n");
        return 0;
    }

    if(strcmp(argv[1], "ignore") == 0){
        signal(SIGUSR1, SIG_IGN);
    }
    else if(strcmp(argv[1], "handler") == 0){
        signal(SIGUSR1, handle_sig);
    }
    else if(strcmp(argv[1], "mask") == 0){
        sigset_t new_mask;
        sigemptyset(&new_mask);
        sigaddset(&new_mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &new_mask, NULL);

    }
    else if (strcmp(argv[1], "none") != 0){
        printf("Unknown action\n");
        return 0;
    }

    raise(SIGUSR1);

    if(strcmp(argv[1], "mask") == 0){
        sigset_t wait_sigset;
        sigpending(&wait_sigset);
            if(sigismember(&wait_sigset, SIGUSR1)){
                printf("Signal number %d is pending\n", SIGUSR1);
            } else {
                printf("No signal number %d pending", SIGUSR1);
            }

    }
    return 0;
}
