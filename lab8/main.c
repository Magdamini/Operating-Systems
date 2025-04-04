#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


#include "printing_system_specs.h"

void rand_txt(char *txt){
    for(int i = 0; i < TXT_LEN; i++){
        txt[i] = rand() % 26 + 'a';
    }
    txt[TXT_LEN] = '\0';
}

void copy_txt(char *source, char *destination){
    for(int i = 0; i < TXT_LEN; i++){
        destination[i] = source[i];
    }
}

void queue_push(int *q_tail, char* queue, char* txt){
    copy_txt(txt, &queue[*q_tail * TXT_LEN]);
    *q_tail = (*q_tail + 1) % (QUEUE_SIZE);
}

void queue_pop(int *q_head, char* queue, char* txt){
    copy_txt(&queue[*q_head * TXT_LEN], txt);
    *q_head = (*q_head + 1) % QUEUE_SIZE;
}




int main(int argc, char const *argv[])
{
    if(argc < 3){
        printf("To less arguments! <Printers> <Users>\n");
        return -1;
    }

    int no_printers = atoi(argv[1]), no_users = atoi(argv[2]);
    char sem_name[TXT_LEN * 2];



    // shared memory
    int fd = shm_open("shared_memory", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0){
        printf("Fail in shm_open");
        return -1;
    }

    if(ftruncate(fd, sizeof(SharedMemory)) < 0){
        printf("Fail in ftruncate");
            return -1;
    }

    SharedMemory* shared_memory = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(shared_memory < 0){
        printf("Fail in mmap");
        return -1;
    }


    sem_unlink("queue_sem");
    shared_memory->queue_sem = sem_open("queue_sem",  O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, QUEUE_SIZE - 1);

    // printers
    for(int i = 0; i < no_printers; i++){
        sprintf(sem_name, "printer%d", i);
        
        sem_unlink(sem_name);
        shared_memory->printers[i].printer_sem = sem_open(sem_name,  O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 0);
        pid_t pid = fork();
        if(pid == 0){
            int val;
            shared_memory->printers[i].printing = 0;
            sem_wait(shared_memory->printers[i].printer_sem);
            while(1){
                printf("\nPrinter %d: printing...\n", i);
                fflush(stdout);
                
                for(int j = 0; j < TXT_LEN; j++){
                    printf("%c", shared_memory->printers[i].txt[j]);
                    fflush(stdout);
                    sleep(1);
                }

                
                // checking if sth is in the queue
                sem_getvalue(shared_memory->queue_sem, &val);
                if(val != QUEUE_SIZE - 1){
                    queue_pop(&shared_memory->q_head, shared_memory->queue, shared_memory->printers[i].txt);
                    sem_post(shared_memory->queue_sem);
                } else {
                    shared_memory->printers[i].printing = 0;
                    sem_wait(shared_memory->printers[i].printer_sem);
                }
            }
            sem_close(shared_memory->printers[i].printer_sem);
            sem_unlink(sem_name);
        }

    }


    // users
    for(int i = 0; i < no_users; i++){
        pid_t pid = fork();
        if(pid == 0){
            srand((int)getpid());
            int printer;
            char txt_buff[TXT_LEN + 1];
            while(1){
                rand_txt(txt_buff);
                printf("\nClient %d: new text to print: %s\n", i, txt_buff);
                fflush(stdout);

                // checking if there is available printer
                printer = -1;
                for(int j = 0; j < no_printers; j++){
                    if(shared_memory->printers[j].printing == 0){
                        printer = j;
                        break;
                    }
                }
                
                if(printer != -1){
                    shared_memory->printers[printer].printing = 1;
                    copy_txt(txt_buff, shared_memory->printers[printer].txt);
                    sem_post(shared_memory->printers[printer].printer_sem);
                } else{
                    // put text in the queue
                    sem_wait(shared_memory->queue_sem);
                    queue_push(&shared_memory->q_tail, shared_memory->queue, txt_buff);
                }
                sleep(rand() % 10 + 5);
            }
            

        }


    }


    while (wait(NULL) > 0) {}

    sem_close(shared_memory->queue_sem);
    sem_unlink("queue_sem");


    munmap(shared_memory, sizeof(shared_memory));
    shm_unlink("shared_memory");
    


    return 0;
}
