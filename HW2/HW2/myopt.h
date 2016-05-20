#ifndef MYO

#define MYO

typedef struct myo{
  int n;
  int f;
  int iteration;
  double *mu;
  double *sigma2;
  double *V;
  double *upper;
  double *lower;
  double *F;
  double *gradient;
  double *x;
  double *y;
  double s;
  double *Vx;
  double *VtF;
  double *Vy;
}myo;

#define NOMEM 100
#define DATAERROR1 101
 
int myoGetmyoFromFile(myo **ppmyo, char *filename);
int myocreatemyo(myo **pmyo);
void myokillmyo(myo **ppmyo);
int myoalgo(myo *pmyo);
int matrixMult(double* m1,int m1_r,int m1_c,double* m2,int m2_r,int m2_c,double* result);

#include "utilities.h"

#endif
