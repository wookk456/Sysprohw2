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

#define CLAMP_COLOR(c) ((unsigned char)((c) < 0 ? 0 : ((c) > 255 ? 255 : (c))))

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
//                 int y_edge1 = y + dy;
//                 int y_edge2 = y_edge1 + 1;

//                 for (int dx = -1; dx <= 1; ++dx) {
//                     int x_edge1 = x + dx;
//                     int x_edge2 = x_edge1 + 1;
//                     int index = (dx + 1) + (dy + 1) * 3;
//                     float filter_val = filter[index];

//                     if (x_edge1 >= 0 && x_edge1 < width) {
//                         if (y_edge1 >= 0 && y_edge1 < height) {
//                             Pixel pixel1 = input[x_edge1 + y_edge1 * width];
//                             r1 += pixel1.r * filter_val;
//                             g1 += pixel1.g * filter_val;
//                             b1 += pixel1.b * filter_val;
//                         }
//                         if (y_edge2 >= 0 && y_edge2 < height) {
//                             Pixel pixel3 = input[x_edge1 + y_edge2 * width];
//                             r3 += pixel3.r * filter_val;
//                             g3 += pixel3.g * filter_val;
//                             b3 += pixel3.b * filter_val;
//                         }
//                     }

//                     if (x_edge2 >= 0 && x_edge2 < width) {
//                         if (y_edge1 >= 0 && y_edge1 < height) {
//                             Pixel pixel2 = input[x_edge2 + y_edge1 * width];
//                             r2 += pixel2.r * filter_val;
//                             g2 += pixel2.g * filter_val;
//                             b2 += pixel2.b * filter_val;
//                         }
//                         if (y_edge2 >= 0 && y_edge2 < height) {
//                             Pixel pixel4 = input[x_edge2 + y_edge2 * width];
//                             r4 += pixel4.r * filter_val;
//                             g4 += pixel4.g * filter_val;
//                             b4 += pixel4.b * filter_val;
//                         }
//                     }
//                 }
//             }

//             output[x + y * width].r = CLAMP_COLOR(r1);
//             output[x + y * width].g = CLAMP_COLOR(g1);
//             output[x + y * width].b = CLAMP_COLOR(b1);

//             if (x + 1 < width) {
//                 output[x + 1 + y * width].r = CLAMP_COLOR(r2);
//                 output[x + 1 + y * width].g = CLAMP_COLOR(g2);
//                 output[x + 1 + y * width].b = CLAMP_COLOR(b2);
//             }

//             if (y + 1 < height) {
//                 output[x + (y + 1) * width].r = CLAMP_COLOR(r3);
//                 output[x + (y + 1) * width].g = CLAMP_COLOR(g3);
//                 output[x + (y + 1) * width].b = CLAMP_COLOR(b3);

//                 if (x + 1 < width) {
//                     output[x + 1 + (y + 1) * width].r = CLAMP_COLOR(r4);
//                     output[x + 1 + (y + 1) * width].g = CLAMP_COLOR(g4);
//                     output[x + 1 + (y + 1) * width].b = CLAMP_COLOR(b4);
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

            // y-1, x-1
            int x_edge1 = x - 1;
            int x_edge2 = x;
            int x_edge3 = x + 1;
            int x_edge4 = x + 2;

            int y_edge1 = y - 1;
            int y_edge2 = y;
            int y_edge3 = y + 1;
            int y_edge4 = y + 2;

            float filter_val = filter[0];

            if (x_edge1 >= 0 && x_edge1 < width && y_edge1 >= 0 && y_edge1 < height) {
                Pixel pixel = input[x_edge1 + y_edge1 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge1 >= 0 && y_edge1 < height) {
                Pixel pixel = input[x_edge2 + y_edge1 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge1 >= 0 && x_edge1 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge1 + y_edge2 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge2 + y_edge2 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[1];
            if (x_edge2 >= 0 && x_edge2 < width && y_edge1 >= 0 && y_edge1 < height) {
                Pixel pixel = input[x_edge2 + y_edge1 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge1 >= 0 && y_edge1 < height) {
                Pixel pixel = input[x_edge3 + y_edge1 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge2 + y_edge2 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge3 + y_edge2 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[2];
            if (x_edge3 >= 0 && x_edge3 < width && y_edge1 >= 0 && y_edge1 < height) {
                Pixel pixel = input[x_edge3 + y_edge1 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge4 >= 0 && x_edge4 < width && y_edge1 >= 0 && y_edge1 < height) {
                Pixel pixel = input[x_edge4 + y_edge1 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge3 + y_edge2 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge4 >= 0 && x_edge4 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge4 + y_edge2 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[3];
            if (x_edge1 >= 0 && x_edge1 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge1 + y_edge2 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge2 + y_edge2 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge1 >= 0 && x_edge1 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge1 + y_edge3 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge2 + y_edge3 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[4];
            if (x_edge2 >= 0 && x_edge2 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge2 + y_edge2 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge3 + y_edge2 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge2 + y_edge3 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge3 + y_edge3 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[5];
            if (x_edge3 >= 0 && x_edge3 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge3 + y_edge2 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge4 >= 0 && x_edge4 < width && y_edge2 >= 0 && y_edge2 < height) {
                Pixel pixel = input[x_edge4 + y_edge2 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge3 + y_edge3 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge4 >= 0 && x_edge4 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge4 + y_edge3 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[6];
            if (x_edge1 >= 0 && x_edge1 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge1 + y_edge3 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge2 + y_edge3 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge1 >= 0 && x_edge1 < width && y_edge4 >= 0 && y_edge4 < height) {
                Pixel pixel = input[x_edge1 + y_edge4 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge4 >= 0 && y_edge4 < height) {
                Pixel pixel = input[x_edge2 + y_edge4 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[7];
            if (x_edge2 >= 0 && x_edge2 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge2 + y_edge3 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge3 + y_edge3 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge2 >= 0 && x_edge2 < width && y_edge4 >= 0 && y_edge4 < height) {
                Pixel pixel = input[x_edge2 + y_edge4 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge4 >= 0 && y_edge4 < height) {
                Pixel pixel = input[x_edge3 + y_edge4 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            filter_val = filter[8];
            if (x_edge3 >= 0 && x_edge3 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge3 + y_edge3 * width];
                r1 += pixel.r * filter_val;
                g1 += pixel.g * filter_val;
                b1 += pixel.b * filter_val;
            }
            if (x_edge4 >= 0 && x_edge4 < width && y_edge3 >= 0 && y_edge3 < height) {
                Pixel pixel = input[x_edge4 + y_edge3 * width];
                r2 += pixel.r * filter_val;
                g2 += pixel.g * filter_val;
                b2 += pixel.b * filter_val;
            }
            if (x_edge3 >= 0 && x_edge3 < width && y_edge4 >= 0 && y_edge4 < height) {
                Pixel pixel = input[x_edge3 + y_edge4 * width];
                r3 += pixel.r * filter_val;
                g3 += pixel.g * filter_val;
                b3 += pixel.b * filter_val;
            }
            if (x_edge4 >= 0 && x_edge4 < width && y_edge4 >= 0 && y_edge4 < height) {
                Pixel pixel = input[x_edge4 + y_edge4 * width];
                r4 += pixel.r * filter_val;
                g4 += pixel.g * filter_val;
                b4 += pixel.b * filter_val;
            }

            output[x + y * width].r = CLAMP_COLOR(r1);
            output[x + y * width].g = CLAMP_COLOR(g1);
            output[x + y * width].b = CLAMP_COLOR(b1);

            if (x + 1 < width) {
                output[x + 1 + y * width].r = CLAMP_COLOR(r2);
                output[x + 1 + y * width].g = CLAMP_COLOR(g2);
                output[x + 1 + y * width].b = CLAMP_COLOR(b2);
            }

            if (y + 1 < height) {
                output[x + (y + 1) * width].r = CLAMP_COLOR(r3);
                output[x + (y + 1) * width].g = CLAMP_COLOR(g3);
                output[x + (y + 1) * width].b = CLAMP_COLOR(b3);

                if (x + 1 < width) {
                    output[x + 1 + (y + 1) * width].r = CLAMP_COLOR(r4);
                    output[x + 1 + (y + 1) * width].g = CLAMP_COLOR(g4);
                    output[x + 1 + (y + 1) * width].b = CLAMP_COLOR(b4);
                }
            }
        }
    }
}