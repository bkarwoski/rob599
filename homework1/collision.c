#include <stdio.h>
#include <math.h>
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
    
int main(void) {
    char tmp[100];
    poly_t poly1 = { 0 };
    poly_t poly2 = { 0 };
    FILE *fp = fopen("polygons.csv", "r");
    if (!fp) {
        fprintf(stderr, "polygons.csv is missing\n");
        return 1;
    }
    
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
    //printf("%f\n,", poly1.xPoints[2]);
    
    return 0;
}
