// Assignment 2 - Efficient C Programming
// System Programming, DGIST, Prof. Yeseong Kim
// 
// YOU WILL TURN IN THIS FILE.
// Read the provided instruction carefully.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmplib.h"
#include "hw2.h"

// This implementation is simply copied from "main.c".
// Your job is to modify and optimize it for better performance!

// original

// static Pixel convolution(
//         Pixel* input, int x, int y, int width, int height, float* filter) {
//     double r = 0;
//     double g = 0;
//     double b = 0;

//     for (int dx = -1; dx < 2; ++dx) {
//         for (int dy = -1; dy < 2; ++dy) {
//             if ((y + dy) < 0 || (y + dy) >= height)
//                 continue;

//             if ((x + dx) < 0 || (x + dx) >= width)
//                 continue;

//             r += input[(x+dx)+(y+dy)*width].r * filter[(dx+1)+(dy+1)*3];
//             g += input[(x+dx)+(y+dy)*width].g * filter[(dx+1)+(dy+1)*3];
//             b += input[(x+dx)+(y+dy)*width].b * filter[(dx+1)+(dy+1)*3];
//         }
//     }

//     if (r < 0) r = 0;
//     if (g < 0) g = 0;
//     if (b < 0) b = 0;

//     if (r > 255) r = 255;
//     if (g > 255) g = 255;
//     if (b > 255) b = 255;
    
//     Pixel p;
//     memset(&p, 0, sizeof(p));

//     p.r = (unsigned char)r;
//     p.g = (unsigned char)g;
//     p.b = (unsigned char)b;

//     return p;
// }

// if optimize

// static Pixel convolution(
//         Pixel* input, int x, int y, int width, int height, float* filter) {
//     double r = 0;
//     double g = 0;
//     double b = 0;

//     for (int dx = -1; dx <= 1; ++dx) {
//         int nx = x + dx;
//         if (nx < 0 || nx >= width)
//             continue;

//         for (int dy = -1; dy <= 1; ++dy) {
//             int ny = y + dy;
//             if (ny < 0 || ny >= height)
//                 continue;

//             r += input[nx + ny * width].r * filter[(dx + 1) + (dy + 1) * 3];
//             g += input[nx + ny * width].g * filter[(dx + 1) + (dy + 1) * 3];
//             b += input[nx + ny * width].b * filter[(dx + 1) + (dy + 1) * 3];
//         }
//     }

//     r = r < 0 ? 0 : (r > 255 ? 255 : r);
//     g = g < 0 ? 0 : (g > 255 ? 255 : g);
//     b = b < 0 ? 0 : (b > 255 ? 255 : b);
    
//     Pixel p;
//     p.r = (unsigned char)r;
//     p.g = (unsigned char)g;
//     p.b = (unsigned char)b;

//     return p;
// }

// for loop optimize

// static Pixel convolution(
//         Pixel* input, int x, int y, int width, int height, float* filter) {
//     double r = 0;
//     double g = 0;
//     double b = 0;

//     int x_min = x - 1 < 0 ? 0 : x - 1;
//     int x_max = x + 1 >= width ? width - 1 : x + 1;
//     int y_min = y - 1 < 0 ? 0 : y - 1;
//     int y_max = y + 1 >= height ? height - 1 : y + 1;

//     for (int dx = x_min; dx <= x_max; ++dx) {
//         for (int dy = y_min; dy <= y_max; ++dy) {
//             r += input[dx + dy * width].r * filter[(dx - x + 1) + (dy - y + 1) * 3];
//             g += input[dx + dy * width].g * filter[(dx - x + 1) + (dy - y + 1) * 3];
//             b += input[dx + dy * width].b * filter[(dx - x + 1) + (dy - y + 1) * 3];
//         }
//     }

//     r = r < 0 ? 0 : (r > 255 ? 255 : r);
//     g = g < 0 ? 0 : (g > 255 ? 255 : g);
//     b = b < 0 ? 0 : (b > 255 ? 255 : b);
    
//     Pixel p;
//     p.r = (unsigned char)r;
//     p.g = (unsigned char)g;
//     p.b = (unsigned char)b;

//     return p;
// }

// original

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     for (int x = 0; x < width; ++x) {
//         for (int y = 0; y < height; ++y) {
//             Pixel* p = (Pixel*)malloc(sizeof(Pixel));
//             *p = convolution(input, x, y, width, height, filter);

//             output[x+y*width].r = p->r;
//             output[x+y*width].g = p->g;
//             output[x+y*width].b = p->b;

//             free(p);
//         }
//     }
// }

// no memory alloc

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     for (int x = 0; x < width; ++x) {
//         for (int y = 0; y < height; ++y) {
//             Pixel p = convolution(input, x, y, width, height, filter);
//             output[x + y * width] = p;
//         }
//     }
// }

// OpenMP

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     #pragma omp parallel for collapse(2)
//     for (int x = 0; x < width; ++x) {
//         for (int y = 0; y < height; ++y) {
//             output[x + y * width] = convolution(input, x, y, width, height, filter);
//         }
//     }
// }


// caching

// static Pixel convolution(
//         Pixel* input, int x, int y, int width, int height, float* filter, Pixel* block) {
//     double r = 0;
//     double g = 0;
//     double b = 0;

//     for (int dx = -1; dx <= 1; ++dx) {
//         for (int dy = -1; dy <= 1; ++dy) {
//             int nx = x + dx;
//             int ny = y + dy;

//             Pixel pixel = block[(dx + 1) + (dy + 1) * 3];

//             r += pixel.r * filter[(dx + 1) + (dy + 1) * 3];
//             g += pixel.g * filter[(dx + 1) + (dy + 1) * 3];
//             b += pixel.b * filter[(dx + 1) + (dy + 1) * 3];
//         }
//     }

//     r = r < 0 ? 0 : (r > 255 ? 255 : r);
//     g = g < 0 ? 0 : (g > 255 ? 255 : g);
//     b = b < 0 ? 0 : (b > 255 ? 255 : b);
    
//     Pixel p;
//     p.r = (unsigned char)r;
//     p.g = (unsigned char)g;
//     p.b = (unsigned char)b;

//     return p;
// }

// static Pixel convolution(
//         Pixel* input, int x, int y, int width, int height, float* filter, Pixel* block) {
//     float filter[3], filter[3], filter[3];

//     double r, g, b;
    



//     r = r < 0 ? 0 : (r > 255 ? 255 : r);
//     g = g < 0 ? 0 : (g > 255 ? 255 : g);
//     b = b < 0 ? 0 : (b > 255 ? 255 : b);
    
//     Pixel p;
//     p.r = (unsigned char)r;
//     p.g = (unsigned char)g;
//     p.b = (unsigned char)b;

//     return p;
// }

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     Pixel block[9];

//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             for (int dy = -1; dy <= 1; ++dy) {
//                 for (int dx = -1; dx <= 1; ++dx) {
//                     int nx = x + dx;
//                     int ny = y + dy;

//                     if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
//                         block[(dx + 1) + (dy + 1) * 3] = (Pixel){0, 0, 0};
//                     } else {
//                         block[(dx + 1) + (dy + 1) * 3] = input[nx + ny * width];
//                     }
//                 }
//             }

//             output[x + y * width] = convolution(input, x, y, width, height, filter, block);
//         }
//     }
// }

static Pixel convolution(Pixel* input, int x, int y, int width, int height, float* filter) {
    double r = 0;
    double g = 0;
    double b = 0;

    // 루프 언롤링
    for (int dx = -1; dx <= 1; ++dx) {
        int xIndex = x + dx;
        if (xIndex < 0 || xIndex >= width)
            continue;

        for (int dy = -1; dy <= 1; ++dy) {
            int yIndex = y + dy;
            if (yIndex < 0 || yIndex >= height)
                continue;

            int inputIndex = xIndex + yIndex * width;
            int filterIndex = (dx + 1) + (dy + 1) * 3;

            r += input[inputIndex].r * filter[filterIndex];
            g += input[inputIndex].g * filter[filterIndex];
            b += input[inputIndex].b * filter[filterIndex];
        }
    }

    // 클램핑
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    Pixel p;
    p.r = (unsigned char)r;
    p.g = (unsigned char)g;
    p.b = (unsigned char)b;

    return p;
}

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = (Pixel*)args[2];
//     Pixel* output = (Pixel*)args[3];
//     float* filter = (float*)args[4];

//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x += 2) {  // 2개씩 처리
//             Pixel p1 = convolution(input, x, y, width, height, filter);
//             output[x + y * width] = p1;

//             if (x + 1 < width) {  // 경계를 벗어나지 않도록 체크
//                 Pixel p2 = convolution(input, x + 1, y, width, height, filter);
//                 output[x + 1 + y * width] = p2;
//             }
//         }
//     }
// }

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = (Pixel*)args[2];
//     Pixel* output = (Pixel*)args[3];
//     float* filter = (float*)args[4];

//     // 주변 픽셀 인덱스 미리 계산
//     int widthMinusOne = width - 1;
//     int heightMinusOne = height - 1;
//     int widthTimesHeight = width * height;
    
//     // 외부 루프 언롤링 (y좌표에 대한 루프)
//     for (int y = 0; y < height; ++y) {
//         // y좌표에 대한 상수 계산
//         int yWidth = y * width;
//         int yPlus1Width = (y < heightMinusOne) ? yWidth + width : yWidth;
//         int yMinus1Width = (y > 0) ? yWidth - width : yWidth;

//         // 내부 루프 언롤링 (x좌표에 대한 루프)
//         for (int x = 0; x < width; ++x) {
//             // x좌표에 대한 상수 계산
//             int xPlus1 = (x < widthMinusOne) ? x + 1 : x;
//             int xMinus1 = (x > 0) ? x - 1 : x;

//             // 픽셀 연산
//             double r = 0, g = 0, b = 0;
//             int filterIndex = 0;

//             // 3x3 필터 적용
//             for (int dy = -1; dy <= 1; ++dy) {
//                 int yIndex = y + dy;
//                 if (yIndex < 0 || yIndex >= height) continue;

//                 for (int dx = -1; dx <= 1; ++dx) {
//                     int xIndex = x + dx;
//                     if (xIndex < 0 || xIndex >= width) continue;

//                     Pixel* currentPixel = &input[xIndex + yIndex * width];
//                     float currentFilterValue = filter[filterIndex++];

//                     r += currentPixel->r * currentFilterValue;
//                     g += currentPixel->g * currentFilterValue;
//                     b += currentPixel->b * currentFilterValue;
//                 }
//             }

//             // 클램핑
//             r = (r < 0) ? 0 : ((r > 255) ? 255 : r);
//             g = (g < 0) ? 0 : ((g > 255) ? 255 : g);
//             b = (b < 0) ? 0 : ((b > 255) ? 255 : b);

//             // 결과 저장
//             output[x + yWidth].r = (unsigned char)r;
//             output[x + yWidth].g = (unsigned char)g;
//             output[x + yWidth].b = (unsigned char)b;
//         }
//     }
// }

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = (Pixel*)args[2];
//     Pixel* output = (Pixel*)args[3];
//     float* filter = (float*)args[4];

//     // 외부 루프 언롤링 (y좌표에 대한 루프)
//     for (int y = 0; y < height; ++y) {
//         // y좌표에 대한 상수 계산
//         int yWidth = y * width;

//         // 내부 루프 언롤링 (x좌표에 대한 루프)
//         for (int x = 0; x < width; ++x) {
//             // 픽셀 연산
//             double r = 0, g = 0, b = 0;

//             // 3x3 필터 적용
//             int filterIndex = 0; // 필터 인덱스를 반복문 밖으로 이동
//             for (int dy = -1; dy <= 1; ++dy) {
//                 int yIndex = y + dy;
//                 if (yIndex < 0 || yIndex >= height) continue;

//                 for (int dx = -1; dx <= 1; ++dx) {
//                     int xIndex = x + dx;
//                     if (xIndex < 0 || xIndex >= width) continue;

//                     Pixel* currentPixel = &input[xIndex + yIndex * width];
//                     float currentFilterValue = filter[filterIndex]; // 필터 인덱스 사용
//                     filterIndex++; // 필터 인덱스 증가

//                     r += currentPixel->r * currentFilterValue;
//                     g += currentPixel->g * currentFilterValue;
//                     b += currentPixel->b * currentFilterValue;
//                 }
//             }

//             // 클램핑
//             r = (r < 0) ? 0 : ((r > 255) ? 255 : r);
//             g = (g < 0) ? 0 : ((g > 255) ? 255 : g);
//             b = (b < 0) ? 0 : ((b > 255) ? 255 : b);

//             // 결과 저장
//             output[x + yWidth].r = (unsigned char)r;
//             output[x + yWidth].g = (unsigned char)g;
//             output[x + yWidth].b = (unsigned char)b;
//         }
//     }
// }

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     // Loop unrolling by 2 for both x and y loops
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             double r = 0, g = 0, b = 0;

//             // Unrolled loop for filter application
//             for (int dy = -1; dy < 2; ++dy) {
//                 for (int dx = -1; dx < 2; ++dx) {
//                     int nx = x + dx;
//                     int ny = y + dy;

//                     if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel = input[nx + ny * width];
//                         r += pixel.r * filter[index];
//                         g += pixel.g * filter[index];
//                         b += pixel.b * filter[index];
//                     }
//                 }
//             }

//             // Clamping and assigning values to output
//             output[x + y * width].r = (unsigned char)(r < 0 ? 0 : (r > 255 ? 255 : r));
//             output[x + y * width].g = (unsigned char)(g < 0 ? 0 : (g > 255 ? 255 : g));
//             output[x + y * width].b = (unsigned char)(b < 0 ? 0 : (b > 255 ? 255 : b));
//         }
//     }
// }

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     // Loop unrolling by 2 for both x and y loops
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; x += 2) { // Unrolled loop for x by 2
//             double r1 = 0, g1 = 0, b1 = 0;
//             double r2 = 0, g2 = 0, b2 = 0;

//             // Unrolled loop for filter application
//             for (int dy = -1; dy < 2; ++dy) {
//                 for (int dx = -1; dx < 2; ++dx) {
//                     int nx1 = x + dx;
//                     int nx2 = x + dx + 1; // Update x for the second pixel
//                     int ny = y + dy;

//                     if (nx1 >= 0 && nx1 < width && ny >= 0 && ny < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel1 = input[nx1 + ny * width];
//                         r1 += pixel1.r * filter[index];
//                         g1 += pixel1.g * filter[index];
//                         b1 += pixel1.b * filter[index];
//                     }

//                     if (nx2 >= 0 && nx2 < width && ny >= 0 && ny < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel2 = input[nx2 + ny * width];
//                         r2 += pixel2.r * filter[index];
//                         g2 += pixel2.g * filter[index];
//                         b2 += pixel2.b * filter[index];
//                     }
//                 }
//             }

//             // Clamping and assigning values to output for the first pixel
//             output[x + y * width].r = (unsigned char)(r1 < 0 ? 0 : (r1 > 255 ? 255 : r1));
//             output[x + y * width].g = (unsigned char)(g1 < 0 ? 0 : (g1 > 255 ? 255 : g1));
//             output[x + y * width].b = (unsigned char)(b1 < 0 ? 0 : (b1 > 255 ? 255 : b1));

//             // Clamping and assigning values to output for the second pixel
//             output[x + 1 + y * width].r = (unsigned char)(r2 < 0 ? 0 : (r2 > 255 ? 255 : r2));
//             output[x + 1 + y * width].g = (unsigned char)(g2 < 0 ? 0 : (g2 > 255 ? 255 : g2));
//             output[x + 1 + y * width].b = (unsigned char)(b2 < 0 ? 0 : (b2 > 255 ? 255 : b2));
//         }
//     }
// }

// void filter_optimized(void* args[]) {
//     // fastest!!!!!!!!!!!!!!!!!!!!
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     // Unrolled loop for both x and y
//     for (int y = 0; y < height; y += 2) {
//         for (int x = 0; x < width; x += 2) {
//             double r1 = 0, g1 = 0, b1 = 0;
//             double r2 = 0, g2 = 0, b2 = 0;
//             double r3 = 0, g3 = 0, b3 = 0;
//             double r4 = 0, g4 = 0, b4 = 0;

//             // Unrolled loop for filter application
//             for (int dy = -1; dy < 2; ++dy) {
//                 for (int dx = -1; dx < 2; ++dx) {
//                     int nx1 = x + dx;
//                     int nx2 = x + dx + 1;
//                     int nx3 = x + dx;
//                     int nx4 = x + dx + 1;
//                     int ny1 = y + dy;
//                     int ny2 = y + dy + 1;

//                     if (nx1 >= 0 && nx1 < width && ny1 >= 0 && ny1 < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel1 = input[nx1 + ny1 * width];
//                         r1 += pixel1.r * filter[index];
//                         g1 += pixel1.g * filter[index];
//                         b1 += pixel1.b * filter[index];
//                     }

//                     if (nx2 >= 0 && nx2 < width && ny1 >= 0 && ny1 < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel2 = input[nx2 + ny1 * width];
//                         r2 += pixel2.r * filter[index];
//                         g2 += pixel2.g * filter[index];
//                         b2 += pixel2.b * filter[index];
//                     }

//                     if (nx3 >= 0 && nx3 < width && ny2 >= 0 && ny2 < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel3 = input[nx3 + ny2 * width];
//                         r3 += pixel3.r * filter[index];
//                         g3 += pixel3.g * filter[index];
//                         b3 += pixel3.b * filter[index];
//                     }

//                     if (nx4 >= 0 && nx4 < width && ny2 >= 0 && ny2 < height) {
//                         int index = (dx + 1) + (dy + 1) * 3;
//                         Pixel pixel4 = input[nx4 + ny2 * width];
//                         r4 += pixel4.r * filter[index];
//                         g4 += pixel4.g * filter[index];
//                         b4 += pixel4.b * filter[index];
//                     }
//                 }
//             }

//             // Clamping and assigning values to output for the first pixel
//             output[x + y * width].r = (unsigned char)(r1 < 0 ? 0 : (r1 > 255 ? 255 : r1));
//             output[x + y * width].g = (unsigned char)(g1 < 0 ? 0 : (g1 > 255 ? 255 : g1));
//             output[x + y * width].b = (unsigned char)(b1 < 0 ? 0 : (b1 > 255 ? 255 : b1));

//             // Clamping and assigning values to output for the second pixel
//             output[x + 1 + y * width].r = (unsigned char)(r2 < 0 ? 0 : (r2 > 255 ? 255 : r2));
//             output[x + 1 + y * width].g = (unsigned char)(g2 < 0 ? 0 : (g2 > 255 ? 255 : g2));
//             output[x + 1 + y * width].b = (unsigned char)(b2 < 0 ? 0 : (b2 > 255 ? 255 : b2));

//             // Clamping and assigning values to output for the third pixel
//             output[x + (y + 1) * width].r = (unsigned char)(r3 < 0 ? 0 : (r3 > 255 ? 255 : r3));
//             output[x + (y + 1) * width].g = (unsigned char)(g3 < 0 ? 0 : (g3 > 255 ? 255 : g3));
//             output[x + (y + 1) * width].b = (unsigned char)(b3 < 0 ? 0 : (b3 > 255 ? 255 : b3));

//             // Clamping and assigning values to output for the fourth pixel
//             output[x + 1 + (y + 1) * width].r = (unsigned char)(r4 < 0 ? 0 : (r4 > 255 ? 255 : r4));
//             output[x + 1 + (y + 1) * width].g = (unsigned char)(g4 < 0 ? 0 : (g4 > 255 ? 255 : g4));
//             output[x + 1 + (y + 1) * width].b = (unsigned char)(b4 < 0 ? 0 : (b4 > 255 ? 255 : b4));
//         }
//     }
// }

// void filter_optimized(void* args[]) {
        // fast fastest !!!!!!!!!!!!!!!!!!
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = args[2];
//     Pixel* output = args[3];
//     float* filter = args[4];

//     for (int y = 0; y < height; y += 2) {
//         for (int x = 0; x < width; x += 2) {
//             double r1 = 0, g1 = 0, b1 = 0;
//             double r2 = 0, g2 = 0, b2 = 0;
//             double r3 = 0, g3 = 0, b3 = 0;
//             double r4 = 0, g4 = 0, b4 = 0;

//             for (int dy = -1; dy <= 1; ++dy) {
//                 for (int dx = -1; dx <= 1; ++dx) {
//                     int nx1 = x + dx;
//                     int nx2 = x + dx + 1;
//                     int ny1 = y + dy;
//                     int ny2 = y + dy + 1;
//                     int index = (dx + 1) + (dy + 1) * 3;

//                     if (nx1 >= 0 && nx1 < width && ny1 >= 0 && ny1 < height) {
//                         Pixel pixel1 = input[nx1 + ny1 * width];
//                         r1 += pixel1.r * filter[index];
//                         g1 += pixel1.g * filter[index];
//                         b1 += pixel1.b * filter[index];
//                     }

//                     if (nx2 >= 0 && nx2 < width && ny1 >= 0 && ny1 < height) {
//                         Pixel pixel2 = input[nx2 + ny1 * width];
//                         r2 += pixel2.r * filter[index];
//                         g2 += pixel2.g * filter[index];
//                         b2 += pixel2.b * filter[index];
//                     }

//                     if (nx1 >= 0 && nx1 < width && ny2 >= 0 && ny2 < height) {
//                         Pixel pixel3 = input[nx1 + ny2 * width];
//                         r3 += pixel3.r * filter[index];
//                         g3 += pixel3.g * filter[index];
//                         b3 += pixel3.b * filter[index];
//                     }

//                     if (nx2 >= 0 && nx2 < width && ny2 >= 0 && ny2 < height) {
//                         Pixel pixel4 = input[nx2 + ny2 * width];
//                         r4 += pixel4.r * filter[index];
//                         g4 += pixel4.g * filter[index];
//                         b4 += pixel4.b * filter[index];
//                     }
//                 }
//             }

//             // Clamping and assigning values to output
//             output[x + y * width].r = (unsigned char)(r1 < 0 ? 0 : (r1 > 255 ? 255 : r1));
//             output[x + y * width].g = (unsigned char)(g1 < 0 ? 0 : (g1 > 255 ? 255 : g1));
//             output[x + y * width].b = (unsigned char)(b1 < 0 ? 0 : (b1 > 255 ? 255 : b1));

//             if (x + 1 < width) {
//                 output[x + 1 + y * width].r = (unsigned char)(r2 < 0 ? 0 : (r2 > 255 ? 255 : r2));
//                 output[x + 1 + y * width].g = (unsigned char)(g2 < 0 ? 0 : (g2 > 255 ? 255 : g2));
//                 output[x + 1 + y * width].b = (unsigned char)(b2 < 0 ? 0 : (b2 > 255 ? 255 : b2));
//             }

//             if (y + 1 < height) {
//                 output[x + (y + 1) * width].r = (unsigned char)(r3 < 0 ? 0 : (r3 > 255 ? 255 : r3));
//                 output[x + (y + 1) * width].g = (unsigned char)(g3 < 0 ? 0 : (g3 > 255 ? 255 : g3));
//                 output[x + (y + 1) * width].b = (unsigned char)(b3 < 0 ? 0 : (b3 > 255 ? 255 : b3));

//                 if (x + 1 < width) {
//                     output[x + 1 + (y + 1) * width].r = (unsigned char)(r4 < 0 ? 0 : (r4 > 255 ? 255 : r4));
//                     output[x + 1 + (y + 1) * width].g = (unsigned char)(g4 < 0 ? 0 : (g4 > 255 ? 255 : g4));
//                     output[x + 1 + (y + 1) * width].b = (unsigned char)(b4 < 0 ? 0 : (b4 > 255 ? 255 : b4));
//                 }
//             }
//         }
//     }
// }

// void filter_optimized(void* args[]) {
//     unsigned int width = *(unsigned int*)args[0];
//     unsigned int height = *(unsigned int*)args[1];
//     Pixel* input = (Pixel*)args[2];
//     Pixel* output = (Pixel*)args[3];
//     float* filter = (float*)args[4];

//     for (int y = 0; y < height; y += 2) {
//         for (int x = 0; x < width; x += 2) {
//             double r1 = 0, g1 = 0, b1 = 0;
//             double r2 = 0, g2 = 0, b2 = 0;
//             double r3 = 0, g3 = 0, b3 = 0;
//             double r4 = 0, g4 = 0, b4 = 0;

//             for (int dy = -1; dy <= 1; ++dy) {
//                 int ny1 = y + dy;
//                 int ny2 = y + dy + 1;

//                 for (int dx = -1; dx <= 1; ++dx) {
//                     int nx1 = x + dx;
//                     int nx2 = x + dx + 1;
//                     int index = (dx + 1) + (dy + 1) * 3;

//                     if (nx1 >= 0 && nx1 < width) {
//                         if (ny1 >= 0 && ny1 < height) {
//                             Pixel pixel1 = input[nx1 + ny1 * width];
//                             r1 += pixel1.r * filter[index];
//                             g1 += pixel1.g * filter[index];
//                             b1 += pixel1.b * filter[index];
//                         }
//                         if (ny2 >= 0 && ny2 < height) {
//                             Pixel pixel3 = input[nx1 + ny2 * width];
//                             r3 += pixel3.r * filter[index];
//                             g3 += pixel3.g * filter[index];
//                             b3 += pixel3.b * filter[index];
//                         }
//                     }

//                     if (nx2 >= 0 && nx2 < width) {
//                         if (ny1 >= 0 && ny1 < height) {
//                             Pixel pixel2 = input[nx2 + ny1 * width];
//                             r2 += pixel2.r * filter[index];
//                             g2 += pixel2.g * filter[index];
//                             b2 += pixel2.b * filter[index];
//                         }
//                         if (ny2 >= 0 && ny2 < height) {
//                             Pixel pixel4 = input[nx2 + ny2 * width];
//                             r4 += pixel4.r * filter[index];
//                             g4 += pixel4.g * filter[index];
//                             b4 += pixel4.b * filter[index];
//                         }
//                     }
//                 }
//             }

//             // Clamping and assigning values to output
//             output[x + y * width].r = (unsigned char)(r1 < 0 ? 0 : (r1 > 255 ? 255 : r1));
//             output[x + y * width].g = (unsigned char)(g1 < 0 ? 0 : (g1 > 255 ? 255 : g1));
//             output[x + y * width].b = (unsigned char)(b1 < 0 ? 0 : (b1 > 255 ? 255 : b1));

//             if (x + 1 < width) {
//                 output[x + 1 + y * width].r = (unsigned char)(r2 < 0 ? 0 : (r2 > 255 ? 255 : r2));
//                 output[x + 1 + y * width].g = (unsigned char)(g2 < 0 ? 0 : (g2 > 255 ? 255 : g2));
//                 output[x + 1 + y * width].b = (unsigned char)(b2 < 0 ? 0 : (b2 > 255 ? 255 : b2));
//             }

//             if (y + 1 < height) {
//                 output[x + (y + 1) * width].r = (unsigned char)(r3 < 0 ? 0 : (r3 > 255 ? 255 : r3));
//                 output[x + (y + 1) * width].g = (unsigned char)(g3 < 0 ? 0 : (g3 > 255 ? 255 : g3));
//                 output[x + (y + 1) * width].b = (unsigned char)(b3 < 0 ? 0 : (b3 > 255 ? 255 : b3));

//                 if (x + 1 < width) {
//                     output[x + 1 + (y + 1) * width].r = (unsigned char)(r4 < 0 ? 0 : (r4 > 255 ? 255 : r4));
//                     output[x + 1 + (y + 1) * width].g = (unsigned char)(g4 < 0 ? 0 : (g4 > 255 ? 255 : g4));
//                     output[x + 1 + (y + 1) * width].b = (unsigned char)(b4 < 0 ? 0 : (b4 > 255 ? 255 : b4));
//                 }
//             }
//         }
//     }
// }

void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = (Pixel*)args[2];
    Pixel* output = (Pixel*)args[3];
    float* filter = (float*)args[4];

    for (int y = 0; y < height; y += 2) {
        for (int x = 0; x < width; x += 2) {
            double r1 = 0, g1 = 0, b1 = 0;
            double r2 = 0, g2 = 0, b2 = 0;
            double r3 = 0, g3 = 0, b3 = 0;
            double r4 = 0, g4 = 0, b4 = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                int ny1 = y + dy;
                int ny2 = ny1 + 1;

                for (int dx = -1; dx <= 1; ++dx) {
                    int nx1 = x + dx;
                    int nx2 = nx1 + 1;
                    int index = (dx + 1) + (dy + 1) * 3;
                    float filter_val = filter[index];

                    if (nx1 >= 0 && nx1 < width) {
                        if (ny1 >= 0 && ny1 < height) {
                            Pixel pixel1 = input[nx1 + ny1 * width];
                            r1 += pixel1.r * filter_val;
                            g1 += pixel1.g * filter_val;
                            b1 += pixel1.b * filter_val;
                        }
                        if (ny2 >= 0 && ny2 < height) {
                            Pixel pixel3 = input[nx1 + ny2 * width];
                            r3 += pixel3.r * filter_val;
                            g3 += pixel3.g * filter_val;
                            b3 += pixel3.b * filter_val;
                        }
                    }

                    if (nx2 >= 0 && nx2 < width) {
                        if (ny1 >= 0 && ny1 < height) {
                            Pixel pixel2 = input[nx2 + ny1 * width];
                            r2 += pixel2.r * filter_val;
                            g2 += pixel2.g * filter_val;
                            b2 += pixel2.b * filter_val;
                        }
                        if (ny2 >= 0 && ny2 < height) {
                            Pixel pixel4 = input[nx2 + ny2 * width];
                            r4 += pixel4.r * filter_val;
                            g4 += pixel4.g * filter_val;
                            b4 += pixel4.b * filter_val;
                        }
                    }
                }
            }

            // Clamping and assigning values to output
            output[x + y * width].r = (unsigned char)(r1 < 0 ? 0 : (r1 > 255 ? 255 : r1));
            output[x + y * width].g = (unsigned char)(g1 < 0 ? 0 : (g1 > 255 ? 255 : g1));
            output[x + y * width].b = (unsigned char)(b1 < 0 ? 0 : (b1 > 255 ? 255 : b1));

            if (x + 1 < width) {
                output[x + 1 + y * width].r = (unsigned char)(r2 < 0 ? 0 : (r2 > 255 ? 255 : r2));
                output[x + 1 + y * width].g = (unsigned char)(g2 < 0 ? 0 : (g2 > 255 ? 255 : g2));
                output[x + 1 + y * width].b = (unsigned char)(b2 < 0 ? 0 : (b2 > 255 ? 255 : b2));
            }

            if (y + 1 < height) {
                output[x + (y + 1) * width].r = (unsigned char)(r3 < 0 ? 0 : (r3 > 255 ? 255 : r3));
                output[x + (y + 1) * width].g = (unsigned char)(g3 < 0 ? 0 : (g3 > 255 ? 255 : g3));
                output[x + (y + 1) * width].b = (unsigned char)(b3 < 0 ? 0 : (b3 > 255 ? 255 : b3));

                if (x + 1 < width) {
                    output[x + 1 + (y + 1) * width].r = (unsigned char)(r4 < 0 ? 0 : (r4 > 255 ? 255 : r4));
                    output[x + 1 + (y + 1) * width].g = (unsigned char)(g4 < 0 ? 0 : (g4 > 255 ? 255 : g4));
                    output[x + 1 + (y + 1) * width].b = (unsigned char)(b4 < 0 ? 0 : (b4 > 255 ? 255 : b4));
                }
            }
        }
    }
}