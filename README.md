# CSC612M SIMD Programming Project: Vector Triads
G01 Francis Bawa, Enrique Lejano, Luis Roxas

## Code Documentation

**Input**
- `ARRAY_SIZE` = length of vectors. 
- Arrays `b`, `c`, `d` containing single-precision floating point numbers.

Vector value initialization logic:

<img width="481" height="173" alt="vector initialization" src="https://github.com/user-attachments/assets/ad50a378-36f7-487f-97cd-5bb1c7f465f1" />

Each vector (`b`, `c`, `d`) is initialized using trigonometric expressions to create oscillating and non-linear data patterns. These ensure that computations involve realistic and non-constant values for benchmarking.

| **Term** | **Range** | **Reason** |
|-----------|------------|------------|
| `sin(i * 0.05)` | `[-1, 1]` | Sine function oscillates between -1 and 1 |
| `+ 0.5` | shifts range to `[-0.5, 1.5]` | Adds offset; `b[i]` can be slightly negative |
| `cos(i * 0.1) * 2.0` | `[-2, 2]` | Cosine oscillates and is scaled by 2 |
| `sin(i * 0.02) * cos(i * 0.03)` | approximately `[-1, 1]` | Product of two oscillating trigonometric functions |

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

### Snippet of complete execution times (from Debug)
<img width="576" height="702" alt="a 4" src="https://github.com/user-attachments/assets/a5acdfb2-9ce2-400d-8daa-11c266b2299f" />

### Output and Correctness Check (C from Debug)
<img width="409" height="349" alt="c 1" src="https://github.com/user-attachments/assets/36e51bc9-34d2-4134-a82a-86ee556628d8" />

### Output and Correctness Check (x86-64 from Debug)
<img width="613" height="356" alt="d 1" src="https://github.com/user-attachments/assets/5ee644e0-2244-432a-bbb8-948d2a8e6047" />

### Output and Correctness Check (SIMD XMM Register from Debug)
<img width="553" height="346" alt="e 1" src="https://github.com/user-attachments/assets/ef21b21f-8557-43ed-aa3d-f84ac9f970af" />

### Output and Correctness Check (SIMD YMM Register from Debug)
<img width="550" height="359" alt="f 1" src="https://github.com/user-attachments/assets/8cafb684-8b8a-4779-b972-af0ef723c274" />

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

### Boundary Check

#### SIMD with XMM Registers Boundary Check (by adding 3 extra elements)
<img width="1068" height="168" alt="g 1" src="https://github.com/user-attachments/assets/2684ef81-ca3e-4c29-8506-49f1481684da" />
<img width="557" height="725" alt="g 2" src="https://github.com/user-attachments/assets/443f8f4e-d5f3-411d-8f7d-a4a76ea7bdb3" />

#### SIMD with YMM Registers Boundary Check (by adding extra 7 elements)
<img width="1062" height="158" alt="g 3" src="https://github.com/user-attachments/assets/bc03ad81-9741-4c2b-8bea-e6543aa296e2" />
<img width="573" height="716" alt="g 4" src="https://github.com/user-attachments/assets/75c0483c-7957-4997-8793-040285a11396" />


## Discussion
- [ ] Output and correctness check that we're showing is for 2^20. ON the other hand, we use 2^26 and 2^28 to do the boundary checks.
- [ ] How we did correctness check: Add explanation that we used C as the ground truth. Stored in sanity_check array. Then all other kernel versions' arrays are compared agianst sanity_check.
- [ ] Problems Encountered
- [ ] Solutions Made
- [ ] Unique Methodology
- [ ] AHA 
	- [ ] Big improvement from C to YMM
	- [ ] memory bound or compute bound?
	- [ ] debug versus release
	- [ ] simd ymm faster than optimized C?
- [ ] B
