/**
 * @file fft_test.c
 * @brief Comprehensive Test Suite for FFT Implementation
 * 
 * This file contains unit tests and integration tests for the FFT algorithm.
 * Tests cover:
 * - Complex number operations
 * - Bit-reversal permutation
 * - FFT correctness
 * - Edge cases
 * - Mathematical properties (energy conservation, etc.)
 * 
 * @author Test Suite
 * @date 2026-05-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "fft.c"

/**
 * @def EPSILON
 * @brief Tolerance for floating-point comparisons
 */
#define EPSILON 1e-9

/**
 * @brief Global test statistics
 */
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
} TestStats;

TestStats stats = {0, 0, 0};

/**
 * @brief Compares two floating-point numbers with epsilon tolerance
 */
int approx_equal(double a, double b) {
    return fabs(a - b) < EPSILON;
}

/**
 * @brief Compares two complex numbers with epsilon tolerance
 */
int complex_equal(Complex a, Complex b) {
    return approx_equal(a.real, b.real) && approx_equal(a.imag, b.imag);
}

/**
 * @brief Asserts a condition and updates test statistics
 */
void assert_true(int condition, const char *test_name) {
    stats.total_tests++;
    printf("Test %d: %s ... ", stats.total_tests, test_name);
    
    if (condition) {
        stats.passed_tests++;
        printf("✓ PASSED\n");
    } else {
        stats.failed_tests++;
        printf("✗ FAILED\n");
    }
}

/**
 * @brief TEST 1: Complex Number Addition
 */
void test_complex_addition() {
    Complex a = create_complex(3.0, 4.0);
    Complex b = create_complex(1.0, 2.0);
    Complex result = add_complex(a, b);
    Complex expected = create_complex(4.0, 6.0);
    
    assert_true(complex_equal(result, expected), "Complex Addition (3+4i) + (1+2i) = (4+6i)");
}

/**
 * @brief TEST 2: Complex Number Subtraction
 */
void test_complex_subtraction() {
    Complex a = create_complex(5.0, 7.0);
    Complex b = create_complex(2.0, 3.0);
    Complex result = subtract_complex(a, b);
    Complex expected = create_complex(3.0, 4.0);
    
    assert_true(complex_equal(result, expected), "Complex Subtraction (5+7i) - (2+3i) = (3+4i)");
}

/**
 * @brief TEST 3: Complex Number Multiplication
 */
void test_complex_multiplication() {
    Complex a = create_complex(2.0, 3.0);
    Complex b = create_complex(4.0, 5.0);
    Complex result = multiply_complex(a, b);
    Complex expected = create_complex(-7.0, 22.0);
    
    assert_true(complex_equal(result, expected), "Complex Multiplication (2+3i) * (4+5i) = (-7+22i)");
}

/**
 * @brief TEST 4: Complex Magnitude
 */
void test_complex_magnitude() {
    Complex c = create_complex(3.0, 4.0);
    double result = magnitude(c);
    double expected = 5.0;
    
    assert_true(approx_equal(result, expected), "Complex Magnitude |3+4i| = 5");
}

/**
 * @brief TEST 5: Complex Phase
 */
void test_complex_phase() {
    Complex c = create_complex(1.0, 0.0);
    double result = phase(c);
    double expected = 0.0;
    
    assert_true(approx_equal(result, expected), "Complex Phase arg(1+0i) = 0");
}

/**
 * @brief TEST 6: FFT of DC Signal (Constant)
 */
void test_fft_dc_signal() {
    int n = 4;
    Complex input[4] = {
        {1.0, 0.0}, {1.0, 0.0}, {1.0, 0.0}, {1.0, 0.0}
    };
    
    fft(input, n);
    
    int dc_correct = approx_equal(input[0].real, 4.0) && approx_equal(input[0].imag, 0.0);
    
    int others_correct = 1;
    for (int i = 1; i < n; i++) {
        if (!approx_equal(magnitude(input[i]), 0.0)) {
            others_correct = 0;
            break;
        }
    }
    
    assert_true(dc_correct && others_correct, "FFT of DC Signal [1,1,1,1]");
}

/**
 * @brief TEST 7: FFT of Sine Wave
 */
void test_fft_sine_wave() {
    int n = 8;
    Complex input[8];
    
    for (int k = 0; k < n; k++) {
        input[k].real = 0.0;
        input[k].imag = sin(2.0 * PI * k / n);
    }
    
    fft(input, n);
    
    int has_peak = 0;
    for (int i = 0; i < n; i++) {
        if (magnitude(input[i]) > 1.0) {
            has_peak = 1;
            break;
        }
    }
    
    assert_true(has_peak, "FFT of Sine Wave has spectral peaks");
}

/**
 * @brief TEST 8: FFT produces no NaN/Inf
 */
void test_fft_no_nan_inf() {
    int n = 8;
    Complex input[8];
    
    for (int i = 0; i < n; i++) {
        input[i].real = (double)i;
        input[i].imag = 0.0;
    }
    
    fft(input, n);
    
    int valid = 1;
    for (int i = 0; i < n; i++) {
        if (isnan(input[i].real) || isnan(input[i].imag) ||
            isinf(input[i].real) || isinf(input[i].imag)) {
            valid = 0;
            break;
        }
    }
    
    assert_true(valid, "FFT produces no NaN or Inf values");
}

/**
 * @brief TEST 9: FFT Energy Conservation
 */
void test_fft_energy_conservation() {
    int n = 8;
    Complex input[8] = {
        {1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}, {4.0, 0.0},
        {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}
    };
    
    double time_energy = 0.0;
    for (int i = 0; i < n; i++) {
        time_energy += input[i].real * input[i].real + input[i].imag * input[i].imag;
    }
    
    fft(input, n);
    
    double freq_energy = 0.0;
    for (int i = 0; i < n; i++) {
        freq_energy += input[i].real * input[i].real + input[i].imag * input[i].imag;
    }
    freq_energy /= n;
    
    assert_true(approx_equal(time_energy, freq_energy), "FFT Energy Conservation (Parseval's Theorem)");
}

/**
 * @brief TEST 10: FFT Size Powers of 2
 */
void test_fft_power_of_2() {
    int sizes[] = {2, 4, 8, 16, 32, 64};
    int valid = 1;
    
    for (int s = 0; s < 6; s++) {
        int n = sizes[s];
        Complex *input = (Complex *)malloc(n * sizeof(Complex));
        
        for (int i = 0; i < n; i++) {
            input[i].real = 1.0;
            input[i].imag = 0.0;
        }
        
        fft(input, n);
        
        if (!approx_equal(input[0].real, (double)n)) {
            valid = 0;
        }
        
        free(input);
    }
    
    assert_true(valid, "FFT works correctly for all power-of-2 sizes (2 to 64)");
}

/**
 * @brief TEST 11: Bit Reversal Correctness
 */
void test_bit_reversal() {
    int n = 8;
    Complex input[8] = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0},
        {4, 0}, {5, 0}, {6, 0}, {7, 0}
    };
    
    bit_reverse(input, n);
    
    int expected_order[] = {0, 4, 2, 6, 1, 5, 3, 7};
    
    int correct = 1;
    for (int i = 0; i < n; i++) {
        if (!approx_equal(input[i].real, expected_order[i])) {
            correct = 0;
            break;
        }
    }
    
    assert_true(correct, "Bit-Reversal Permutation correct for n=8");
}

/**
 * @brief TEST 12: FFT with Single Non-zero Element
 */
void test_fft_impulse() {
    int n = 8;
    Complex input[8];
    
    for (int i = 0; i < n; i++) {
        input[i].real = 0.0;
        input[i].imag = 0.0;
    }
    input[0].real = 1.0;
    
    fft(input, n);
    
    int valid = 1;
    for (int i = 0; i < n; i++) {
        if (!approx_equal(magnitude(input[i]), 1.0)) {
            valid = 0;
            break;
        }
    }
    
    assert_true(valid, "FFT of Impulse Signal has uniform magnitude");
}

/**
 * @brief TEST 13: Complex Multiplication Commutativity
 */
void test_complex_multiplication_commutative() {
    Complex a = create_complex(3.5, 2.1);
    Complex b = create_complex(1.2, 4.3);
    
    Complex ab = multiply_complex(a, b);
    Complex ba = multiply_complex(b, a);
    
    assert_true(complex_equal(ab, ba), "Complex Multiplication is Commutative");
}

/**
 * @brief TEST 14: Complex Addition Associativity
 */
void test_complex_addition_associative() {
    Complex a = create_complex(1.0, 2.0);
    Complex b = create_complex(3.0, 4.0);
    Complex c = create_complex(5.0, 6.0);
    
    Complex ab_c = add_complex(add_complex(a, b), c);
    Complex a_bc = add_complex(a, add_complex(b, c));
    
    assert_true(complex_equal(ab_c, a_bc), "Complex Addition is Associative");
}

/**
 * @brief TEST 15: FFT of Zeros
 */
void test_fft_all_zeros() {
    int n = 8;
    Complex input[8];
    
    for (int i = 0; i < n; i++) {
        input[i].real = 0.0;
        input[i].imag = 0.0;
    }
    
    fft(input, n);
    
    int all_zero = 1;
    for (int i = 0; i < n; i++) {
        if (!approx_equal(magnitude(input[i]), 0.0)) {
            all_zero = 0;
            break;
        }
    }
    
    assert_true(all_zero, "FFT of all zeros produces all zeros");
}

/**
 * @brief Prints test summary statistics
 */
void print_test_summary() {
    printf("\n");
    printf("================================\n");
    printf("TEST SUMMARY\n");
    printf("================================\n");
    printf("Total Tests:   %d\n", stats.total_tests);
    printf("Passed:        %d ✓\n", stats.passed_tests);
    printf("Failed:        %d ✗\n", stats.failed_tests);
    printf("Success Rate:  %.2f%%\n", 
           (stats.total_tests > 0) ? (100.0 * stats.passed_tests / stats.total_tests) : 0.0);
    printf("================================\n");
}

/**
 * @brief Main test runner function
 */
int main() {
    printf("FFT Implementation Test Suite\n");
    printf("============================\n\n");
    
    printf("--- Complex Number Operations Tests ---\n");
    test_complex_addition();
    test_complex_subtraction();
    test_complex_multiplication();
    test_complex_magnitude();
    test_complex_phase();
    test_complex_multiplication_commutative();
    test_complex_addition_associative();
    
    printf("\n--- FFT Algorithm Tests ---\n");
    test_fft_dc_signal();
    test_fft_sine_wave();
    test_fft_impulse();
    test_fft_all_zeros();
    test_fft_no_nan_inf();
    test_fft_energy_conservation();
    test_fft_power_of_2();
    
    printf("\n--- Utility Function Tests ---\n");
    test_bit_reversal();
    
    print_test_summary();
    
    return stats.failed_tests == 0 ? 0 : 1;
}
