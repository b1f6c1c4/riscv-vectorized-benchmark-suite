#define min(a,b) ( ((a) < (b)) ? (a) : (b) )

#ifdef USE_RISCV_VECTOR
#define NB 1
#else
#define NB 8
#endif

#ifdef USE_RISCV_VECTOR
#include "../../common/vector_defines.h"
#endif

#ifndef USE_RISCV_VECTOR
/*
 * Blocked Jacobi solver: one iteration step
 */
float relax_jacobi (float *u, float *utmp, unsigned sizex, unsigned sizey)
{
    float diff, sum=0.0;
    int nbx, bx, nby, by;
  
    nbx = NB;
    bx = sizex/nbx;
    nby = NB;
    by = sizey/nby;
    for (int ii=0; ii<nbx; ii++)
        for (int jj=0; jj<nby; jj++) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            utmp[i*sizey+j]= 0.25 * (u[ i*sizey     + (j-1) ]+  // left
					     u[ i*sizey     + (j+1) ]+  // right
				             u[ (i-1)*sizey + j     ]+  // top
				             u[ (i+1)*sizey + j     ]); // bottom
	            diff = utmp[i*sizey+j] - u[i*sizey + j];
	            sum += diff * diff; 
	        }

    return sum;
}

#else
/*
 * Jacobi solver Vectorized Version: one iteration step
 */

float relax_jacobi_vector (float *u, float *utmp, unsigned sizex, unsigned sizey)
{
    _MMR_f32    xU;
    _MMR_f32    xUtmp;
    _MMR_f32    xUleft;
    _MMR_f32    xUright;
    _MMR_f32    xUtop;
    _MMR_f32    xUbottom;
    _MMR_f32    xConstant;
    _MMR_f32    xDiff;
    _MMR_f32    xSum;
    _MMR_f32    xSum2;

    float diff, sum=0.0;
    unsigned long int izq,der;
    int size_y = sizey-2;
    int size_x = sizex-2;

    // unsigned long int gvl = __builtin_epi_vsetvl(size_y, __epi_e64, __epi_m1);
    unsigned long int gvl = vsetvl_e32m1(size_y); //PLCT
    xConstant = _MM_SET_f32(0.25,gvl);
    xSum = _MM_SET_f32(0.0,gvl);
    xSum2 = _MM_SET_f32(0.0,gvl);

    for (int j=1; j<=size_x; j=j+gvl) 
    {
        // gvl = __builtin_epi_vsetvl(size_y-j+1, __epi_e64, __epi_m1);
        gvl = vsetvl_e32m1(size_y-j+1); //PLCT
        
        xU = _MM_LOAD_f32(&u[sizey+j],gvl);
        xUtop = _MM_LOAD_f32(&u[j],gvl);
        xUbottom = _MM_LOAD_f32(&u[2*sizey+j],gvl);

        for (int i=1; i<=size_y; i++) 
        {
            if(i!=1)
            {
                xUtop = xU;
                xU =  xUbottom;
                xUbottom =  _MM_LOAD_f32(&u[(i+1)*sizey+j],gvl);
            }
            
            izq = *(unsigned long int*)&u[i*sizey+(j-1)]; 
            //der = *(unsigned long int*)&u[(i+1)*sizey-1-(j-1)];
            der = *(unsigned long int*)&u[i*sizey+j+gvl];
            xUleft = _MM_VSLIDE1UP_f32(xU,izq,gvl);
            xUright = _MM_VSLIDE1DOWN_f32(xU,der,gvl);
            xUtmp = _MM_ADD_f32(xUleft,xUright,gvl);
            xUtmp = _MM_ADD_f32(xUtmp,xUtop,gvl);
            xUtmp = _MM_ADD_f32(xUtmp,xUbottom,gvl);
            xUtmp = _MM_MUL_f32(xUtmp,xConstant,gvl);
            _MM_STORE_f32(&u[i*sizey+j], xUtmp,gvl);
            //_MM_STORE_f32(&utmp[i*sizey+1], xUtmp,gvl);
            xDiff =_MM_SUB_f32(xUtmp,xU,gvl);
            xSum    = _MM_MACC_f32(xSum,xDiff,xDiff,gvl);
        }
    }

    xSum2 = _MM_REDSUM_f32(xSum,xSum2,gvl);
    sum = _MM_VGETFIRST_f32(xSum2,gvl);
    FENCE();
    return sum;
}
#endif

/*
 * Blocked Red-Black solver: one iteration step
 */
float relax_redblack (float *u, unsigned sizex, unsigned sizey)
{
    float unew, diff, sum=0.0;
    int nbx, bx, nby, by;
    int lsw;

    nbx = NB;
    bx = sizex/nbx;
    nby = NB;
    by = sizey/nby;
    // Computing "Red" blocks
    for (int ii=0; ii<nbx; ii++) {
        lsw = ii%2;
        for (int jj=lsw; jj<nby; jj=jj+2) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            unew= 0.25 * (    u[ i*sizey	+ (j-1) ]+  // left
				      u[ i*sizey	+ (j+1) ]+  // right
				      u[ (i-1)*sizey	+ j     ]+  // top
				      u[ (i+1)*sizey	+ j     ]); // bottom
	            diff = unew - u[i*sizey+ j];
	            sum += diff * diff; 
	            u[i*sizey+j]=unew;
	        }
    }

    // Computing "Black" blocks
    for (int ii=0; ii<nbx; ii++) {
        lsw = (ii+1)%2;
        for (int jj=lsw; jj<nby; jj=jj+2) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            unew= 0.25 * (    u[ i*sizey	+ (j-1) ]+  // left
				      u[ i*sizey	+ (j+1) ]+  // right
				      u[ (i-1)*sizey	+ j     ]+  // top
				      u[ (i+1)*sizey	+ j     ]); // bottom
	            diff = unew - u[i*sizey+ j];
	            sum += diff * diff; 
	            u[i*sizey+j]=unew;
	        }
    }

    return sum;
}

#ifndef USE_RISCV_VECTOR
/*
 * Blocked Gauss-Seidel solver: one iteration step
 */
float relax_gauss (float *u, unsigned sizex, unsigned sizey)
{
    float unew, diff, sum=0.0;
    int nbx, bx, nby, by;

    nbx = NB;
    bx = sizex/nbx;
    nby = NB;
    by = sizey/nby;
    for (int ii=0; ii<nbx; ii++)
        for (int jj=0; jj<nby; jj++) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            unew= 0.25 * (    u[ i*sizey	+ (j-1) ]+  // left
				      u[ i*sizey	+ (j+1) ]+  // right
				      u[ (i-1)*sizey	+ j     ]+  // top
				      u[ (i+1)*sizey	+ j     ]); // bottom
	            diff = unew - u[i*sizey+ j];
	            sum += diff * diff; 
	            u[i*sizey+j]=unew;
                }

    return sum;
}

#else
/*
 * Blocked Gauss-Seidel solver Vectorized Version: one iteration step
 */
float relax_gauss_vector (float *u, unsigned sizex, unsigned sizey)
{
    _MMR_f32    xU;
    _MMR_f32    xUNew;
    _MMR_f32    xUleft;
    _MMR_f32    xUright;
    _MMR_f32    xUtop;
    _MMR_f32    xUbottom;
    _MMR_f32    xConstant;
    _MMR_f32    xDiff;
    _MMR_f32    xSum;
    _MMR_f32    xSum2;

    unsigned long int izq,der;
    int size_y = sizey-2;
    int size_x = sizex-2;
    float diff, sum=0.0;

    // unsigned long int gvl = __builtin_epi_vsetvl(size_x, __epi_e64, __epi_m1);
    unsigned long int gvl = vsetvl_e32m1(size_x); //PLCT
    xConstant = _MM_SET_f32(0.25,gvl);
    xSum = _MM_SET_f32(0.0,gvl);
    xSum2 = _MM_SET_f32(0.0,gvl);

    for (int j=1; j<=size_x; j=j+gvl) 
    {
        // gvl = __builtin_epi_vsetvl(size_x-j, __epi_e64, __epi_m1);
        gvl = vsetvl_e32m1(size_x-j); //PLCT

        xU = _MM_LOAD_f32(&u[sizey+j],gvl);
        xUtop = _MM_LOAD_f32(&u[j],gvl);
        xUbottom = _MM_LOAD_f32(&u[2*sizey+j],gvl);

        for (int i=1; i<=size_y; i++) 
        {
            if(i!=1)
            {
                xUtop = xUNew;
                xU =  xUbottom;
                xUbottom =  _MM_LOAD_f32(&u[(i+1)*sizey+j],gvl);
            }
            
            izq = *(unsigned long int*)&u[i*sizey+(j-1)]; 
            der = *(unsigned long int*)&u[i*sizey+j+gvl];
            xUleft = _MM_VSLIDE1UP_f32(xU,izq,gvl);
            xUright = _MM_VSLIDE1DOWN_f32(xU,der,gvl);
            xUNew = _MM_ADD_f32(xUleft,xUright,gvl);
            xUNew = _MM_ADD_f32(xUNew,xUtop,gvl);
            xUNew = _MM_ADD_f32(xUNew,xUbottom,gvl);
            xUNew = _MM_MUL_f32(xUNew,xConstant,gvl);
            _MM_STORE_f32(&u[i*sizey+j], xUNew,gvl);
            xDiff =_MM_SUB_f32(xUNew,xU,gvl);
            xSum    = _MM_MACC_f32(xSum,xDiff,xDiff,gvl);
        }
    }

    xSum2 = _MM_REDSUM_f32(xSum,xSum2,gvl);
    sum = _MM_VGETFIRST_f32(xSum2,gvl);
    FENCE();
    return sum;
}
#endif
