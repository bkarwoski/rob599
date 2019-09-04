#include <stdio.h>
#include <math.h>

int main(void) {
    printf("Enter a number to factorize:\n");
    int number = 0;
    int vals_read = scanf("%d", &number);
    if (vals_read != 1) {
        fprintf(stderr, "Invalid input. Please enter a number.\n");
        return 1;
    }
    printf("Prime factors:\n");

int temp = 2;
    while(temp <= sqrt(number)){ //update to sqrt
        if(number % temp == 0){
            printf("%d\n", temp);
            number = number / temp;
        }
        else{
            temp = temp + 1;
        }
    }
    printf("%d\n", number);


    return 0;
}
