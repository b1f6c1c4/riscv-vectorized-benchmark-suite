#include "HJM_type.h"

void     nrerror(const char *error_text);
int      choldc(FTYPE **a, int n);
void     gaussj(FTYPE **a, int n, FTYPE **b, int m);
int      *ivector(int nl, int nh);
void     free_ivector(int *v, int nl, int nh);
FTYPE   *dvector( int nl, int nh );
void     free_dvector( FTYPE *v, int nl, int nh );
FTYPE   **dmatrix( int nrl, int nrh, int ncl, int nch );
void     free_dmatrix( FTYPE **m, int nrl, int nrh, int ncl, int nch );
