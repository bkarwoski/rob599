#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define DEGTORAD M_PI / 180

typedef struct poly {

    int numPoints;
    double xPoints[16];
    double yPoints[16];
    double rot;
    double xShift;
    double yShift;

} poly_t;

int main(){

char tmp[100];
poly_t poly1 = { 0 };
poly_t poly2 = { 0 };
FILE *fp = fopen("polygons.csv", "r");
if(fp == NULL) {
    printf("file does not exist\n");
    return 1;
}
fgets(tmp, 100, fp); //clear first line

fscanf(fp, "%lf", &poly1.xShift);
fscanf(fp, "%lf", &poly1.yShift);
fscanf(fp, "%lf", &poly1.rot);
poly1.rot *= DEGTORAD;
fscanf(fp, "%d", &poly1.numPoints);
for(int i = 0; i < poly1.numPoints; i++) {
    fscanf(fp, "%lf", &poly1.xPoints[i]);
}
for(int i = 0; i < poly1.numPoints; i++) {
    fscanf(fp, "%lf", &poly1.yPoints[i]);
}

fscanf(fp, "%lf", &poly2.xShift);
fscanf(fp, "%lf", &poly2.yShift);
fscanf(fp, "%lf", &poly2.rot);
poly2.rot *= DEGTORAD;
fscanf(fp, "%d", &poly2.numPoints);
for(int i = 0; i < poly2.numPoints; i++) {
    fscanf(fp, "%lf", &poly2.xPoints[i]);
}
for(int i = 0; i < poly2.numPoints; i++) {
    fscanf(fp, "%lf", &poly2.yPoints[i]);
}

//printf("%f\n,", poly2.yPoints[2]);

fclose(fp);
return 0;

}
