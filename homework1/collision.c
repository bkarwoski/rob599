#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define M_PI 3.14159265358979323846
#define DEGTORAD (M_PI / 180)

typedef struct poly {
    int numPoints;
    double xPoints[16];
    double yPoints[16];
    double rot;
    double xShift;
    double yShift;

} poly_t;

void shift(poly_t *polyX) {
    float c = cos(polyX->rot);
    float s = sin(polyX->rot);
    double newX = 0;
    double newY = 0;
    for (int i = 0; i < polyX->numPoints; i++) {
        newX = polyX->xPoints[i] * c - polyX->yPoints[i] * s;
        newY = polyX->xPoints[i] * s + polyX->yPoints[i] * c;

        newX += polyX->xShift;
        newY += polyX->yShift;

        polyX->xPoints[i] = newX;
        polyX->yPoints[i] = newY;
    }
}

void intersects(double x1, double y1, double x2, double y2,
                double x3, double y3, double x4, double y4) {
    
    //  return false;
}

//bool contains(double x1, double y1) {
//return false;
//}

int main(void) {
    char tmp[100];
    poly_t poly1 = {0};
    poly_t poly2 = {0};
    FILE *fp = fopen("polygons.csv", "r");
    if (!fp) {
        fprintf(stderr, "polygons.csv is missing\n");
        return 1;
    }
    bool collides = false;

    fgets(tmp, 100, fp);

    fscanf(fp, "%lf", &poly1.xShift);
    fscanf(fp, "%lf", &poly1.yShift);
    fscanf(fp, "%lf", &poly1.rot);
    poly1.rot *= DEGTORAD;
    fscanf(fp, "%d", &poly1.numPoints);
    int pointCheck = 0;
    for (int i = 0; i < poly1.numPoints; i++) {
        pointCheck += fscanf(fp, "%lf", &poly1.xPoints[i]);
    }

    for (int i = 0; i < poly1.numPoints; i++) {
        pointCheck += fscanf(fp, "%lf", &poly1.yPoints[i]);
    }

    if (pointCheck != (2 * poly1.numPoints)) {
        fprintf(stderr, "a polygons.csv number is invalid\n");
        return 1;
    }

    fscanf(fp, "%lf", &poly2.xShift);
    fscanf(fp, "%lf", &poly2.yShift);
    fscanf(fp, "%lf", &poly2.rot);
    poly2.rot *= DEGTORAD;
    fscanf(fp, "%d", &poly2.numPoints);
    pointCheck = 0;
    for (int i = 0; i < poly2.numPoints; i++) {
        pointCheck += fscanf(fp, "%lf", &poly2.xPoints[i]);
    }

    for (int i = 0; i < poly2.numPoints; i++) {
        pointCheck += fscanf(fp, "%lf", &poly2.yPoints[i]);
    }

    if (pointCheck != (2 * poly1.numPoints)) {
        fprintf(stderr, "a polygons.csv number is invalid\n");
        return 1;
    }

    fclose(fp);

    if (poly1.numPoints > 16 || poly2.numPoints > 16) {
        printf("too many points!\n");
        return 1;
    }

    shift(&poly1);
    shift(&poly2);

    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double x3 = 0;
    double y3 = 0;
    double x4 = 0;
    double y4 = 0;

    for (int i = 0; i < poly1.numPoints; i++) {
        x1 = poly1.xPoints[i];
        y1 = poly1.yPoints[i];
        if (1+i == poly1.numPoints) {
            x2 = poly1.xPoints[0];
            y2 = poly1.yPoints[0];
        }
        else {
            x2 = poly1.xPoints[i+1];
            y2 = poly1.yPoints[i+1];
        }
        for (int j = 0; j < poly2.numPoints; j++) {
            x3 = poly2.xPoints[i];
            y3 = poly2.yPoints[i];
            if (1+j == poly2.numPoints) {
                x4 = poly2.xPoints[0];
                y4 = poly2.yPoints[0];
            }
            else {
                x4 = poly2.xPoints[j+1];
                y4 = poly2.yPoints[j+1];
            }
            if (intersects(x1, y1, x2, y2, x3, y3, x4, y4)) {
                collides = true;
                break;

            }

        }
    }
    //printf("%f\n,", poly1.xPoints[2]);

    if (collides) {
        printf("collision!\n");
    } else {
        printf("no collision\n");
    }

    return 0;
}
