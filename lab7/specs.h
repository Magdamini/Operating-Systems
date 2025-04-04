#ifndef SPECS_H
#define SPECS_H

#define SERVER_NAME "/server_queue"
#define CLIENT_NAME "/client_queue"
#define MAX_CLIENTS 10
#define MSG_SIZE 256
#define MAX_MSG 10

typedef enum MsgType{
    INIT,
    MESSAGE

} MsgType;

typedef struct Message{
    MsgType msg_type;
    int client_id;
    char txt_msg[MSG_SIZE];
} Message;

#endif // SPECS_H