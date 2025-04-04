#include <stdio.h>
#include <time.h>

int main(int argc, char** argv)
{   
    FILE *file = NULL, *reversed_file = NULL;

    if (argc != 3){
        printf("Niepoprawna liczba argumentow");
        return 0;
    }

    file = fopen(argv[1], "r");
    reversed_file = fopen(argv[2], "w");

    
    if(file == NULL || reversed_file == NULL){
        printf("Blad otwarci plikow");
        return 0;
    }

    char c;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    
    if(fseek(file, -1, SEEK_END) != EOF){
        while (1){
            fread(&c, 1, 1, file);
            fwrite(&c, 1, 1, reversed_file);
            if(fseek(file, -2, SEEK_CUR) == EOF) break;
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Czas odwrócenia pliku bajt po bajcie: %f sekund\n", (end.tv_nsec - start.tv_nsec) / 1000000000.0);


    fclose(file);
    fclose(reversed_file);

    return 0;
}
