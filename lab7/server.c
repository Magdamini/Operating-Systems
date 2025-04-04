#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "specs.h"
 

int main()
{
    mq_unlink(SERVER_NAME);
    struct mq_attr queue_attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(Message),
        .mq_curmsgs = 0
    };

    // o_creat <- zosanie utworzona jeśli nie istnieje
    mqd_t queue_desc = mq_open(SERVER_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &queue_attr);
    if (queue_desc < 0){
        printf("Failed to open the queue\n");
        return -1;
    }

    Message msg;
    mqd_t client_queues[MAX_CLIENTS];
    int curr_id = 0;

    while (1)
    {
        mq_receive(queue_desc, (char*)&msg, sizeof(msg), NULL);
        
        if(msg.msg_type == INIT){
            // msg.txt - queue_name
            printf("init");
            client_queues[curr_id] = mq_open(msg.txt_msg, O_RDWR, S_IRUSR | S_IWUSR, NULL);
            if (client_queues[curr_id] < 0){
                printf("Failed to register client\n");
                break;
            } 

            Message msg_to_send = {
                .client_id = curr_id,
                .msg_type = INIT
            };
            
            mq_send(client_queues[curr_id], (char*)&msg_to_send, sizeof(msg_to_send), 1);
            curr_id++;
        } else if (msg.msg_type == MESSAGE)     
        {
            for(int i = 0; i < curr_id; i++){
                if(i == msg.client_id) continue;
                mq_send(client_queues[i], (char*)&msg, sizeof(msg), 1);
            }
        }
        

    }


    return 0;
}
