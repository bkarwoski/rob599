#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int fib (int index) {
    if (index == 0) {
        return 0;
    }
    else if (index == 1) {
        return 1;
    } else {
   // printf("recurring\n");
    return fib(index - 1) + fib (index - 2);
    }
}

int main(int argc, char *argv[]) {
    clock_t start = clock();
    // compute something difficult
    int index = atoi(argv[1]);
    int ans = fib(index);
    printf("%d\n", ans);

    double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    printf("took %.2f seconds\n", elapsed);
    return 0;
}
