# CSC612M SIMD Programming Project: Vector Triads
G01 Francis Bawa, Enrique Lejano, Luis Roxas

## Code Documentation

**Input**
- `ARRAY_SIZE` = length of vectors. 
- Arrays `b`, `c`, `d` containing single-precision floating point numbers.

The values for each vector are initialized using the following code block:

<img width="481" height="173" alt="vector initialization" src="https://github.com/user-attachments/assets/ad50a378-36f7-487f-97cd-5bb1c7f465f1" />

Following the formulas above, the vectors have the following first few values (rounded to 4 decimal places):
- `b` = [0.5, 0.5400, 0.5998, ...]
- `c` = [0,  1.9900, 1.9601, ...]
- `d` = [0,  0.0200, 0.0400,...]

**Process**
- `a[i] = b[i] + c[i] * d[i]`
<img width="509" height="105" alt="vector triad " src="https://github.com/user-attachments/assets/c1fc3061-3255-4dbd-af00-f0ea95c9dd2e" />

**Output**
- Results of each vector triad to be stored in array `a`.

## Output Documentation

### Average Execution Times (Debug)
<img width="748" height="163" alt="a 7" src="https://github.com/user-attachments/assets/683317a0-e2a2-46de-a834-aee36564eff4" />

### Average Execution Times (Release)
<img width="754" height="162" alt="a 7" src="https://github.com/user-attachments/assets/79b3797a-f550-4472-8135-d2c53306613e" />

### Correctness Check (C)

### Correctness Check (SIMD XMM Register)

### Correctness Check (SIMD YMM Register)

### Boundary Check

### Performance Comparison: Debug vs Release

| **Vector Size** | **Kernel** | **Debug (ms)** | **Release (ms)** | **Differences (Debug to Release)** |
|-----------------|-------------|----------------|------------------|-------------------------------|
| **2^20** | C | 2.760 | 0.851 | **3.24× faster** |
| | x86-64 | 1.840 | 1.666 | **1.10× faster** |
| | SIMD XMM | 0.622 | 0.806 | **0.77× slower** |
| | SIMD YMM | 0.569 | 0.740 | **0.77× slower** |
| **2^26** | C | 177.312 | 54.040 | **3.28× faster** |
| | x86-64 | 118.787 | 107.312 | **1.11× faster** |
| | SIMD XMM | 53.001 | 53.159 | **negligible difference** |
| | SIMD YMM | 51.479 | 51.922 | **negligible difference** |
| **2^28** | C | 734.636 | 235.134 | **3.12× faster** |
| | x86-64 | 502.906 | 441.932 | **1.14× faster** |
| | SIMD XMM | 205.155 | 232.834 | **0.88× slower** |
| | SIMD YMM | 201.073 | 223.749 | **0.90× slower** |

### Performance Observations

TODO: Replace with Luis explanations

- **C kernel:** Gains the most from Release mode (3× faster across all array lengths).  
- **x86-64 kernel:** Shows decent improvements (10–15% faster).  
- **SIMD kernels (XMM/YMM):** Performance is nearly identical or slightly slower in Release mode, indicating that SIMD code is already highly optimized.

## Discussion

- [ ] Problems Encountered
- [ ] Solutions Made
- [ ] Unique Methodology
- [ ] AHA 
	- [ ] Big improvement from C to YMM
	- [ ] memory bound or compute bound?
	- [ ] debug versus release
	- [ ] simd ymm faster than optimized C?
