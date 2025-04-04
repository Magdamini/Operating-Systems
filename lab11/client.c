#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#include "specs.h"

int socket_fd;
int client_id;
int run = 1;
pthread_t receiver_thread;

void signal_handler(int signal_no){
    Message msg;
    msg.client_id = client_id;
    msg.msg_type = STOP;
    send(socket_fd, &msg, sizeof(Message), 0);
    pthread_cancel(receiver_thread);
    close(socket_fd);
    exit(0);
}

void *receive_messages(void *args){
    Message msg;
    while(1){
        int no_bytes = recv(socket_fd, &msg, sizeof(Message), 0);
        if(no_bytes > 0){
            switch (msg.msg_type)
            {
            case STOP:{
                close(socket_fd);
                exit(0);
            }
            case LIST:{
                printf("List of active clients:\n");
                printf("%s\n", msg.txt_msg);
                fflush(stdout);
                break;
            }
            case ONE:{
                printf("%s Received message from client %d: %s\n", msg.curr_time, msg.client_id, msg.txt_msg);
                fflush(stdout);
                break;
            }
            case ALIVE:{
                Message alive_msg;
                alive_msg.msg_type = ALIVE;
                alive_msg.client_id = client_id;
                send(socket_fd, &alive_msg, sizeof(Message), 0);

            }
            default:
                break;
            }
        }
    }
}

int set_msg_type(Message *msg, char *command){
    if (strcmp(command, "LIST") == 0){
        msg->msg_type = LIST;
        return 1;
    }
    if (strcmp(command, "2ALL") == 0){
        msg->msg_type = ALL;
        return 1;
    }
    if (strcmp(command, "2ONE") == 0){
        msg->msg_type = ONE;
        return 1;
    }
    if (strcmp(command, "STOP") == 0){
        msg->msg_type = STOP;
        return 1;
    }
    printf("Unknown command\n");
    return 0;
}


int main(int argc, char const *argv[])
{
    if(argc < 3){
        printf("To less arguments: <ID> <server address>");
        return -1;
    }
    char client_name[CLIENT_NAME_SIZE];
    strcpy(client_name, argv[1]);
    int port = atoi(argv[2]);

    // connect to server
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = port;
    address.sin_addr.s_addr = INADDR_ANY;

    connect(socket_fd, (struct sockaddr *) &address, sizeof(address));

    // send init msg
    Message init_msg;
    strcpy(init_msg.txt_msg, client_name);
    send(socket_fd, &init_msg, sizeof(Message), 0);

    recv(socket_fd, &init_msg, sizeof(Message), 0);
    client_id = init_msg.client_id;
    printf("Server client ID: %d\n", client_id);


    // handle keyboard exit signal
    signal(SIGINT, signal_handler);

    pthread_create(&receiver_thread, NULL, receive_messages, NULL);


    // send messages
    char command[32];

    Message msg;
    
    while(1){
        scanf("%s", command);
        msg.client_id = client_id;
        if(set_msg_type(&msg, command) == 0) continue;
        switch (msg.msg_type){
        case STOP:{
            send(socket_fd, &msg, sizeof(Message), 0);
            pthread_cancel(receiver_thread);
            close(socket_fd);
            exit(0);
        }
        case LIST:{
            send(socket_fd, &msg, sizeof(Message), 0);
            break;
        }
        case ALL:{
            scanf("%s", msg.txt_msg);
            send(socket_fd, &msg, sizeof(Message), 0);
            break;
        }
        case ONE:{
            scanf("%d", &msg.reciver_id);
            scanf("%s", msg.txt_msg);
            send(socket_fd, &msg, sizeof(Message), 0);
            break;
        }
        default:
            break;
        }


    }


    return 0;
}
