#include <stdio.h>

char char1;
int int1;
double doubles[3];

int main(void){
    char char2;
    int int2;
    float floats[3];
   // int variable = 10;
   // printf("%p\n", (void *)&variable);
   // return 0;
    printf("%li\n", (void *)&char1);
    printf("%li\n", (void *)&int1);
    printf("%li\n", (void *)&char1-(void *)&int1);
    printf("%li\n", (void *)&doubles);
    printf("%li\n", (void *)&doubles[0]);
    printf("%li\n", (void *)&doubles[1]);
    printf("%li\n", (void *)&doubles[2]);

    printf("%li\n", (void *)&char2);
    printf("%li\n", (void *)&int2);
    printf("%li\n", (void *)&char2-(void *)&int2);
    printf("%li\n", (void *)&floats);
    printf("%li\n", (void *)&floats[0]);
    printf("%li\n", (void *)&floats[1]);
    printf("%li\n", (void *)&floats[2]);
return 0;
}
