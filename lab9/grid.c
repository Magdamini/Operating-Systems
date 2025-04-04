#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "grid.h"

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}
//////////////////////////////////////////////

void ignore_handler(){}

void* update_part_of_grid(void *args){
    ThreadData *thread_data = (ThreadData*) args;
    int row, col, start, end, n_elem, grid_cells = grid_height * grid_width;
    char *tmp;

    // find cells to update
    n_elem = ceil(grid_cells / thread_data->no_threads);
    start = thread_data->n * n_elem;
    end = start + n_elem;
    if (end > grid_cells) end = grid_cells;


    signal(SIGUSR1, ignore_handler);

    while (1){
        
        pause();

        // update cells
        for(int i = start; i < end; i++){
            row = (int)(i / grid_width);
            col = i % grid_height;
            thread_data->dst[i] = is_alive(row, col, thread_data->src);
        }

        // swap grids for next step
        tmp = thread_data->src;
        thread_data->src = thread_data->dst;
        thread_data->dst = tmp;
    }
}