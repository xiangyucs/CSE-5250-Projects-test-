#include "CSE5250intrin.h"
#include "logger.h"

//******************
//* Implementation *
//******************

__cse5250_mask _cse5250_init_ones(int first) {
  __cse5250_mask mask;
  for (int i=0; i<VECTOR_WIDTH; i++) {
    mask.value[i] = (i<first) ? true : false;
  }
  return mask;
}

__cse5250_mask _cse5250_mask_not(__cse5250_mask &maska) {
  __cse5250_mask resultMask;
  for (int i=0; i<VECTOR_WIDTH; i++) {
    resultMask.value[i] = !maska.value[i];
  }
  CSE5250Logger.addLog("masknot", _cse5250_init_ones(), VECTOR_WIDTH);
  return resultMask;
}

__cse5250_mask _cse5250_mask_or(__cse5250_mask &maska, __cse5250_mask &maskb) {
  __cse5250_mask resultMask;
  for (int i=0; i<VECTOR_WIDTH; i++) {
    resultMask.value[i] = maska.value[i] | maskb.value[i];
  }
  CSE5250Logger.addLog("maskor", _cse5250_init_ones(), VECTOR_WIDTH);
  return resultMask;
}

__cse5250_mask _cse5250_mask_and(__cse5250_mask &maska, __cse5250_mask &maskb) {
  __cse5250_mask resultMask;
  for (int i=0; i<VECTOR_WIDTH; i++) {
    resultMask.value[i] = maska.value[i] && maskb.value[i];
  }
  CSE5250Logger.addLog("maskand", _cse5250_init_ones(), VECTOR_WIDTH);
  return resultMask;
}

int _cse5250_cntbits(__cse5250_mask &maska) {
  int count = 0;
  for (int i=0; i<VECTOR_WIDTH; i++) {
    if (maska.value[i]) count++;
  }
  CSE5250Logger.addLog("cntbits", _cse5250_init_ones(), VECTOR_WIDTH);
  return count;
}

template <typename T>
void _cse5250_vset(__cse5250_vec<T> &vecResult, T value, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    vecResult.value[i] = mask.value[i] ? value : vecResult.value[i];
  }
  CSE5250Logger.addLog("vset", mask, VECTOR_WIDTH);
}

template void _cse5250_vset<float>(__cse5250_vec_float &vecResult, float value, __cse5250_mask &mask);
template void _cse5250_vset<int>(__cse5250_vec_int &vecResult, int value, __cse5250_mask &mask);

void _cse5250_vset_float(__cse5250_vec_float &vecResult, float value, __cse5250_mask &mask) { _cse5250_vset<float>(vecResult, value, mask); }
void _cse5250_vset_int(__cse5250_vec_int &vecResult, int value, __cse5250_mask &mask) { _cse5250_vset<int>(vecResult, value, mask); }

__cse5250_vec_float _cse5250_vset_float(float value) {
  __cse5250_vec_float vecResult;
  __cse5250_mask mask = _cse5250_init_ones();
  _cse5250_vset_float(vecResult, value, mask);
  return vecResult;
}
__cse5250_vec_int _cse5250_vset_int(int value) {
  __cse5250_vec_int vecResult;
  __cse5250_mask mask = _cse5250_init_ones();
  _cse5250_vset_int(vecResult, value, mask);
  return vecResult;
}

template <typename T>
void _cse5250_vmove(__cse5250_vec<T> &dest, __cse5250_vec<T> &src, __cse5250_mask &mask) {
    for (int i = 0; i < VECTOR_WIDTH; i++) {
        dest.value[i] = mask.value[i] ? src.value[i] : dest.value[i];
    }
    CSE5250Logger.addLog("vmove", mask, VECTOR_WIDTH);
}

template void _cse5250_vmove<float>(__cse5250_vec_float &dest, __cse5250_vec_float &src, __cse5250_mask &mask);
template void _cse5250_vmove<int>(__cse5250_vec_int &dest, __cse5250_vec_int &src, __cse5250_mask &mask);

void _cse5250_vmove_float(__cse5250_vec_float &dest, __cse5250_vec_float &src, __cse5250_mask &mask) { _cse5250_vmove<float>(dest, src, mask); }
void _cse5250_vmove_int(__cse5250_vec_int &dest, __cse5250_vec_int &src, __cse5250_mask &mask) { _cse5250_vmove<int>(dest, src, mask); }

template <typename T>
void _cse5250_vload(__cse5250_vec<T> &dest, T* src, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    dest.value[i] = mask.value[i] ? src[i] : dest.value[i];
  }
  CSE5250Logger.addLog("vload", mask, VECTOR_WIDTH);
}

template void _cse5250_vload<float>(__cse5250_vec_float &dest, float* src, __cse5250_mask &mask);
template void _cse5250_vload<int>(__cse5250_vec_int &dest, int* src, __cse5250_mask &mask);

void _cse5250_vload_float(__cse5250_vec_float &dest, float* src, __cse5250_mask &mask) { _cse5250_vload<float>(dest, src, mask); }
void _cse5250_vload_int(__cse5250_vec_int &dest, int* src, __cse5250_mask &mask) { _cse5250_vload<int>(dest, src, mask); }

template <typename T>
void _cse5250_vstore(T* dest, __cse5250_vec<T> &src, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    dest[i] = mask.value[i] ? src.value[i] : dest[i];
  }
  CSE5250Logger.addLog("vstore", mask, VECTOR_WIDTH);
}

template void _cse5250_vstore<float>(float* dest, __cse5250_vec_float &src, __cse5250_mask &mask);
template void _cse5250_vstore<int>(int* dest, __cse5250_vec_int &src, __cse5250_mask &mask);

void _cse5250_vstore_float(float* dest, __cse5250_vec_float &src, __cse5250_mask &mask) { _cse5250_vstore<float>(dest, src, mask); }
void _cse5250_vstore_int(int* dest, __cse5250_vec_int &src, __cse5250_mask &mask) { _cse5250_vstore<int>(dest, src, mask); }

template <typename T>
void _cse5250_vadd(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    vecResult.value[i] = mask.value[i] ? (veca.value[i] + vecb.value[i]) : vecResult.value[i];
  }
  CSE5250Logger.addLog("vadd", mask, VECTOR_WIDTH);
}

template void _cse5250_vadd<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_vadd<int>(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_vadd_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_vadd<float>(vecResult, veca, vecb, mask); }
void _cse5250_vadd_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_vadd<int>(vecResult, veca, vecb, mask); }

template <typename T>
void _cse5250_vsub(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    vecResult.value[i] = mask.value[i] ? (veca.value[i] - vecb.value[i]) : vecResult.value[i];
  }
  CSE5250Logger.addLog("vsub", mask, VECTOR_WIDTH);
}

template void _cse5250_vsub<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_vsub<int>(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_vsub_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_vsub<float>(vecResult, veca, vecb, mask); }
void _cse5250_vsub_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_vsub<int>(vecResult, veca, vecb, mask); }

template <typename T>
void _cse5250_vmult(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    vecResult.value[i] = mask.value[i] ? (veca.value[i] * vecb.value[i]) : vecResult.value[i];
  }
  CSE5250Logger.addLog("vmult", mask, VECTOR_WIDTH);
}

template void _cse5250_vmult<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_vmult<int>(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_vmult_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_vmult<float>(vecResult, veca, vecb, mask); }
void _cse5250_vmult_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_vmult<int>(vecResult, veca, vecb, mask); }

template <typename T>
void _cse5250_vdiv(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    vecResult.value[i] = mask.value[i] ? (veca.value[i] / vecb.value[i]) : vecResult.value[i];
  }
  CSE5250Logger.addLog("vdiv", mask, VECTOR_WIDTH);
}

template void _cse5250_vdiv<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_vdiv<int>(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_vdiv_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_vdiv<float>(vecResult, veca, vecb, mask); }
void _cse5250_vdiv_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_vdiv<int>(vecResult, veca, vecb, mask); }

template <typename T>
void _cse5250_vabs(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &veca, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    vecResult.value[i] = mask.value[i] ? (abs(veca.value[i])) : vecResult.value[i];
  }
  CSE5250Logger.addLog("vabs", mask, VECTOR_WIDTH);
}

template void _cse5250_vabs<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_mask &mask);
template void _cse5250_vabs<int>(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_mask &mask);

void _cse5250_vabs_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &veca, __cse5250_mask &mask) { _cse5250_vabs<float>(vecResult, veca, mask); }
void _cse5250_vabs_int(__cse5250_vec_int &vecResult, __cse5250_vec_int &veca, __cse5250_mask &mask) { _cse5250_vabs<int>(vecResult, veca, mask); }

template <typename T>
void _cse5250_vgt(__cse5250_mask &maskResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    maskResult.value[i] = mask.value[i] ? (veca.value[i] > vecb.value[i]) : maskResult.value[i];
  }
  CSE5250Logger.addLog("vgt", mask, VECTOR_WIDTH);
}

template void _cse5250_vgt<float>(__cse5250_mask &maskResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_vgt<int>(__cse5250_mask &maskResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_vgt_float(__cse5250_mask &maskResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_vgt<float>(maskResult, veca, vecb, mask); }
void _cse5250_vgt_int(__cse5250_mask &maskResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_vgt<int>(maskResult, veca, vecb, mask); }

template <typename T>
void _cse5250_vlt(__cse5250_mask &maskResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    maskResult.value[i] = mask.value[i] ? (veca.value[i] < vecb.value[i]) : maskResult.value[i];
  }
  CSE5250Logger.addLog("vlt", mask, VECTOR_WIDTH);
}

template void _cse5250_vlt<float>(__cse5250_mask &maskResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_vlt<int>(__cse5250_mask &maskResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_vlt_float(__cse5250_mask &maskResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_vlt<float>(maskResult, veca, vecb, mask); }
void _cse5250_vlt_int(__cse5250_mask &maskResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_vlt<int>(maskResult, veca, vecb, mask); }

template <typename T>
void _cse5250_veq(__cse5250_mask &maskResult, __cse5250_vec<T> &veca, __cse5250_vec<T> &vecb, __cse5250_mask &mask) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    maskResult.value[i] = mask.value[i] ? (veca.value[i] == vecb.value[i]) : maskResult.value[i];
  }
  CSE5250Logger.addLog("veq", mask, VECTOR_WIDTH);
}

template void _cse5250_veq<float>(__cse5250_mask &maskResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask);
template void _cse5250_veq<int>(__cse5250_mask &maskResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask);

void _cse5250_veq_float(__cse5250_mask &maskResult, __cse5250_vec_float &veca, __cse5250_vec_float &vecb, __cse5250_mask &mask) { _cse5250_veq<float>(maskResult, veca, vecb, mask); }
void _cse5250_veq_int(__cse5250_mask &maskResult, __cse5250_vec_int &veca, __cse5250_vec_int &vecb, __cse5250_mask &mask) { _cse5250_veq<int>(maskResult, veca, vecb, mask); }

template <typename T>
void _cse5250_hadd(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &vec) {
  for (int i=0; i<VECTOR_WIDTH/2; i++) {
    T result = vec.value[2*i] + vec.value[2*i+1];
    vecResult.value[2 * i] = result;
    vecResult.value[2 * i + 1] = result;
  }
}

template void _cse5250_hadd<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &vec);

void _cse5250_hadd_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &vec) { _cse5250_hadd<float>(vecResult, vec); }

template <typename T>
void _cse5250_interleave(__cse5250_vec<T> &vecResult, __cse5250_vec<T> &vec) {
  for (int i=0; i<VECTOR_WIDTH; i++) {
    int index = i < VECTOR_WIDTH/2 ? (2 * i) : (2 * (i - VECTOR_WIDTH/2) + 1);
    vecResult.value[i] = vec.value[index];
  }
}

template void _cse5250_interleave<float>(__cse5250_vec_float &vecResult, __cse5250_vec_float &vec);

void _cse5250_interleave_float(__cse5250_vec_float &vecResult, __cse5250_vec_float &vec) { _cse5250_interleave<float>(vecResult, vec); }

void addUserLog(const char * logStr) {
  CSE5250Logger.addLog(logStr, _cse5250_init_ones(), 0);
}

