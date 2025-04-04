#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/times.h>

double calculate_rect(double start, double end){
    double m = (start + end) / 2;
    return (end - start) * 4 / (m * m + 1);
}

double calculate_integral(double a, double b, double width){
    double start = a, end, sum = 0;
    while (start < b){
        end = start + width;
        if (end > b) end = b;
        sum += calculate_rect(start, end);
        start = end;
    }
    return sum;
}


int main(int argc, char const *argv[])
{
    if(argc < 3){
        printf("<width> <numer of processes>");
        return -1;
    }

    double width = strtod(argv[1], NULL);
    if(width > 1 || width <= 0){
        printf("Width must be between 0 and 1");
        return -1;
    }

    // struct tms start, end;
    clock_t start_time = times(NULL);
    
    double a = 0, b = 1;
    int n = atoi(argv[2]);
    int intervals = ceil((b - a) / n);
    int single_proc_intervals = (int) (intervals / n);

    double start = a, end;
    int fd[n][2];
    for(int i = 0; i < n; i++){
        pipe(fd[i]);

        if(i != n - 1) end = start + single_proc_intervals * width;
        else end = b;

        pid_t pid = fork();
        if(pid == 0){
            close(fd[i][0]);
            double sum = calculate_integral(start, end, width);
            if (write(fd[i][1], &sum, sizeof(sum)) < 0){
                printf("Failed to write result\n");
                return -1;
            }
            exit(0);

        }

        close(fd[i][1]);
        start = end;
    }


    double res = 0, single_res;

    for(int i = 0; i < n; i++){
        if(read(fd[i][0], &single_res, sizeof(double)) < 0){
            printf("Failed to read result\n");
            return -1;
        }
        res += single_res;
    }

    clock_t end_time = times(NULL);
    clock_t time = end_time - start_time;

    printf("Calculated integral: %.8lf\n", res);
    printf("Liczba procesów: %d, precyzja: %.10lf, czas wykonania: %.2lf, wynik: %.8lf\n", n, width, (double)time / sysconf(_SC_CLK_TCK), res);

    return 0;
}
