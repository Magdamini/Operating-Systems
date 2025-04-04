#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_REEINDERS 9
#define MAX_DELIVERIES 4

int reeinders_at_home = 0;
pthread_mutex_t lock_reeinders;
pthread_cond_t c_santa_sleep;
pthread_cond_t c_giving_presents;

void* reeinder(void *args){
    int *id = (int*) args;
    srand(*id);
    while (1)
    {
        // spend holidays
        sleep(rand() % 6 + 5);
        
        // come back to santa
        pthread_mutex_lock(&lock_reeinders);
        reeinders_at_home++;
        printf("RENIFER %d: Czeka %d reniferów na Mikołaja\n", *id, reeinders_at_home);
        if (reeinders_at_home == 9){
            // wake up santa
            printf("RENIFER %d: Wybudzam Mikołaja\n", *id);
            pthread_cond_broadcast(&c_santa_sleep);
        }
        pthread_mutex_unlock(&lock_reeinders);

        // give presents
        pthread_cond_wait(&c_giving_presents, &lock_reeinders);
        pthread_mutex_unlock(&lock_reeinders);

        // holidays

    }
    


}

void* santa_claus(void *args){
    int no_deliveries = 0;
    srand(time(NULL));

    while(no_deliveries < MAX_DELIVERIES){

        // sleep
        pthread_cond_wait(&c_santa_sleep, &lock_reeinders);  // blokuje mutex
        printf("MIKOŁAJ: Budzę się\n");

        // give presents
        no_deliveries++;
        printf("MIKOŁAJ: Dostarczam zabawki\n");
        sleep(rand() % 3 + 2);
        reeinders_at_home = 0;

        pthread_cond_broadcast(&c_giving_presents);
        pthread_mutex_unlock(&lock_reeinders);

        // sleep
        printf("MIKOŁAJ: Zasypiam\n");
    }

    return NULL;

}

int main(int argc, char const *argv[])
{
    // set up mutex
    pthread_mutex_init(&lock_reeinders, NULL);

    // create threads for reeinders
    pthread_t threads[NO_REEINDERS];
    int id[NO_REEINDERS];
    for(int i = 0; i < NO_REEINDERS; i++){
        id[i] = i;
        pthread_create(&threads[i], NULL, reeinder, id + i);
    }

    // create thread for santa
    pthread_t santa;
    pthread_create(&santa, NULL, santa_claus, NULL);

    pthread_join(santa, NULL);
    for(int i = 0; i < NO_REEINDERS; i++){
        pthread_cancel(threads[i]);
    }

    
    return 0;
}
