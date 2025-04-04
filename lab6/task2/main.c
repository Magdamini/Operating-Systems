#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

char *PIPE_OUT = "/tmp/pipe_out";
char *PIPE_IN = "/tmp/pipe_in";


int main(int argc, char const *argv[])
{
    int input_fd = open(PIPE_IN, O_WRONLY);
    int output_fd = open(PIPE_OUT, O_RDONLY);

    if (output_fd < 0 || input_fd < 0){
        printf("Failed to opent pipes\n");
        return -1;
    }


    double a, b, width, res;

    printf("Start: ");
    scanf("%lf", &a);

    printf("End: ");
    scanf("%lf", &b);

    printf("Width: ");
    scanf("%lf", &width);

    if(width > (b - a) || width <= 0){
        printf("Incorrect width\n");
        return -1;
    }

    if(write(input_fd, &a, sizeof(a)) < 0){
        printf("Failed to write data\n");
        return -1;
    }
    if(write(input_fd, &b, sizeof(b)) < 0){
        printf("Failed to write data\n");
        return -1;
    }
    if(write(input_fd, &width, sizeof(width)) < 0){
        printf("Failed to write data\n");
        return -1;
    }


    if(read(output_fd, &res, sizeof(res)) < 0){
        printf("Failed to read result\n");
        return -1;
    }

    printf("Calculated integral: %.8lf\n", res);

    close(input_fd);
    close(output_fd);    


    return 0;
}
