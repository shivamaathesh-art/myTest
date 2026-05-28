/**
 * @file fft.c
 * @brief Fast Fourier Transform (FFT) Implementation
 * 
 * This file implements the Cooley-Tukey FFT algorithm for computing
 * the Discrete Fourier Transform (DFT) of a sequence of complex numbers.
 * The algorithm has a time complexity of O(n log n) compared to the
 * naive DFT approach which is O(n^2).
 * 
 * @author FFT Implementation
 * @date 2026-05-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

/**
 * @def PI
 * @brief Mathematical constant PI used in FFT calculations
 */
#define PI 3.14159265358979323846

/**
 * @struct Complex
 * @brief Structure to represent complex numbers
 * @param real Real part of the complex number
 * @param imag Imaginary part of the complex number
 */
typedef struct {
    double real;
    double imag;
} Complex;

/**
 * @brief Creates a complex number from real and imaginary parts
 * 
 * @param real The real part of the complex number
 * @param imag The imaginary part of the complex number
 * @return Complex number with specified real and imaginary parts
 */
Complex create_complex(double real, double imag) {
    Complex c;
    c.real = real;
    c.imag = imag;
    return c;
}

/**
 * @brief Adds two complex numbers
 * 
 * @param a First complex number
 * @param b Second complex number
 * @return Sum of the two complex numbers (a + b)
 */
Complex add_complex(Complex a, Complex b) {
    return create_complex(a.real + b.real, a.imag + b.imag);
}

/**
 * @brief Subtracts two complex numbers
 * 
 * @param a First complex number (minuend)
 * @param b Second complex number (subtrahend)
 * @return Difference of the two complex numbers (a - b)
 */
Complex subtract_complex(Complex a, Complex b) {
    return create_complex(a.real - b.real, a.imag - b.imag);
}

/**
 * @brief Multiplies two complex numbers
 * 
 * Formula: (a + bi)(c + di) = (ac - bd) + (ad + bc)i
 * 
 * @param a First complex number
 * @param b Second complex number
 * @return Product of the two complex numbers (a * b)
 */
Complex multiply_complex(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

/**
 * @brief Performs Bit-Reversal Permutation on the input array
 * 
 * This is a required preprocessing step for in-place FFT computation.
 * It reorders the elements of the array so that their positions follow
 * a bit-reversed pattern.
 * 
 * @param arr Array of complex numbers to be bit-reversed
 * @param n Size of the array (must be a power of 2)
 */
void bit_reverse(Complex *arr, int n) {
    int j = 0;
    for (int i = 0; i < n - 1; i++) {
        if (i < j) {
            Complex temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        int k = n / 2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}

/**
 * @brief Computes the Fast Fourier Transform using Cooley-Tukey algorithm
 * 
 * This function performs an in-place FFT on the input array of complex numbers.
 * The algorithm recursively divides the problem into even and odd indexed elements,
 * significantly reducing computational complexity.
 * 
 * Time Complexity: O(n log n) where n is the size of the input array
 * Space Complexity: O(n) including the call stack
 * 
 * @param arr Input array of complex numbers (modified in place)
 * @param n Size of the array (must be a power of 2)
 * 
 * @note The input size n must be a power of 2 (2, 4, 8, 16, 32, etc.)
 * @note The function modifies the input array in place
 * @note Bit-reversal permutation is applied internally
 */
void fft(Complex *arr, int n) {
    // Bit-reversal permutation
    bit_reverse(arr, n);
    
    // Iterative FFT computation
    for (int len = 2; len <= n; len *= 2) {
        // Calculate the angle increment for current level
        double angle = 2.0 * PI / len;
        
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < len / 2; j++) {
                // Calculate twiddle factor: W_N^k = e^(-i*2*pi*k/N)
                double theta = angle * j;
                Complex w;
                w.real = cos(theta);
                w.imag = -sin(theta);
                
                // Butterfly operation
                Complex u = arr[i + j];
                Complex v = multiply_complex(w, arr[i + j + len / 2]);
                
                arr[i + j] = add_complex(u, v);
                arr[i + j + len / 2] = subtract_complex(u, v);
            }
        }
    }
}

/**
 * @brief Computes the magnitude (absolute value) of a complex number
 * 
 * @param c Complex number
 * @return Magnitude of the complex number: sqrt(real^2 + imag^2)
 */
double magnitude(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}

/**
 * @brief Computes the phase (angle) of a complex number in radians
 * 
 * @param c Complex number
 * @return Phase angle of the complex number: atan2(imag, real)
 */
double phase(Complex c) {
    return atan2(c.imag, c.real);
}

/**
 * @brief Prints a complex number in the format (real + i*imag)
 * 
 * @param c Complex number to be printed
 */
void print_complex(Complex c) {
    printf("(%.4f %+.4fi)", c.real, c.imag);
}

/**
 * @brief Main function demonstrating FFT usage
 * 
 * Example: Computes FFT of a simple input sequence
 * Input: [1, 2, 3, 4, 0, 0, 0, 0]
 * 
 * @return 0 on successful execution
 */
int main() {
    // Example: FFT of size 8 (must be power of 2)
    int n = 8;
    Complex *input = (Complex *)malloc(n * sizeof(Complex));
    
    // Initialize input signal
    for (int i = 0; i < n; i++) {
        input[i].real = (i < 4) ? (i + 1) : 0;  // [1, 2, 3, 4, 0, 0, 0, 0]
        input[i].imag = 0;
    }
    
    printf("Input Signal:\n");
    for (int i = 0; i < n; i++) {
        printf("[%d]: ", i);
        print_complex(input[i]);
        printf("\n");
    }
    
    // Perform FFT
    fft(input, n);
    
    printf("\nFFT Output (Frequency Domain):\n");
    for (int i = 0; i < n; i++) {
        printf("[%d]: ", i);
        print_complex(input[i]);
        printf(" | Magnitude: %.4f, Phase: %.4f rad\n", magnitude(input[i]), phase(input[i]));
    }
    
    // Free allocated memory
    free(input);
    
    return 0;
}
