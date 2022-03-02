//
// RISC-V VECTOR COSINE FUNCTION Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
// This RISC-V Vector implementation is based on the original code presented by Julien Pommier

/* 
   Implementation of sin, cos, sincos, exp and log

   Based on "sse_mathfun.h", by Julien Pommier
   http://gruntthepeon.free.fr/ssemath/

   Copyright (C) 2012 Giovanni Garberoglio
   Interdisciplinary Laboratory for Computational Science (LISC)
   Fondazione Bruno Kessler and University of Trento
   via Sommarive, 18
   I-38123 Trento (Italy)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  (this is the zlib license)
*/


inline _MMR_f32 __cos_1xf32(_MMR_f32 x , unsigned long int gvl) {

_MMR_i32   _ps_inv_sign_mask    = _MM_SET_i32(~0x80000000,gvl);
_MMR_f32   _ps_cephes_FOPI      = _MM_SET_f32(1.27323954473516,gvl); // 4 / M_PI
_MMR_i32   _pi32_1              = _MM_SET_i32(0x00000001,gvl);
_MMR_i32   _pi32_inv1              = _MM_SET_i32(~0x00000001,gvl);
_MMR_i32   _pi32_2              = _MM_SET_i32(2,gvl);
_MMR_i32   _pi32_4              = _MM_SET_i32(4,gvl);
_MMR_i32   _Zero                = _MM_SET_i32(0,gvl);

_MMR_f32   xmm2 =  _MM_SET_f32(0.0f,gvl);
_MMR_f32   xmm1;
_MMR_f32   xmm3;
_MMR_f32   y;

_MMR_i32   emm0;
_MMR_i32   emm2;

_MMR_MASK_i32 xMask;
  /* take the absolute value */
  x = vreinterpret_v_i32m1_f32m1(_MM_AND_i32(vreinterpret_v_f32m1_i32m1(x), _ps_inv_sign_mask,gvl));

  /* scale by 4/Pi */
  y = _MM_MUL_f32(x, _ps_cephes_FOPI,gvl);

  /* store the integer part of y in mm0 */
  emm2 = _MM_VFCVT_X_F_i32(y,gvl);

  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = _MM_ADD_i32(emm2,_pi32_1,gvl);
  emm2 = _MM_AND_i32(emm2, _pi32_inv1,gvl);
  y = _MM_VFCVT_F_X_f32(emm2,gvl);

  emm2 = _MM_SUB_i32(emm2, _pi32_2,gvl);

  /* get the swap sign flag */
  emm0 = _MM_XOR_i32(emm2, _MM_SET_i32(0xffffffff,gvl),gvl);
  emm0 = _MM_AND_i32(emm0, _pi32_4,gvl);

  emm0 = _MM_SLL_i32(emm0, vreinterpret_v_i32m1_u32m1(_MM_SET_i32(29,gvl)),gvl);

  /* get the polynom selection mask */
  emm2 = _MM_AND_i32(emm2, _pi32_2 ,gvl);
  xMask= _MM_VMSEQ_i32(emm2,_Zero,gvl);
  emm2 = _MM_MERGE_i32(_Zero,_MM_SET_i32(0xffffffff,gvl), xMask,gvl);

  _MMR_f32 sign_bit =  vreinterpret_v_i32m1_f32m1(emm0);
  _MMR_f32 poly_mask =  vreinterpret_v_i32m1_f32m1(emm2);

  /* The magic pass: "Extended precision modular arithmetic"
     x = ((x - y * DP1) - y * DP2) - y * DP3; */

  _MMR_f32   _ps_minus_cephes_DP1    = _MM_SET_f32(-0.78515625,gvl);
  _MMR_f32   _ps_minus_cephes_DP2    = _MM_SET_f32(-2.4187564849853515625E-4,gvl);
  _MMR_f32   _ps_minus_cephes_DP3    = _MM_SET_f32(-3.77489497744594108E-8,gvl);

  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;

  xmm1 = _MM_MUL_f32(y, xmm1,gvl);
  xmm2 = _MM_MUL_f32(y, xmm2,gvl);
  xmm3 = _MM_MUL_f32(y, xmm3,gvl);

  x = _MM_ADD_f32(x, xmm1,gvl);
  x = _MM_ADD_f32(x, xmm2,gvl);
  x = _MM_ADD_f32(x, xmm3,gvl);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  _MMR_f32   _ps_coscof_p0      = _MM_SET_f32(2.443315711809948E-005,gvl);
  _MMR_f32   _ps_coscof_p1      = _MM_SET_f32(-1.388731625493765E-003,gvl);
  _MMR_f32   _ps_coscof_p2      = _MM_SET_f32(4.166664568298827E-002,gvl);
  _MMR_f32   _ps_0p5            = _MM_SET_f32(0.5f,gvl);

  _MMR_f32 z;
  _MMR_f32 tmp;

  y = _ps_coscof_p0;
  z = _MM_MUL_f32(x,x,gvl);

  y = _MM_MUL_f32(y, z,gvl);
  y = _MM_ADD_f32(y, _ps_coscof_p1,gvl);
  y = _MM_MUL_f32(y, z,gvl);
  y = _MM_ADD_f32(y, _ps_coscof_p2,gvl);
  y = _MM_MUL_f32(y, z,gvl);
  y = _MM_MUL_f32(y, z,gvl);
  tmp = _MM_MUL_f32(z, _ps_0p5,gvl);
  y = _MM_SUB_f32(y, tmp,gvl);
  y = _MM_ADD_f32(y, _MM_SET_f32(1.0,gvl) ,gvl);

  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */
  _MMR_f32   _ps_sincof_p0      = _MM_SET_f32(-1.9515295891E-4,gvl);
  _MMR_f32   _ps_sincof_p1      = _MM_SET_f32(8.3321608736E-3,gvl);
  _MMR_f32   _ps_sincof_p2      = _MM_SET_f32(-1.6666654611E-1,gvl);
  _MMR_f32 y2;

  y2 = _ps_sincof_p0;
  y2 = _MM_MUL_f32(y2, z ,gvl);
  y2 = _MM_ADD_f32(y2, _ps_sincof_p1 ,gvl);
  y2 = _MM_MUL_f32(y2, z ,gvl);
  y2 = _MM_ADD_f32(y2, _ps_sincof_p2 ,gvl);
  y2 = _MM_MUL_f32(y2, z ,gvl);
  y2 = _MM_MUL_f32(y2, x ,gvl);
  y2 = _MM_ADD_f32(y2, x ,gvl);

  /* select the correct result from the two polynoms */
  xmm3 = poly_mask;
  y2 = vreinterpret_v_i32m1_f32m1(_MM_AND_i32(vreinterpret_v_f32m1_i32m1(xmm3), vreinterpret_v_f32m1_i32m1(y2) , gvl)); //, xmm3);
  y = vreinterpret_v_i32m1_f32m1(_MM_AND_i32(_MM_XOR_i32(vreinterpret_v_f32m1_i32m1(xmm3), _MM_SET_i32(0xffffffff,gvl),gvl),vreinterpret_v_f32m1_i32m1(y),gvl));   
  y = _MM_ADD_f32(y, y2 ,gvl);
  /* update the sign */
  y = vreinterpret_v_i32m1_f32m1(_MM_XOR_i32(vreinterpret_v_f32m1_i32m1(y), vreinterpret_v_f32m1_i32m1(sign_bit) , gvl));

  return y;
}
