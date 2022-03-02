#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


#include "../../common/vector_defines.h"

void axpy_intrinsics(float a, float *dx, float *dy, int n) {
  int i;

  // long gvl = __builtin_epi_vsetvl(n, __epi_e64, __epi_m1);
  long gvl = vsetvl_e32m1(n); //PLCT
  
  for (i = 0; i < n;) {
    // gvl = __builtin_epi_vsetvl(n - i, __epi_e64, __epi_m1);
    gvl = vsetvl_e32m1(n - i); //PLCT

    _MMR_f32 v_dx = _MM_LOAD_f32(&dx[i], gvl);
    _MMR_f32 v_dy = _MM_LOAD_f32(&dy[i], gvl);
    _MMR_f32 v_res = vfmacc_vf_f32m1(v_dy, a, v_dx, gvl);
    _MM_STORE_f32(&dy[i], v_res, gvl);
	
    i += gvl;
  }

FENCE();
}
