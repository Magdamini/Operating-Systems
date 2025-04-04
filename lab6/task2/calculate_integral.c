#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


char *PIPE_OUT = "/tmp/pipe_out";
char *PIPE_IN = "/tmp/pipe_in";

double calculate_rect(double start, double end){
    double m = (start + end) / 2;
    return (end - start) * 4 / (m * m + 1);
}

double calculate_integral(double a, double b, double width){

    double sum = 0, start = a, end;

    while (start < b){
        end = start + width;
        if (end > b) end = b;
        sum += calculate_rect(start, end);
        start = end;
    }
    return sum;
}

void delete_pipes(){
    unlink(PIPE_IN);
    unlink(PIPE_OUT);
}


int main(int argc, char const *argv[])
{

    delete_pipes();

    if(mkfifo(PIPE_OUT, S_IRWXU) == -1){
        printf("Failed to create output pipe\n");
        return -1;
    }
    if(mkfifo(PIPE_IN, S_IRWXU) == -1){
        printf("Failed to create input pipe\n");
        return -1;
    }


    int input_fd = open(PIPE_IN, O_RDONLY);
    int output_fd = open(PIPE_OUT, O_WRONLY);

    if (output_fd < 0 || input_fd < 0){
        printf("Failed to open pipes\n");
        return -1;
    }

    double a, b, width, res;

    if (read(input_fd, &a, sizeof(a)) < 0 || read(input_fd, &b, sizeof(b)) < 0 || read(input_fd, &width, sizeof(width)) < 0){

        printf("Failed to read data\n");
        return -1;
        }

    res = calculate_integral(a, b, width);

    if(write(output_fd, &res, sizeof(res)) < 0){
        printf("Failed to write result\n");
        return -1;
    }

    close(input_fd);
    close(output_fd);

    return 0;
}
