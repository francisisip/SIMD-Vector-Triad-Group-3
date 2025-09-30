#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void vectriad(size_t n, float* a, float* b, float* c, float* d) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = b[i] + c[i] * d[i];
}

extern vector_triad_x86_64(size_t ARRAY_SIZE, float* a, float* b, float* c, float* d);
extern vecaddSIMDx(size_t n, float* a, float* b, float* c, float* d);
extern vecaddSIMDy(size_t n, float* a, float* b, float* c, float* d);

int main() {
    const size_t ARRAY_SIZE = 1 << 24ULL;
    const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(float);

    int i;
    printf("Number of elements = %zd\n", ARRAY_SIZE);

    LARGE_INTEGER li;
    long long int start, end;
    double PCFreq, elapse, elapse1;
    QueryPerformanceFrequency(&li);
    PCFreq = (double)(li.QuadPart);
    float* a, * b, * c, * d, * a_corect;
    a = (float*)malloc(ARRAY_BYTES);
    b = (float*)malloc(ARRAY_BYTES);
    c = (float*)malloc(ARRAY_BYTES);
    d = (float*)malloc(ARRAY_BYTES);
	a_corect = (float*)malloc(ARRAY_BYTES);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        b[i] = sin(i * 0.05) + 0.5;             // sine wave shifted upward
        c[i] = cos(i * 0.1) * 2.0;              // cosine wave with larger amplitude
        d[i] = sin(i * 0.02) * cos(i * 0.03);   // product of sine and cosine
    }

    QueryPerformanceCounter(&li);
    start = li.QuadPart;
    //vectriad(ARRAY_SIZE, a, b, c, d);
	printf("Array size = %zd\n", ARRAY_SIZE);
	vecaddSIMDy(ARRAY_SIZE, a, b, c, d);
    QueryPerformanceCounter(&li);
    end = li.QuadPart;
    elapse = ((double)(end - start)) * 1000.0 / PCFreq;
    printf("Time in C = %f ms\n\n", elapse);

    // PRINT first and last 5 values of a
    printf("First 5 values of a:\n");
	for (i = 0; i < 5; i++) {
        printf("a[%d] = %f\n", i, a[i]);
	}
	printf("\nLast 5 values of a:\n");
    for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
        printf("a[%d] = %f\n", i, a[i]);
	}

	// transfer all values from a to a_corect
    for (i = 0; i < ARRAY_SIZE; i++) {
        a_corect[i] = a[i];
	}

	free(a);
	free(b);
	free(c);
	free(d);
	free(a_corect);

	return 0;
}