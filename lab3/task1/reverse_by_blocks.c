#include <stdio.h>
#include <time.h>

void reverse_buff(char buff[], int size){
    char tmp;
    for(int i = 0; i < size / 2; i++){
        tmp = buff[i];
        buff[i] = buff[size - 1 - i];
        buff[size - 1 - i] = tmp;
    }
}

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
        printf("Blad otwarcia plikow");
        return 0;
    }

    char buff[1024];
    int size;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    fseek(file, 0, SEEK_END);
    while (ftell(file) >= 1024){
        fseek(file, -1024, SEEK_CUR);
        size = fread(buff, 1, sizeof(buff), file);
        reverse_buff(buff, size);
        fwrite(buff, 1, sizeof(buff), reversed_file);
        fseek(file, -1024, SEEK_CUR);
    }

    size = ftell(file);
    if(size > 0){
        fseek(file, 0, SEEK_SET);
        fread(buff, 1, size, file);
        reverse_buff(buff, size);
        fwrite(buff, 1, size, reversed_file);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    printf("Czas odwrócenia pliku w blokach po 1024 znaki: %f sekund\n", (end.tv_nsec - start.tv_nsec) / 1000000000.0);


    fclose(file);
    fclose(reversed_file);

    return 0;
}
