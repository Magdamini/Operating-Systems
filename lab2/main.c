#include<stdio.h>
#include<stdlib.h>

#ifdef DYNAMIC
    #include <dlfcn.h>
#else
    #include "collatz.h"
#endif


int main(void)
{
    #ifdef DYNAMIC
    void *p = dlopen("./libcollatz.so", RTLD_LAZY);
    if(!p){
        printf("Opening library error");
        return 0;
    }

    int (*test_collatz_convergence)(int, int);
    test_collatz_convergence = dlsym(p, "test_collatz_convergence");
    if (dlerror() != 0){
        printf("Test_collatz_convergence error");
        return 0;
    }
    #endif

    int n = 5, a, iter, max_iter = 50;
    for(int i = 0; i < n; i++){
        a = rand() % 100 + 1;
        iter = test_collatz_convergence(a, max_iter);
        if(iter == -1){
            printf("Liczba iteracji dla liczby %d jest większa od %d\n", a, max_iter);
        } else{
            printf("Liczba iteracji dla liczby %d wynosi %d\n", a, iter);
        }
    }

    #ifdef DYNAMIC
    dlclose(p);
    #endif
    return 0;
}