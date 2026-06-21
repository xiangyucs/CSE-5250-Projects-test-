// Define vector unit width here
#define VECTOR_WIDTH 4

#ifndef CSE5250INTRIN_H_
#define CSE5250INTRIN_H_

#include <cstdlib>
#include <cmath>
#include "logger.h"

//*******************
//* Type Definition *
//*******************

extern Logger CSE5250Logger;

template <typename T>
struct __cse5250_vec {
  T value[VECTOR_WIDTH];
};

// Declare a mask with __cse5250_mask
struct __cse5250_mask : __cse5250_vec<bool> {};

// Declare a floating point vector register with __cse5250_vec_float
#define __cse5250_vec_float __cse5250_vec<float>

// Declare an integer vector register with __cse5250_vec_int
#define __cse5250_vec_int   __cse5250_vec<int>

//***********************
//* Function Definition *
//***********************

// Return a mask initialized to 1 in the first N lanes and 0 in the others
__cse5250_mask _cse5250_init_ones(int first = VECTOR_WIDTH);

// Return the inverse of maska
__cse5250_mask _cse5250_mask_not(__cse5250_mask &maska);

// Return (maska | maskb)
__cse5250_mask _cse5250_mask_or(__cse5250_mask &maska, __cse5250_mask &maskb);

// Return (maska & maskb)
__cse5250_mask _cse5250_mask_and(__cse5250_mask &maska, __cse5250_mask &maskb);

// Count the number of 1s in maska
int _cse5250_cntbits(__cse5250_mask &maska);

// Set register to value if vector lane is active
//  otherwise keep the old value
void _cse5250_vset_float(__cse5250_vec_float &vecResult, float value, __cse5250_mask &mask);
void _cse5250_vset_int(__cse5250_vec_int &vecResult, int value, __cse5250_mask &mask);
// For user's convenience, returns a vector register with all lanes initialized to value
__cse5250_vec_float _cse5250_vset_float(float value);
__cse5250_vec_int _cse5250_vset_int(int value);

// Copy values from vector register src to vector register dest if vector lane active
// otherwise keep the old value
void _cse5250_vmove_float(__cse5250_vec_float &dest, __cse5250_vec_float &src, __cse5250_mask &mask);
void _cse5250_vmove_int(__cse5250_vec_int &dest, __cse5250_vec_int &src, __cse5250_mask &mask);

// Load values from array src to vector register dest if vector lane active
//  otherwise keep the old value
void _cse5250_vload_float(__cse5250_vec_float &dest, float* src, __cse5250_mask &mask);
void _cse5250_vload_int(__cse5250_vec_int &dest, int* src, __cse5250_mask &mask);

// Store values from vector register src to array dest if vector lane active
//  otherwise keep the old value
void _cse5250_vstore_float(float* dest, __cse5250_vec_float &src, __cse5250_mask &mask);
void _cse5250_vstore_int(int* dest, __cse5250_vec_int &src, __cse5250_mask &mask);

// Return calculation of (veca + vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_vadd_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_vadd_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

// Return calculation of (veca - vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_vsub_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_vsub_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

// Return calculation of (veca * vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_vmult_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_vmult_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

// Return calculation of (veca / vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_vdiv_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_vdiv_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);


// Return calculation of absolute value abs(veca) if vector lane active
//  otherwise keep the old value
void _cse5250_vabs_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_mask &mask);
void _cse5250_vabs_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_mask &mask);

// Return a mask of (veca > vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_vgt_float(__cse5250_mask &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_vgt_int(__cse5250_mask &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

// Return a mask of (veca < vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_vlt_float(__cse5250_mask &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_vlt_int(__cse5250_mask &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

// Return a mask of (veca == vecb) if vector lane active
//  otherwise keep the old value
void _cse5250_veq_float(__cse5250_mask &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
void _cse5250_veq_int(__cse5250_mask &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

// Adds up adjacent pairs of elements, so
//  [0 1 2 3] -> [0+1 0+1 2+3 2+3]
void _cse5250_hadd_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &vec);

// Performs an even-odd interleaving where all even-indexed elements move to front half
//  of the array and odd-indexed to the back half, so
//  [0 1 2 3 4 5 6 7] -> [0 2 4 6 1 3 5 7]
void _cse5250_interleave_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &vec);

// Add a customized log to help debugging
void addUserLog(const char * logStr);

#endif
