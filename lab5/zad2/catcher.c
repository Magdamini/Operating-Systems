#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int changes = 0, curr_mode = -1;

void handle_signal(int sig_no, siginfo_t *sig_info, void *p){
    pid_t sender_pid = sig_info->si_pid;
    int action = sig_info->si_value.sival_int;

    changes++;
    curr_mode = action;

    kill(sender_pid, SIGUSR1);

}

int main(int argc, char const *argv[])
{
    printf("PID: %d\n", (int)getpid());
    

    struct sigaction act;
    sigset_t sig_mask;

    sigemptyset(&sig_mask);
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    act.sa_mask = sig_mask;

    sigaction(SIGUSR1, &act, NULL);

    while (1){
        switch (curr_mode){
        case 1:
            for(int i = 1; i <= 100; i++){
                printf("%d\n", i);
            }
            curr_mode = -1;
            break;
        case 2:
            printf("Changes of operating mode: %d\n", changes);
            curr_mode = -1;
            break;
        case 3:
            printf("EXIT\n");
            return 0;
        default:
            break;
        }
    }

    return 0;
}
