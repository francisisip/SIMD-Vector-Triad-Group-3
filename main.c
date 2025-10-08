#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

void vectriad_C(size_t n, float* a, float* b, float* c, float* d) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = b[i] + c[i] * d[i];
}
extern void vectriad_x86_64(size_t n, float* a, float* b, float* c, float* d);
extern void vectriad_SIMDx(size_t n, float* a, float* b, float* c, float* d);
extern void vectriad_SIMDy(size_t n, float* a, float* b, float* c, float* d);

int main() {
    printf("------------| Vector Triad |----------------\n");
    printf("Formula: a[i] = b[i] + c[i] * d[i]\n");
    printf("Implementations: C, x86-64 Assembly, SIMD AVX2 assembly language using XMM register, SIMD AVX2 assembly language using YMM register\n");
    
    printf("\nInitialization formula:\n");
    printf("for (int i = 0; i < ARRAY_SIZE; i++) {\n");
    printf("    b[i] = sin(i * 0.05) + 0.5;\n");
    printf("    c[i] = cos(i * 0.1) * 2.0;\n");
    printf("    d[i] = sin(i * 0.02) * cos(i * 0.03);\n");
    printf("}\n");

	// Define vector sizes to test
    const int n_sizes[] = {
        20, 26, 28
    };
    int n_count = sizeof(n_sizes) / sizeof(n_sizes[0]);
    int ctr;

	// To hold average times for each implementation
    double c_average_times[] = { 0, 0, 0 };
	double x86_64_average_times[] = { 0, 0, 0 };
	double simdx_average_times[] = { 0, 0, 0 };
	double simdy_average_times[] = { 0, 0, 0 };

    // Variables for timing
    LARGE_INTEGER li;
    long long int start, end;
    double PCFreq, elapse, elapse1;
    QueryPerformanceFrequency(&li);
    PCFreq = (double)(li.QuadPart);

    // Start of main loop
    for (int ctr = 0; ctr < n_count; ctr++) {

		// Initialize offset based on size for boundary conditions
		// 2^26 + 3, 2^28 + 7
		int offset = 0;
        if (ctr == 1) {
			offset = 3;
        }
        else if (ctr == 2) {
            offset = 7;
        }

		// Calculate array size and array bytes
		const size_t ORIGINAL_ARRAY_SIZE = (1ULL << n_sizes[ctr]);
        const size_t ARRAY_SIZE = (1ULL << n_sizes[ctr]) + offset;
;       const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
        size_t i;

        printf("\n------------| Main Program |----------------\n");
        printf("\nVector Size = 2^%d or %zd\n", n_sizes[ctr], ORIGINAL_ARRAY_SIZE);
        printf("Number of runs = 30. ");
        printf("Offset is added to total number of elements to check if program can handle boundary conditions.\n");
        printf("Offset for this vector size = %d. UPDATED Vector Size = %zd\n", offset, ARRAY_SIZE);
        printf("For the sake of brevity, printing the first and last 5 elements will only be done for the first iteration.\n");
        printf("In effect, there are print statements designed to trigger for any errors in the program for any run.\n\n");
		
		// Flag to indicate first run for printing purposes
        int is_first = 1;

        // Allocate memory for the arrays
        float* a, * b, * c, * d, * sanity_check;
        a = (float*)malloc(ARRAY_BYTES);
        b = (float*)malloc(ARRAY_BYTES);
        c = (float*)malloc(ARRAY_BYTES);
        d = (float*)malloc(ARRAY_BYTES);
        sanity_check = (float*)malloc(ARRAY_BYTES);

        // Initialize arrays b, c, and d with some values
        for (int i = 0; i < ARRAY_SIZE; i++) {
            a[i] = 0.0f; // Initialize a to 0.0f
            b[i] = sin(i * 0.05) + 0.5;
            c[i] = cos(i * 0.1) * 2.0;
            d[i] = sin(i * 0.02) * cos(i * 0.03);
            sanity_check[i] = 0.0f; // Initialize sanity_check to 0.0f
        }

		// Initialize array for all 30 runs to 0.0f for all kernel versions
        double c_times[30] = { 0.0f };
		double x86_64_times[30] = { 0.0f };
		double simdx_times[30] = { 0.0f };
		double simdy_times[30] = { 0.0f };
		int runs = 30;
        int run_ctr;

		// Loop 30 times to get a more stable average time
        for (run_ctr = 0; run_ctr < runs; run_ctr++) {
            if (is_first) {
                printf("------------| Program in C |----------------\n");
            }
            
            QueryPerformanceCounter(&li);
            start = li.QuadPart;
            vectriad_C(ARRAY_SIZE, a, b, c, d);
            QueryPerformanceCounter(&li);
            end = li.QuadPart;
            elapse = ((double)(end - start)) * 1000.0 / PCFreq;
            c_times[run_ctr] = elapse;

            // Print first and last 5 values of a if it's the first run
            if (is_first) {
                printf("Time in C = %f ms\n\n", elapse);
                printf("Vector Triad C: PASSED\n");
                printf("First 5 values of a[i]:\n");
                for (i = 0; i < 5; i++) {
                    printf("a[%zu] = %f\n", i, a[i]);
                }
                printf("\nLast 5 values of a[i]:\n");
                for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
                    printf("a[%zu] = %f\n", i, a[i]);
                }
                printf("\n");
            }

            // Transfer all values from a to sanity_check
            for (i = 0; i < ARRAY_SIZE; i++) {
                sanity_check[i] = a[i];
            }

            if (is_first) {
                printf("------------| Program in x86-64 Assembly Language |----------------\n");
            }
            QueryPerformanceCounter(&li);
            start = li.QuadPart;
            vectriad_x86_64(ARRAY_SIZE, a, b, c, d);
            QueryPerformanceCounter(&li);
            end = li.QuadPart;
            elapse = ((double)(end - start)) * 1000.0 / PCFreq;

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
                x86_64_times[run_ctr] = elapse;

                if (is_first) {
                    printf("Time in x86-64 Assembly = %f ms\n\n", elapse);
                    printf("Vector Triad x86-64 Assembly: PASSED\n");
                    printf("First 5 values of a[i]:\n");
                    for (i = 0; i < 5; i++) {
                        printf("a[%zu] = %f\n", i, a[i]);
                    }
                    printf("\nLast 5 values of a[i]:\n");
                    for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
                        printf("a[%zu] = %f\n", i, a[i]);
                    }

                    printf("\n");
                }
            }

            if (is_first) {
                printf("------------| Program in SIMD XMM Register |----------------\n");
            }
            QueryPerformanceCounter(&li);
            start = li.QuadPart;
            vectriad_SIMDx(ARRAY_SIZE, a, b, c, d);
            QueryPerformanceCounter(&li);
            end = li.QuadPart;
            elapse = ((double)(end - start)) * 1000.0 / PCFreq;

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
				simdx_times[run_ctr] = elapse;

                if (is_first) {
					printf("Time in SIMD XMM = %f ms\n\n", elapse);
                    printf("Vector Triad SIMD XMM: PASSED\n");
                    printf("First 5 values of a[i]:\n");
                    for (i = 0; i < 5; i++) {
                        printf("a[%zu] = %f\n", i, a[i]);
                    }
                    printf("\nLast 5 values of a[i]:\n");
                    for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
                        printf("a[%zu] = %f\n", i, a[i]);
                    }

                    printf("\n");
                }
            }

            if (is_first) {
                printf("------------| Program in SIMD YMM Register |----------------\n");
            }
            QueryPerformanceCounter(&li);
            start = li.QuadPart;
            vectriad_SIMDy(ARRAY_SIZE, a, b, c, d);
            QueryPerformanceCounter(&li);
            end = li.QuadPart;
            elapse = ((double)(end - start)) * 1000.0 / PCFreq;

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
				simdy_times[run_ctr] = elapse;
                
                if (is_first) {
					printf("Time in SIMD YMM = %f ms\n\n", elapse);
                    printf("Vector Triad SIMD YMM: PASSED\n");
                    printf("First 5 values of a[i]:\n");
                    for (i = 0; i < 5; i++) {
                        printf("a[%zu] = %f\n", i, a[i]);
                    }
                    printf("\nLast 5 values of a[i]:\n");
                    for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
                        printf("a[%zu] = %f\n", i, a[i]);
                    }

                    printf("\n");
                }
            }

			// Reset is_first flag after the first run
            if (is_first) {
                is_first = 0;
            }	
        }

		// Visualize all times, then add and calculate average time for each kernel version
        printf("\n------------| Summary table for n = 2^%d |----------------\n", n_sizes[ctr]);
        printf("\n%-6s %-12s %-12s %-12s %-12s\n",
            "Run", "C (ms)", "x86-64 (ms)", "SIMD XMM (ms)", "SIMD YMM (ms)");
        printf("-------------------------------------------------------------\n");

        double c_avg = 0.0f, x86_64_avg = 0.0f, simdx_avg = 0.0f, simdy_avg = 0.0f;
        for (int k = 0; k < runs; k++) {
            printf("%-6d %-12.3f %-12.3f %-12.3f %-12.3f\n", k + 1, c_times[k], x86_64_times[k], simdx_times[k], simdy_times[k]);
            c_avg += c_times[k];
            x86_64_avg += x86_64_times[k];
            simdx_avg += simdx_times[k];
            simdy_avg += simdy_times[k];
        }

		c_average_times[ctr] = c_avg / runs;
		x86_64_average_times[ctr] = x86_64_avg / runs;
		simdx_average_times[ctr] = simdx_avg / runs;
		simdy_average_times[ctr] = simdy_avg / runs;

        // Free allocated memory
        free(a);
        free(b);
        free(c);
        free(d);
        free(sanity_check);
    }

    
    printf("\n\n------------| Average time for each vector size and each kernel |----------------\n");
    printf("\n%-20s %-10s %-12s %-12s %-12s\n",
        "Vector Size", "C (ms)", "x86-64 (ms)", "SIMD XMM (ms)", "SIMD YMM (ms)");
    printf("--------------------------------------------------------------------------\n");

    for (int ctr = 0; ctr < n_count; ctr++) {
        printf("2^%-17d %-12.3f %-12.3f %-12.3f %-12.3f\n",
            n_sizes[ctr],
            c_average_times[ctr],
            x86_64_average_times[ctr],
            simdx_average_times[ctr],
            simdy_average_times[ctr]);
    }

    return 0;
    
}