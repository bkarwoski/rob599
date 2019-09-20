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

/*bool isZero (double *vec) {
    if ((vec[0] == 0) && vec[1] == 0) {
        return true;
    } else
        { return false;
    }
}*/

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

bool intersects(double x1, double y1, double x2, double y2,
                double x3, double y3, double x4, double y4) {
    double vec1[2] = {x2 - x1, y2 - y1};
    double t11[2] = {x3 - x1, y3 - y1};
    double t12[2] = {x2 - x1, y4 - y3};
    double cv1t11 = vec1[0] * t11[1] - vec1[1] * t11[0];
    double cv1t12 = vec1[0] * t12[1] - vec1[1] * t12[0];
    bool neg1 = (cv1t11 * cv1t12 <= 0);

    double vec2[2] = {x4 - x3, y4 - y3};
    double t21[2] = {x1 - x3, y1 - y3};
    double t22[2] = {x2 - x3, y2 - y3};
    double cv2t21 = vec2[0] * t21[1] - vec2[1] * t21[0];
    double cv2t22 = vec2[0] * t22[1] - vec2[1] * t22[0];
    bool neg2 = (cv2t21 * cv2t22 <= 0);

    if (neg1 && neg2) {
        //return true;
    } else if (!((cv1t11 * cv1t12 == 0) && (cv2t21 * cv2t22 == 0)) &&
                   ((cv1t11 * cv1t12 == 0) || (cv2t21 * cv2t22 == 0))) {
        //return true;
    }
   return false;
}

bool contains(double x, double y, poly_t *polyX) {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    double cross = 0;
    double crossInit = 0;
    bool assigned = false;
    for (int i = 0; i < polyX->numPoints; i++) {
        x1 = polyX->xPoints[i];
        y1 = polyX->yPoints[i];
        if (i + 1 == polyX->numPoints) {
            x2 = polyX->xPoints[0];
            y2 = polyX->yPoints[0];
        } else {
            x2 = polyX->xPoints[i + 1];
            y2 = polyX->yPoints[i + 1];
        }
        double vec1[2] = {x2 - x1, y2 - y1};
        double t1[2] = {x - x1, y - y1};
        if (!assigned) {
            crossInit = vec1[0] * t1[1] - vec1[1] * t1[0];
            assigned = true;
        }
        cross = vec1[0] * t1[1] - vec1[1] * t1[0];
        if ((cross * crossInit) < 0) {
            return false;
        }
    }
    return true;
    //return false;
}

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
        if (1 + i == poly1.numPoints) {
            x2 = poly1.xPoints[0];
            y2 = poly1.yPoints[0];
        } else {
            x2 = poly1.xPoints[i + 1];
            y2 = poly1.yPoints[i + 1];
        }
        for (int j = 0; j < poly2.numPoints; j++) {
            x3 = poly2.xPoints[i];
            y3 = poly2.yPoints[i];
            if (1 + j == poly2.numPoints) {
                x4 = poly2.xPoints[0];
                y4 = poly2.yPoints[0];
            } else {
                x4 = poly2.xPoints[j + 1];
                y4 = poly2.yPoints[j + 1];
            }
            if (intersects(x1, y1, x2, y2, x3, y3, x4, y4)) {
                collides = true;
                break;
            }
        }
    }
    if (!collides) {
        collides = (contains(poly1.xPoints[0], poly1.yPoints[0], &poly2) ||
                    contains(poly2.xPoints[0], poly2.yPoints[0], &poly1));
    }

    if (collides) {
        printf("collision!\n");
    } else {
        printf("no collision\n");
    }

    return 0;
}
