#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

int int_compare(const void *a, const void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return val_a - val_b;
}

int main(int argc, char **argv) {
    int target = atoi(argv[1]);
    int n = 1000;
    int *randArray = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        randArray[i] = rand() % 1000;
    }
    qsort(randArray, n, sizeof(int), int_compare);

    //printf("test rand: %d\n", randArray[5]);
    int left = 0;
    int right = n;
    int mid_i = 0;
    while (left < right) {
        mid_i = (left + right) / 2;
        printf("mid_i: %d\n", mid_i);
        if (randArray[mid_i] == target) {
            printf("value = %d\n", randArray[mid_i]);
            printf("%d\n", mid_i);
            break;
        }
        if (randArray[mid_i] < target) {
            left = mid_i + 1;
        } else {
            right = mid_i;
        }
    }
    free(randArray);
    return 0;
}
