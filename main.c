#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void vectriad_C(size_t n, float* a, float* b, float* c, float* d) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = b[i] + c[i] * d[i];
}
extern vectriad_x86_64(size_t n, float* a, float* b, float* c, float* d);
extern vectriad_SIMDx(size_t n, float* a, float* b, float* c, float* d);
extern vectriad_SIMDy(size_t n, float* a, float* b, float* c, float* d);

int main() {
    printf("------------| Vector Triad |----------------\n");
    printf("a[i] = b[i] + c[i] * d[i]\n\n");

    // Define the size of the arrays
    const size_t ARRAY_SIZE = 1 << 24ULL;
    const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
    size_t i;
    printf("Number of elements = %zd\n", ARRAY_SIZE);

    // Variables for timing
    LARGE_INTEGER li;
    long long int start, end;
    double PCFreq, elapse, elapse1;
    QueryPerformanceFrequency(&li);
    PCFreq = (double)(li.QuadPart);

    // Allocate memory for the arrays
    float* a, * b, * c, * d, * sanity_check;
    a = (float*)malloc(ARRAY_BYTES);
    b = (float*)malloc(ARRAY_BYTES);
    c = (float*)malloc(ARRAY_BYTES);
    d = (float*)malloc(ARRAY_BYTES);
    sanity_check = (float*)malloc(ARRAY_BYTES);

    // Initialize arrays b, c, and d with some values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        b[i] = sin(i * 0.05) + 0.5;             // sine wave shifted upward
        c[i] = cos(i * 0.1) * 2.0;              // cosine wave with larger amplitude
        d[i] = sin(i * 0.02) * cos(i * 0.03);   // product of sine and cosine
    }

    //------------| Program in C |----------------//

    QueryPerformanceCounter(&li);
    start = li.QuadPart;
    vectriad_C(ARRAY_SIZE, a, b, c, d);
    QueryPerformanceCounter(&li);
    end = li.QuadPart;
    elapse = ((double)(end - start)) * 1000.0 / PCFreq;
    printf("Time in C = %f ms\n\n", elapse);

    // Print first and last 5 values of a
    printf("First 5 values of a:\n");
    for (i = 0; i < 5; i++) {
        printf("a[%d] = %f\n", i, a[i]);
    }
    printf("\nLast 5 values of a:\n");
    for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
        printf("a[%d] = %f\n", i, a[i]);
    }

    // Transfer all values from a to sanity_check
    for (i = 0; i < ARRAY_SIZE; i++) {
        sanity_check[i] = a[i];
    }

    //------------| Program in x86-64 Assembly Language |----------------//
    QueryPerformanceCounter(&li);
    start = li.QuadPart;
    vectriad_x86_64(ARRAY_SIZE, a, b, c, d);
    QueryPerformanceCounter(&li);
    end = li.QuadPart;
    elapse = ((double)(end - start)) * 1000.0 / PCFreq;
    printf("Time in x86-64 assembly = %f ms\n\n", elapse);

    // Ensure correctness by comparing a with sanity_check
    int errors = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (a[i] != sanity_check[i]) {
            errors += 1;
        }
    }

    if (errors != 0) {
        printf("Vector Triad x86-64 Assembly: FAILED with %d error/s\n", errors);
    }
    else {
        printf("Vector Triad x86-64 Assembly: PASSED\n");

        // Print first and last 5 values of a
        printf("First 5 values of a:\n");
        for (i = 0; i < 5; i++) {
            printf("a[%d] = %f\n", i, a[i]);
        }
        printf("\nLast 5 values of a:\n");
        for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
            printf("a[%d] = %f\n\n", i, a[i]);
        }
    }

    //------------| Program in SIMD XMM Register |----------------//
    QueryPerformanceCounter(&li);
    start = li.QuadPart;
    vectriad_SIMDx(ARRAY_SIZE, a, b, c, d);
    QueryPerformanceCounter(&li);
    end = li.QuadPart;
    elapse = ((double)(end - start)) * 1000.0 / PCFreq;
    printf("Time in SIMD XMM = %f ms\n\n", elapse);

    // Ensure correctness by comparing a with sanity_check
    errors = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (a[i] != sanity_check[i]) {
            errors += 1;
        }
    }

    if (errors != 0) {
        printf("Vector Triad SIMD XMM: FAILED with %d error/s\n", errors);
    }
    else {
        printf("Vector Triad SIMD XMM: PASSED\n");

        // Print first and last 5 values of a
        printf("First 5 values of a:\n");
        for (i = 0; i < 5; i++) {
            printf("a[%d] = %f\n", i, a[i]);
        }
        printf("\nLast 5 values of a:\n");
        for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
            printf("a[%d] = %f\n\n", i, a[i]);
        }
    }

    //------------| Program in SIMD YMM Register |----------------//
    QueryPerformanceCounter(&li);
    start = li.QuadPart;
    vectriad_SIMDy(ARRAY_SIZE, a, b, c, d);
    QueryPerformanceCounter(&li);
    end = li.QuadPart;
    elapse = ((double)(end - start)) * 1000.0 / PCFreq;
    printf("Time in SIMD YMM = %f ms\n\n", elapse);

    // Ensure correctness by comparing a with sanity_check
    errors = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (a[i] != sanity_check[i]) {
            errors += 1;
        }
    }

    if (errors != 0) {
        printf("Vector Triad SIMD YMM: FAILED with %d error/s\n", errors);
    }
    else {
        printf("Vector Triad SIMD YMM: PASSED\n");

        // Print first and last 5 values of a
        printf("First 5 values of a:\n");
        for (i = 0; i < 5; i++) {
            printf("a[%d] = %f\n", i, a[i]);
        }
        printf("\nLast 5 values of a:\n");
        for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
            printf("a[%d] = %f\n\n", i, a[i]);
        }
    }

    free(a);
    free(b);
    free(c);
    free(d);
    free(sanity_check);

    return 0;
    
}