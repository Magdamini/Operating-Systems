int collatz_conjecture(int input){
    if(input % 2 == 0){
        return input / 2;
    }
    return 3 * input + 1;
}

int test_collatz_convergence(int input, int max_iter){
    int cnt = 0;
    while (input != 1){
        if(cnt == max_iter){
            return -1;
        }
        input = collatz_conjecture(input);
        cnt++;
    }
    return cnt;
}