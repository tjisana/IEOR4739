
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <gurobi_c.h>


int engine(int numassets, int numfactors, 
	     double *ub, double *lb, double *mu, double *sigma2, 
	   double *V, double *F, double lambda)
{
  int retcode = 0;
  GRBenv   *env = NULL;
  GRBmodel *model = NULL;
  int n, i, j, k;
  double *x;
  int *qrow, *qcol, Nq;
  double *qval;
  int *cind;
  double rhs;
  char sense;
  double *cval;
  int numnonz;
  char **names, bigname[100];
  double expectedreturnval;

  printf("running solver engine\n");

  n = numassets + numfactors;

  retcode = GRBloadenv(&env, "engine.log");
  if (retcode) goto BACK;

 /* Create initial model */
  retcode = GRBnewmodel(env, &model, "factors", n, 
                      NULL, NULL, NULL, NULL, NULL);
  if (retcode) goto BACK;

  names = (char **) calloc(n, sizeof(char *));

  /** next we create the remaining attributes for the n columns **/
  x     = (double *) calloc (n, sizeof(double));

  for(j = 0; j < numassets; j++){
    names[j] = (char *)calloc(3, sizeof(char));
    if(names[j] == NULL){
      retcode = 1; goto BACK;
    }
    sprintf(names[j],"x%d", j);
  }
  for(j = numassets; j < numassets + numfactors; j++){
    names[j] = (char *)calloc(3, sizeof(char));
    if(names[j] == NULL){
		  retcode = 1; goto BACK;
    }
    sprintf(names[j],"F%d", j - numassets);
  }


  /* initialize variables */
  for(j = 0; j < n; j++){
    retcode = GRBsetstrattrelement(model, "VarName", j, names[j]);
    if (retcode) goto BACK;

    retcode = GRBsetdblattrelement(model, "Obj", j, -mu[j]);
    if (retcode) goto BACK;

    retcode = GRBsetdblattrelement(model, "LB", j, lb[j]);
    if (retcode) goto BACK;

    retcode = GRBsetdblattrelement(model, "UB", j, ub[j]);
    if (retcode) goto BACK;

  }

  /** next, the quadratic -- there are numassets + numfactors*numfactors nonzeroes: 
      numassets residual variances plus the numfactors x numfactors
      factor covariance matrix**/

  Nq = numassets + numfactors*numfactors;
  qrow = (int *) calloc(Nq, sizeof(int));  /** row indices **/
  qcol = (int *) calloc(Nq, sizeof(int));  /** column indices **/
  qval = (double *) calloc(Nq, sizeof(double));  /** values **/

  if( ( qrow == NULL) || ( qcol == NULL) || (qval == NULL) ){
    printf("could not create quadratic\n");
    retcode = 1; goto BACK;
  }

  for (j = 0; j < numassets; j++){
    qval[j] = lambda*sigma2[j];
    qrow[j] = qcol[j] = j;
  }
  for (i = 0; i < numfactors; i++){
    for (j = 0; j < numfactors; j++){
      k = i*numfactors + j;
      qval[k + numassets] = lambda*F[k];
      qrow[k + numassets] = numassets + i;
      qcol[k + numassets] = numassets + j;
    }
  }
  retcode = GRBaddqpterms(model, Nq, qrow, qcol, qval);
  if (retcode) goto BACK;

  /** now we will add one constraint at a time **/
  /** we need to have a couple of auxiliary arrays **/

  cind = (int *)calloc(n, sizeof(int));  /** n is over the top since no constraint is totally dense;		     but it's not too bad here **/
  cval= (double *)calloc(n, sizeof(double));
  if(!cval){
    printf("cannot allocate cval\n"); retcode = 2; goto BACK;
  }
  for(i = 0; i < numfactors; i++){
    for(j = 0; j < numassets; j++){
      cval[j] = V[i*numassets + j];
      cind[j] = j;
    }
    cind[numassets] = /* j */ numassets + i;
    cval[numassets] = -1;
    numnonz = numassets + 1;
    rhs = 0;
    sense = GRB_EQUAL;

    sprintf(bigname,"factor%d",i);
    retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, bigname);
    if (retcode) goto BACK;

  }

  /** sum of x variables = 1 **/


  for (j = 0; j < numassets; j++){
    cval[j] = 1.0;  cind[j] = j;
  }

  numnonz = numassets;
  rhs = 1.0;
  sense = GRB_EQUAL;

  /* let's reuse some space */
  sprintf(bigname, "sum");

  retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, bigname);
  if (retcode) goto BACK;

  retcode = GRBupdatemodel(model);
  if (retcode) goto BACK;

  /** optional: write the problem **/

  retcode = GRBwrite(model, "engine.lp");
  if (retcode) goto BACK;


  retcode = GRBoptimize(model);
  if (retcode) goto BACK;


  /** get solution **/


  retcode = GRBgetdblattrarray(model,
                               GRB_DBL_ATTR_X, 0, n,
                               x);
  if(retcode) goto BACK;

  /** now let's see the values **/

  expectedreturnval = 0;
  for(j = 0; j < numassets; j++){
    if( x[j] > 1.0e-09){
      printf("%s = %g\n", names[j], x[j]);
      expectedreturnval += x[j]*mu[j];
    }
  }

  printf("\n*** expected portfolio return: %g\n", expectedreturnval);

  GRBfreemodel(model);
  GRBfreeenv(env);



 BACK:
  printf("engine exits with code %d\n", retcode);
  return retcode;
}
