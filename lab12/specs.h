#ifndef SPECS_H
#define SPECS_H

#define MAX_CLIENTS 10
#define MSG_SIZE 256
#define CLIENT_NAME_SIZE 10
#define PING_INTERVAL 5

typedef enum MsgType{
    LIST,
    ALL,
    ONE,
    ALIVE,
    STOP,
    INIT
    

} MsgType;

typedef struct Message{
    MsgType msg_type;
    int client_id;
    int reciver_id;
    char txt_msg[MSG_SIZE];
    char curr_time[32];
} Message;

#endif // SPECS_H