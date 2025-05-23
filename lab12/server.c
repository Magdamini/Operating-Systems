#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <poll.h>
#include <unistd.h>
#include <time.h>

#include "specs.h"



typedef struct client{
    int id; // -1 if no client registered
    char client_name[CLIENT_NAME_SIZE];
    time_t last_msg;
    struct sockaddr_in address;
} client;

client clients[MAX_CLIENTS];
int server_socket_fd;
pthread_t ping_thread;


void signal_handler(int signal_no){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].id != -1){
            Message stop_msg;
            stop_msg.msg_type = STOP;

            sendto(server_socket_fd, &stop_msg, sizeof(stop_msg), 0,
            (const struct sockaddr *) &clients[i].address, sizeof(clients[i].address));
        }
    }
    pthread_cancel(ping_thread);
    close(server_socket_fd);
    exit(0);
}

void *ping_clients(void *args){
    Message ping_msg;
    ping_msg.msg_type = ALIVE;
    while (1)
    {
        sleep(PING_INTERVAL);

        // look for unactive clients
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(clients[i].id < 0) continue;
            if(time(NULL) - clients[i].last_msg > 2 * PING_INTERVAL){
                clients[i].id = -1;
                printf("Unregister client %d\n", i);
                fflush(stdout);
            }
        }

        // ping clients
        printf("Pinging clients\n");
        fflush(stdout);
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(clients[i].id < 0) continue;
            sendto(server_socket_fd, &ping_msg, sizeof(ping_msg), 0,
            (const struct sockaddr *) &clients[i].address, sizeof(clients[i].address));
        }
    }
    
}

int main(int argc, char const *argv[])
{
    if(argc < 2){
        printf("Too less arguments: <server address>\n");
        return -1;
    }
    int server_port = atoi(argv[1]);

    // handle keyboard exit signal
    signal(SIGINT, signal_handler);

    // set empty spots for clients
    for(int i = 0; i < MAX_CLIENTS; i++){
        clients[i].id = -1;
    }

    // create server socket
    server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket_fd < 0){
        printf("Failed to create server network socket\n");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;            // use IPv4
    server_address.sin_port = server_port;          // set port number
    server_address.sin_addr.s_addr = INADDR_ANY;    // accept connections send on any computer IP

    bind(server_socket_fd,(struct sockaddr *) &server_address, sizeof(server_address));

    pthread_create(&ping_thread, NULL, ping_clients, NULL);

    // receive messages from clients
    Message msg, msg_to_send;
    time_t t;
    struct sockaddr_in client_address;
    socklen_t len; 
    while (1){

        recvfrom(server_socket_fd, &msg, sizeof(Message), 0, ( struct sockaddr *) &client_address, &len);

        switch (msg.msg_type){
            case INIT:{
                // register client
                // find client id
                int new_client_id = -1;
                for(int i = 0; i < MAX_CLIENTS; i++){
                    if(clients[i].id == -1){
                        new_client_id = i;
                        break;
                    }
                }
                if(new_client_id == -1){
                    printf("Maximum number of clients reached\n");
                    continue;
                }

                // add client to server structures
                clients[new_client_id].id = new_client_id;
                strcpy(clients[new_client_id].client_name, msg.txt_msg);
                clients[new_client_id].last_msg = time(NULL);
                clients[new_client_id].address = client_address;

                // send info back to the client
                msg_to_send.client_id = new_client_id;
                if (sendto(server_socket_fd, &msg_to_send, sizeof(msg_to_send), 0,
                (const struct sockaddr *) &clients[new_client_id].address, sizeof(client_address)) < 0){
                    printf("Send error\n");
                    clients[new_client_id].id = -1;
                } else{
                    printf("Client %d succesfully registered\n", new_client_id);
                }
                break;

            }
            case LIST: {
                char client_buff[2 * CLIENT_NAME_SIZE];
                char buff[MSG_SIZE] = "";

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].id > -1) {
                        sprintf(client_buff, "ID: %d, %s; ", j, clients[j].client_name);
                        strcat(buff, client_buff);
                    }
                }
                msg_to_send.msg_type = LIST;
                strcpy(msg_to_send.txt_msg, buff);
                sendto(server_socket_fd, &msg_to_send, sizeof(msg_to_send), 0,
                (const struct sockaddr *) &clients[msg.client_id].address, sizeof(clients[msg.client_id].address));

                break;
            }
            case ALL: {
                t = time(NULL);
                msg_to_send.msg_type = ONE;
                strcpy(msg_to_send.txt_msg, msg.txt_msg);
                msg_to_send.client_id = msg.client_id;
                sprintf(msg_to_send.curr_time, "%s", ctime(&t));
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].id > -1 && clients[j].id != msg.client_id) {
                        sendto(server_socket_fd, &msg_to_send, sizeof(msg_to_send), 0,
                        (const struct sockaddr *) &clients[j].address, sizeof(clients[j].address));
                        
                    }
                }
                break;
            }
            case ONE: {
                t = time(NULL);
                msg_to_send.msg_type = ONE;
                strcpy(msg_to_send.txt_msg, msg.txt_msg);
                msg_to_send.client_id = msg.client_id;
                sprintf(msg_to_send.curr_time, "%s", ctime(&t));
                sendto(server_socket_fd, &msg_to_send, sizeof(msg_to_send), 0,
                (const struct sockaddr *) &clients[msg.reciver_id].address, sizeof(clients[msg.reciver_id].address));
                break;
            }
            case STOP:
                clients[msg.client_id].id = -1;
                break;
            case ALIVE:
                clients[msg.client_id].last_msg = time(NULL);
                break;

        }
        
    }







    return 0;



}

 