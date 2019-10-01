#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

double y(double x) {
    return (2 * x - 20 * x * x + 20 * x * x * x - 4 * x * x * x * x);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: %s <low> <high>\n", argv[0]);
        return 1;
    }

    double x0 = atof(argv[1]);
    double x1 = atof(argv[2]);
    double x_mid = 0;
    double y_mid = 0;
    double threshold = 1e-6;

    while (x1 - x0 > threshold) {
        x_mid = (x0 + x1) / 2;
        printf("x_mid: %11.4e ", x_mid);
        y_mid = y(x_mid);
        printf("y_mid: %11.4e\n", y_mid);
        if (y_mid == 0) {
            break;
        }
        if (y_mid * y(x0) > 0) {
            x0 = x_mid;
        } else {
            x1 = x_mid;
        }
    }

    return 0;
}
