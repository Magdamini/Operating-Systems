#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>


int main(void)
{
    int cnt = 0;
    long long sum_size = 0;
    DIR* dir = opendir(".");
    struct stat buff_stat;
    struct dirent* file_in_dir;

    while (1){
        file_in_dir = readdir(dir);
        if(file_in_dir == NULL) break;
        
        stat(file_in_dir->d_name, &buff_stat);
        
        if(!S_ISDIR(buff_stat.st_mode)){
            cnt++;
            printf("%d. %s, size: %ld bytes\n", cnt, file_in_dir->d_name, buff_stat.st_size);
            sum_size += buff_stat.st_size;
        }
    }

    printf("The total size of all files in the current directory is %lld bytes.\n", sum_size);

    closedir(dir);


    return 0;
}
