#include <assert.h>
#include "HJM_type.h"

#include <cstring>

#include <time.h>
#include <sys/time.h>


FTYPE RanUnif( int *s );
void RanUnif_vector( int *s , int iFactors , int iN ,int  BLOCKSIZE, FTYPE **randZ);

FTYPE CumNormalInv( FTYPE u );
void CumNormalInv_vector( FTYPE* u ,FTYPE* output ,unsigned int gvl);

void icdf_SSE(const int N, FTYPE *in, FTYPE *out);
void icdf_baseline(const int N, FTYPE *in, FTYPE *out);
int HJM_SimPath_Forward_SSE(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
			    FTYPE **ppdFactors, int *lRndSeed);
int Discount_Factors_SSE(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath);
int Discount_Factors_opt(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath);


int HJM_SimPath_Forward_Blocking_SSE(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
			    FTYPE **ppdFactors, int *lRndSeed, int BLOCKSIZE);
int HJM_SimPath_Forward_Blocking(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
			    FTYPE **ppdFactors, int *lRndSeed, int BLOCKSIZE);


int Discount_Factors_Blocking(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath, int BLOCKSIZE);
int Discount_Factors_Blocking_vector(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath, int BLOCKSIZE);
int Discount_Factors_Blocking_SSE(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath, int BLOCKSIZE);


int HJM_Swaption_Blocking_SSE(FTYPE *pdSwaptionPrice, //Output vector that will store simulation results in the form:
			                              //Swaption Price
			                              //Swaption Standard Error
			      //Swaption Parameters 
			      FTYPE dStrike,				  
			      FTYPE dCompounding,     //Compounding convention used for quoting the strike (0 => continuous,
			      //0.5 => semi-annual, 1 => annual).
			      FTYPE dMaturity,	      //Maturity of the swaption (time to expiration)
			      FTYPE dTenor,	      //Tenor of the swap
			      FTYPE dPaymentInterval, //frequency of swap payments e.g. dPaymentInterval = 0.5 implies a swap payment every half
		                              //year
			      //HJM Framework Parameters (please refer HJM.cpp for explanation of variables and functions)
			      int iN,						
			      int iFactors, 
			      FTYPE dYears, 
			      FTYPE *pdYield, 
			      FTYPE **ppdFactors,
			      //Simulation Parameters
			      int iRndSeed, 
			      int lTrials, int blocksize, int tid);
 
int HJM_Swaption_Blocking(FTYPE *pdSwaptionPrice, //Output vector that will store simulation results in the form:
			                              //Swaption Price
			                              //Swaption Standard Error
			      //Swaption Parameters 
			      FTYPE dStrike,				  
			      FTYPE dCompounding,     //Compounding convention used for quoting the strike (0 => continuous,
			      //0.5 => semi-annual, 1 => annual).
			      FTYPE dMaturity,	      //Maturity of the swaption (time to expiration)
			      FTYPE dTenor,	      //Tenor of the swap
			      FTYPE dPaymentInterval, //frequency of swap payments e.g. dPaymentInterval = 0.5 implies a swap payment every half
		                              //year
			      //HJM Framework Parameters (please refer HJM.cpp for explanation of variables and functions)
			      int iN,						
			      int iFactors, 
			      FTYPE dYears, 
			      FTYPE *pdYield, 
			      FTYPE **ppdFactors,
			      //Simulation Parameters
			      int *iRndSeed, 
			      int lTrials, int blocksize, int tid);

/*
extern "C" FTYPE *dvector( int nl, int nh );
extern "C" FTYPE **dmatrix( int nrl, int nrh, int ncl, int nch );
extern "C" void free_dvector( FTYPE *v, int nl, int nh );
extern "C" void free_dmatrix( FTYPE **m, int nrl, int nrh, int ncl, int nch );
*/
