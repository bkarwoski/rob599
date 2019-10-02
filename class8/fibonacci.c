#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

int fib1 (int index) {
    if (index == 0) {
        return 0;
    }
    else if (index == 1) {
        return 1;
    } else {
    return fib1(index - 1) + fib1(index - 2);
    }
}

uint64_t fib2 (uint64_t index) {
    if (index == 0) {
        return 0;
    }
    if (index == 1) {
        return 1;
    }
    uint64_t prevPrev = 0;
    uint64_t prev = 1;
    uint64_t result = 0;
        
    for (uint64_t i = 2; i <= index; i++)
    {
        result = prev + prevPrev;
        prevPrev = prev;
        prev = result;
    }
    return result;
}

uint64_t fib3 (uint64_t index, uint64_t *fibVals) {
    if (index == 0) {
        return 0;
    }
    else if (index == 1) {
        return 1;
    }
    fibVals[1] = 1;
    fibVals[2] = 1;
    if (fibVals[index]) {
        //printf("fibVals[%ld] already in index. value is %d\n", index, fibVals[index]);
        return fibVals[index];
    } else {
    fibVals[index] = fib3(index - 1, fibVals) + fib3(index - 2, fibVals);
    return fibVals[index];
    }
    
}

int main(void) {
    //uint64_t index = atoi(argv[1]);
    int fib1index = 38;
    int fib2index = 90;
    int fib3index = 90;
    clock_t start = clock();
    int ans1 = fib1(fib1index);
    double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    printf("fib1(38) got %d and took %.6f ms per iteration\n", ans1, elapsed);
    /*uint64_t fibVals [fib3index +1];
    for (int i = 0; i < fib3index + 1; i++) {
        fibVals[i] = 0;
    }
    uint64_t ans = fib3(fib3index, fibVals);
    printf("%lu\n", ans);*/


    //printf("took %.2f seconds\n", elapsed);
   
    return 0;
}
