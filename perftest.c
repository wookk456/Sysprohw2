#include <stdio.h>
#include <time.h>

void clamp_rgb_1(int *r, int *g, int *b) {
    if (*r < 0) *r = 0;
    if (*g < 0) *g = 0;
    if (*b < 0) *b = 0;

    if (*r > 255) *r = 255;
    if (*g > 255) *g = 255;
    if (*b > 255) *b = 255;
}

void clamp_rgb_2(int *r, int *g, int *b) {
    *r = *r < 0 ? 0 : (*r > 255 ? 255 : *r);
    *g = *g < 0 ? 0 : (*g > 255 ? 255 : *g);
    *b = *b < 0 ? 0 : (*b > 255 ? 255 : *b);
}

int main() {
    int r, g, b;
    const int iterations = 1000000;
    clock_t start, end;
    double cpu_time_used;

    // Measure time for clamp_rgb_1
    r = 300; g = -20; b = 150;
    start = clock();
    for (int i = 0; i < iterations; i++) {
        int temp_r = r, temp_g = g, temp_b = b;
        clamp_rgb_1(&temp_r, &temp_g, &temp_b);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("First code execution time: %f seconds\n", cpu_time_used);

    // Measure time for clamp_rgb_2
    r = 300; g = -20; b = 150;
    start = clock();
    for (int i = 0; i < iterations; i++) {
        int temp_r = r, temp_g = g, temp_b = b;
        clamp_rgb_2(&temp_r, &temp_g, &temp_b);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Second code execution time: %f seconds\n", cpu_time_used);

    return 0;
}