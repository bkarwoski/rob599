#include "bmp.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"

//void gx_plotLine(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color);
//Use ONLY for test case 2!

void gx_draw_line(bitmap_t *bmp, color_bgr_t color,int x0, int y0, int x1, int y1);

void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points);

void gx_draw_poly(bitmap_t *bmp, color_bgr_t color, vector_xy_t *shape);

void roundC(vector_xy_t *doubles);

void gx_rasterize_line(int x0, int y0, int x1, int y1, vector_xy_t *perimeter);

void gx_perimeter(vector_xy_t *shape, vector_xy_t *perimeter);

void gx_trans(double x, double y, vector_xy_t *vec);

void gx_fill_poly(bitmap_t *bmp, color_bgr_t color, vector_xy_t *shape);

void gx_fill(vector_xy_t *points);

vector_xy_t gx_rect(double width, double height);
//vector_xy_t *gx_rect(double width, double height);
/*

void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points);

void gx_clear(bitmap_t *bmp, color_bgr_t color);

vector_xy_t *gx_rect(double width, double height);

vector_xy_t *gx_robot(double dim);

vector_xy_t *gx_rot(double theta, vector_xy_t *points);

vector_xy_t *gx_trans(double x, double y, vector_xy_t *points);
*/
