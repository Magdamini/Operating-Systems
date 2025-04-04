#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include "grid.h"

int run = 1;

void sig_handler(){
    run = 0;
}


int main(int argc, char const *argv[])
{

	if(argc < 2){
		printf("Specify number of threads\n");
		return -1;
	}

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;



	init_grid(foreground);

	

	// threads
	int no_threads = atoi(argv[1]);
	pthread_t threads[no_threads];
	ThreadData thread_data[no_threads];
	
	for(int i = 0; i < no_threads; i++){
        thread_data[i].src = foreground;
        thread_data[i].dst = background;
        thread_data[i].n = i;
        thread_data[i].no_threads = no_threads;
		pthread_create(&threads[i], NULL, update_part_of_grid, &thread_data[i]);
	}

	// handle keyboard exit signal
    signal(SIGINT, sig_handler);


	while (run)
	{
		draw_grid(foreground);

		// send signal to threads
		for(int i = 0; i < no_threads; i++){
			pthread_kill(threads[i], SIGUSR1);
		}

		usleep(500 * 1000);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode

	// end threads
	for(int i = 0; i < no_threads; i++){
		pthread_cancel(threads[i]);
	}

	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
