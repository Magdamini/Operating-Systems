#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handle_signal(int sig_no){
    printf("Received confirmation signal number %d\n", sig_no);
}

int main(int argc, char const *argv[])
{
    if(argc < 3){
        printf("Wrong number of arguments\n<catcher's PID> <action>\n");
        return 0;
    }

    signal(SIGUSR1, handle_signal);

    int catcher_pid = atoi(argv[1]);
    int action = atoi(argv[2]);

    if (action < 1 || action > 3){
        printf("Unknown action\n");
        return 0;

    }


    union sigval sig_val;
    sig_val.sival_int = action;
    sigqueue(catcher_pid, SIGUSR1, sig_val);


    sigset_t waiting_mask;
    sigfillset(&waiting_mask);
    sigdelset(&waiting_mask, SIGUSR1);

    sigsuspend(&waiting_mask);


    return 0;
}
