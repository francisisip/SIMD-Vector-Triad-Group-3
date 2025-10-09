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

### Execution Times

### Correctness Check (C)

### Correctness Check (SIMD XMM Register)

### Correctness Check (SIMD YMM Register)

### Boundary Check

## Discussion

- [ ] Problems Encountered
- [ ] Solutions Made
- [ ] Unique Methodology
- [ ] AHA moments
	- [ ] memory bound or compute bound?
	- [ ] debug versus release
	- [ ] simd ymm faster than optimized C?
