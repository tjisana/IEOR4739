#include <stdio.h>
#include <stdlib.h>
#include "myopt.h"

int myofind_feasible(myo *pmyo);
int myo_iteration(myo *pmyo);
int myo_getgradient(myo *pmyo);
int myo_step(myo *pmyo);
void myo_showx(myo *pmyo, int start, int end);
void myoVtimesy(myo *pmyo, double *y);
int myoprepare(myo *pmyo);

#define LOUDFEASIBLE

int myoalgo(myo *pmyo)
{
  int retcode = 0;
  int max_its = 10;

  if((retcode = myoprepare(pmyo))) goto BACK;

  if((retcode = myofind_feasible(pmyo))) goto BACK;

  for(pmyo->iteration = 0; pmyo->iteration < max_its; pmyo->iteration++){
    if((retcode = myo_iteration(pmyo))) goto BACK;
    myo_showx(pmyo, 0, pmyo->n-1);
  }

 BACK:
  printf("myoalgo returning with code %d\n\n", retcode);
  return retcode;
}
int myoprepare(myo *pmyo)
{
  int retcode = 0;
  int i, j, k, f = pmyo->f, n = pmyo->n;
  double sum, *V = pmyo->V, *F = pmyo->F;
  /** first compute VtF, which is an nxf matrix **/

  for(j = 0; j < n; j++){
    for(i = 0; i < f; i++){
      /* compute the (j,i) entry of VtF */
      sum = 0;
      for (k = 0; k < f; k++){
	sum  += V[k*n + j]*F[k*f + i];
      }
      pmyo->VtF[j*f + i] = sum;
    }
  }
  
  printf("myoprepare returns %d\n", retcode);
  return retcode;
}
  
int myofind_feasible(myo *pmyo)
{
  int retcode = 0;
  int j;
  double sum, *x = pmyo->x, delta;

  sum = 0;
  for(j = 0; j < pmyo->n; j++){
    x[j] = pmyo->lower[j];  /** we assume lower <= upper, but should
				      check **/
#ifdef LOUDFEASIBLE
    printf("  -> x[%d] initialized at %g\n", j, x[j]);
#endif

    sum += x[j];
  }
  if(sum > 1.0){
    printf(" error: sum of asset lower bounds equals %g > 1.0\n", sum);
    retcode = DATAERROR1; goto BACK;
  }

  for(j = 0; (j < pmyo->n) && (sum < 1.0) ; j++){
    if(sum < 1.0){
      delta = (1.0 - sum < pmyo->upper[j] - pmyo->lower[j]) ?
	1.0 - sum : pmyo->upper[j] - pmyo->lower[j];
      sum += delta;
      x[j] += delta;
    }
#ifdef LOUDFEASIBLE
    printf("  -> x[%d] increased to %g\n", j,x[j]);
#endif
  }
  printf("find_feasible done at j = %d\n", j);

 BACK:
  return retcode;
}

int myo_iteration(myo *pmyo)
{
  int retcode = 0;

  printf("\niteration %d\n", pmyo->iteration);

  if( (retcode = myo_step(pmyo))) goto BACK;

 BACK:
  return retcode;
}

int myo_getgradient(myo *pmyo)
{
  int retcode = 0, i, j, n = pmyo->n, f = pmyo->f;
  double *Vx = pmyo->Vx, *VtF = pmyo->VtF, sum;

  printf(" computing gradient at iteration %d\n", pmyo->iteration);
  myoVtimesy(pmyo, pmyo->x);
  for(j = 0; j < n; j++){
    /** first initialize the gradient **/

    pmyo->gradient[j] = -pmyo->mu[j] + 2*pmyo->sigma2[j]*pmyo->x[j];

    /** computes jth entry of VtFVx */
    sum = 0;
    for(i = 0; i < f; i++){
      sum += VtF[j*f + i]*Vx[i];
    }
    pmyo->gradient[j] += 2*sum;
  }

  return retcode;
}


int myo_step(myo *pmyo)
{
  int retcode = 0;
  int m=0;//counter for finding y*
  double cost=0,lowestcost=0;

  printf(" computing step at iteration %d\n", pmyo->iteration);

  if( (retcode = myo_getgradient(pmyo))) goto BACK;  
  int *index=calloc(pmyo->n,sizeof(int));
  double *ysorted=calloc(pmyo->n,sizeof(double));
  for (int i = 0; i < pmyo->n; ++i)
  {
    index[i]=i;
  }

  int cmp(const void* a, const void* b)
  {
    int ia=*(int *)a;
    int ib=*(int *)b;
    return pmyo->gradient[ia]>pmyo->gradient[ib] ? -1:pmyo->gradient[ia]<pmyo->gradient[ib];
  } 
  /** next, sort gradient to start solving the auxiliary LP (k1)**/
  qsort(index,pmyo->n,sizeof(int),cmp); //sort the indices of gradient vector

  /** next, compute direction y that is solve the auxiliary (k1)LP**/
  //enumerate all values of m
  double ysum=0;
  for (m = 0; m < pmyo->n; ++m)
  {
      ysum=0;
      for (int i = 0; i < m; ++i)
      {
        ysorted[i]=pmyo->lower[index[i]]-pmyo->x[index[i]];
        ysum+=ysorted[i];
      }
      for (int i = m+1; i < pmyo->n; ++i)
      {
        ysorted[i]=pmyo->upper[index[i]]-pmyo->x[index[i]];
        ysum+=ysorted[i];
      }
      ysorted[m]=-ysum;
      
      if (!(ysorted[m]>pmyo->upper[index[m]]-pmyo->x[index[m]] || ysorted[m]<pmyo->lower[index[m]]-pmyo->x[index[m]]))
      {
          cost=0;
          for (int i = 0; i < pmyo->n; ++i)
          {
            cost+=pmyo->gradient[index[i]] * ysorted[i];
          }

          if(cost<lowestcost)
          {
            lowestcost=cost;
            for (int i = 0; i < pmyo->n; ++i)
            {
              pmyo->y[index[i]]=ysorted[i];              
            }
          }
      }
      
  }

  /** next, compute step size s (k2) **/  

  double denominator = 0.0, numerator = 0.0, sum=0,lambda=1;

  /** compute Vy **/
  for(int i = 0; i < pmyo->f; i++){
    sum = 0;
    for(int j = 0; j < pmyo->n; j++){
      sum += pmyo->V[i*pmyo->n + j]*pmyo->y[j];
    }
    pmyo->Vy[i] = sum;
  }

  for(int j = 0; j < pmyo->n; j++) {  
    sum = 0;
    for(int i = 0; i < pmyo->f; i++){
      sum += pmyo->VtF[j*pmyo->f + i]*pmyo->Vy[i];
    }
    
    sum += pmyo->sigma2[j] * pmyo->y[j];

    /** multiply by y_j to get the j-th entry of yT * (VTFV y + D y) and add it to a**/
    denominator += sum * pmyo->y[j];
  }
  //a *= 2.0 * lambda;
  
  
  sum = 0.0;
  for (int j = 0; j < pmyo->n; j++) {
    sum += pmyo->y[j] * pmyo->gradient[j];
  }  


  pmyo->s = -sum/(denominator * 2 * lambda);

  /** To maintain feasibility we need to constrain s to be in [0, 1]
    Since G(s) is a parabola facing upwards,
    we see that if s is outside [0, 1] we only need to take the closest point of [0, 1] ( 0 or 1 ) to s
    which justifies the following code
  **/
  if (pmyo->s > 1.0) {
    pmyo->s = 1.0;
  }
  if (pmyo->s < 0.0) {
    pmyo->s = 0.0;
  }
  /** This second check is however redundant because 
    b = yT * gradient < 0
    a = 2lambda yTDy + yTVTFVy = 2 lambda yT Q y > 0
    so s = -b/a has to be > 0
  **/

  printf("step size = %g\n", pmyo->s);
  
  /** update x **/
  for(int j = 0; j < pmyo->n; j++){
    pmyo->x[j] += pmyo->s*pmyo->y[j];
  }

 BACK:
  return retcode;
}

void myoVtimesy(myo *pmyo, double *y)
{
  /** here y is an n-vector, and we compute V*y and place it in pmyo->Vx **/
  int i, j, f = pmyo->f, n = pmyo->n;
  double sum, *V = pmyo->V;

  for(i = 0; i < f; i++){
    sum = 0;
    for(j = 0; j < n; j++){
      sum += V[i*n + j]*y[j];
    }
    pmyo->Vx[i] = sum;
  }
}

void myo_showx(myo *pmyo, int start, int end)
{
  int j;

  start = start < 0 ? 0 : start;
  end = end >= pmyo->n ? pmyo->n-1 : end;
  printf("\n");
  for(j = start; j <= end; j++){
    printf("  x[%d] = %g\n", j, pmyo->x[j]);
  }
  printf("\n");
}

