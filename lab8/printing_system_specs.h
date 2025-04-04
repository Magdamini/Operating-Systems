#ifndef SPECS_H
#define SPECS_H

#include <semaphore.h>

#define QUEUE_SIZE 10
#define TXT_LEN 10
#define MAX_PRINTERS 256

typedef struct Printer{
    sem_t *printer_sem;
    char txt[TXT_LEN];
    int printing;
} Printer;

typedef struct SharedMemory{
    Printer printers[MAX_PRINTERS];
    char queue[QUEUE_SIZE * TXT_LEN];
    int q_tail;
    int q_head;
    sem_t *queue_sem;
} SharedMemory;

#endif // SPECS_H