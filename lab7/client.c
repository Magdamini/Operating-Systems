#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "specs.h"

int main()
{
    pid_t pid = getpid();
    char queue_name[MSG_SIZE], txt[MSG_SIZE];
    int client_id;
    sprintf(queue_name, "%s%d", CLIENT_NAME, (int)pid);
    struct mq_attr queue_attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(Message),
        .mq_curmsgs = 0
    };

    
    mqd_t server_queue = mq_open(SERVER_NAME, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if(server_queue < 0){
        printf("Failed to open the server queue\n");
        return -1;
    }

    mqd_t client_queue = mq_open(queue_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &queue_attr);
    if(client_queue < 0){
        printf("Failed to open the client queue\n");
        return -1;
    }

    Message init_msg = {
        .msg_type = INIT,
    };

    strcpy(init_msg.txt_msg, queue_name);


    Message msg;
    mq_send(server_queue, (char*)&init_msg, sizeof(init_msg), 1);
    mq_receive(client_queue, (char*)&msg, sizeof(msg), NULL);
    client_id = msg.client_id;
    printf("Client id: %d\n", client_id);


    pid_t child_pid = fork();
    if (child_pid == 0){
        while(1){
            mq_receive(client_queue, (char*)&msg, sizeof(msg), NULL);
            printf("Received message from client %d: %s\n", msg.client_id, msg.txt_msg);
        }
    }
    else{
        while (1)
        {
            scanf("%s", txt);
            Message msg_to_send = {
                .msg_type = MESSAGE,
                .client_id = client_id
            };
            strcpy(msg_to_send.txt_msg, txt);

            mq_send(server_queue, (char*)&msg_to_send, sizeof(init_msg), 1);
        }
    }
        
    

    return 0;
}
