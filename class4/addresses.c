#include <stdio.h>

char char1;
int int1;


int main(void){

   // int variable = 10;
   // printf("%p\n", (void *)&variable);
   // return 0;
    printf("%p\n", (void *)&char1);
    printf("%p\n", (void *)&int1);
    printf("%p\n", (void *)&char1-(void *)&int1);
return 0;
}
