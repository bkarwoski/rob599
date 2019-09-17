#include <stdio.h>

char char1;
int int1;
double doubles[3];

int main(void) {
    char char2;
    int int2;
    float floats[3];
   // int variable = 10;
   // printf("%p\n", (void *)&variable);
   // return 0;
    printf("%p\n", (void *)&char1);
    printf("%p\n", (void *)&int1);
    printf("%ld\n", (long)&char1 - (long)&int1);
    printf("%p\n", (void *)&doubles);
    printf("%p\n", (void *)&doubles[0]);
    printf("%p\n", (void *)&doubles[1]);
    printf("%p\n", (void *)&doubles[2]);
    printf("%p\n", doubles[1] - doubles[0]);
    printf("%ld\n", (long)doubles[1] - (long)doubles[0]);

    printf("%p\n", (void *)&char2);
    printf("%p\n", (void *)&int2);
    printf("%p\n", (void *)&char2 - (void *)&int2);
    printf("%p\n", (void *)&floats);
    printf("%p\n", (void *)&floats[0]);
    printf("%p\n", (void *)&floats[1]);
    printf("%p\n", (void *)&floats[2]);
return 0;
}
