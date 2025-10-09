# CSC612M SIMD Programming Project: Vector Triads
G01 Francis Bawa, Enrique Lejano, Luis Roxas

## Given Problem

Implement the vector triad operation using (1) C program; (2) an x86-64 assembly language; (3) x86-64 SIMD AVX2 assembly language using XMM register; (4) x86-64 SIMD AVX2 assembly language using YMM register.

The formula for the vector triad operation is defined as:

`a[i] = b[i] + c[i] * d[i]`

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

## Output and Correctness Check
To perform the correctness check, we first stored the outputs of the C kernel in an array `sanity_check`. This served as the ground truth for the rest of the kernels. Then, when each kernel completes their calculations, each element in their array `a` is compared with the corresponding indexed element in `sanity_check`. The result of this check is reflected in either a `PASSED` or `FAILED` output.

In all the screenshots below, the correctness checks passed. This can be verified by inspecting the first and last five elements of all the output arrays. 

For readability's sake, the _printing_ of the `PASSED` or `FAILED` output, along with the first and last five elements, is only done on the first of the thirty iterations.

A snippet of the correctness check code is seen below, followed by outputs during the run.
<img width="414" height="161" alt="image" src="https://github.com/user-attachments/assets/e3e449c6-1126-4c5b-aa15-f37e48561929" />

#### C from Debug
 
<img width="409" height="349" alt="c 1" src="https://github.com/user-attachments/assets/36e51bc9-34d2-4134-a82a-86ee556628d8" />

#### x86-64 from Debu)
<img width="613" height="356" alt="d 1" src="https://github.com/user-attachments/assets/5ee644e0-2244-432a-bbb8-948d2a8e6047" />

#### SIMD XMM Register from Debug
<img width="553" height="346" alt="e 1" src="https://github.com/user-attachments/assets/ef21b21f-8557-43ed-aa3d-f84ac9f970af" />

### SIMD YMM Register from Debug
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

It can be observed that the decrease in execution time going from C to x86-64 to SIMD using XMM and then finally SIMD using YMM. The execution times from C to SIMD using the XMM and YMM registers saw more than a 3x improvement for both Debug and Release modes. Therefore, we can conclude that SIMD YMM (and assembly implementation in general) is much faster than optimized C.

**C Implementation:**
AHA! The C code exhibits significant performance improvements in Release mode, with speedups of up to 3.28 times across the various vector sizes. This shows the significant improvements Visual Studio’s compiler can make when optimizations are enabled. In Debug mode, the compiler preserves the code structure for debugging purposes and disables these optimizations. For instance, the stack trace may be made visible in the Debug mode, while Release mode doesn’t have that option.

**Assembly Implementations:**
The assembly implementations (x86-64, SIMD XMM and YMM) show nearly identical performance between Debug and Release modes, with very slight performance differences. This is to be expected, as the assembly code isn’t included in Visual Studio’s optimization pipeline and is built directly as it was written. Of course, the build configuration may affect how the assembler processes the code, but it doesn’t apply the same optimizations as the C counterparts. Regardless of build mode, both SIMD implementations remain faster than the C version.​​​​​​​​​​​​​​​​

### Boundary Check

#### SIMD with XMM Registers Boundary Check (by adding 3 extra elements)
<img width="1068" height="168" alt="g 1" src="https://github.com/user-attachments/assets/2684ef81-ca3e-4c29-8506-49f1481684da" />
<img width="557" height="725" alt="g 2" src="https://github.com/user-attachments/assets/443f8f4e-d5f3-411d-8f7d-a4a76ea7bdb3" />

#### SIMD with YMM Registers Boundary Check (by adding extra 7 elements)
<img width="1062" height="158" alt="g 3" src="https://github.com/user-attachments/assets/bc03ad81-9741-4c2b-8bea-e6543aa296e2" />
<img width="573" height="716" alt="g 4" src="https://github.com/user-attachments/assets/75c0483c-7957-4997-8793-040285a11396" />

## Discussion
While programming the Vector Triad in C was simple, since we only had to copy/paste the formula into the function, we encountered multiple problems and hiccups while developing the other kernel versions. The first problem we encountered was optimizing our code to use the proper addressing modes taught in class, instead of incrementing each register manually at the end of the loop to move on to the next set of values. We kept running into bugs because our calculations were off until we figured out the proper values to increment the memory address for x86-64, SIMD with XMM registers, and SIMD with YMM registers. An AHA moment for us happened while implementing the SIMD with YMM registers because we realized the only thing we had to change was the registers from XMM to YMM, and the value of the register that handles the addressing of our memory. 

The next problem we encountered was realizing that some registers we were using in our code were callee-saved. This meant that the values stored in these registers needed to be preserved across calls, and if we were ever going to use them, we needed to push and pop these registers. To avoid this problem, we simply used other registers that were volatile (caller-saved) since these registers only hold temporary information and can easily be overwritten. The next challenge we encountered was for our SIMD with XMM registers and SIMD with YMM registers implementations. For these implementations, if the value of the vector size had extra elements, it would lead to remainders that had to be addressed independently. Our first solution to handle the boundary conditions was to simply add 3 or 7 to the total number of elements in our XMM and YMM implementations, respectively, so that the program would simply do one extra calculation. However, this caused multiple crashes because the program was writing to memory addresses beyond the allocated array size. To solve this problem, we instead implemented one more loop after the main loop that goes through each remainder one by one and solved these independently.

The final problem we encountered was when we were about to run our main program in RELEASE mode, and encountered a crash despite the program running smoothly in DEBUG mode. After multiple runs, sometimes the program compiled successfully but with garbage values, and sometimes it simply exited the program with an error. We realized that the fix to our problem was to initialize our output (a[n]) and sanity_check array with 0.0, because these garbage values affected the results of our program.
