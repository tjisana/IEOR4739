#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utilities.h"
#include "HW4part3.h"



int main(int argc,char** argv){
  int retcode=0;
  int nAssets; //number of assets
  int T; //number of timeperiods  
  
  double *pPositions = NULL;
  int *pAllocated = NULL;  
  
  //int nRuns = 1000000; //number of simulations
  int nRuns = 10; //number of simulations
  double budget = 1e10; //B
  
  double *prices = NULL;  
  double *deltas = NULL;
  double *sigmas = NULL;
  double *quantities = NULL;
  double *PFvalues= (double*)calloc(nRuns, sizeof(double));
  double *PFreturns = (double*)calloc(nRuns, sizeof(double));
  
  //get positions from file 
  if( (retcode = getpositions(argv[1],&nAssets, &pPositions, &pAllocated,0)) != 0)
    return retcode;  
  //get prices
  if( (retcode = getprices(argv[2], &prices, nAssets, pAllocated, &T)) != 0 )
    return retcode;
  
  if( (retcode = CALC_DelSigQuant(prices, nAssets, T, &deltas,&sigmas, budget, pPositions, &quantities)) != 0 )
    return retcode;
  
  for(int i = 0; i < nRuns; i++) {
    retcode = simulation(i, nAssets, T, prices, quantities, deltas, sigmas, &PFvalues, &PFreturns);
  }

  printf("Average value: %g\n", avg(nRuns, PFvalues));
  printf("Average return: %g\n", avg(nRuns, PFreturns));
  
    free(prices);
    free(deltas);
    free(sigmas);
    free(quantities);
    free(PFvalues);
    free(PFreturns);
    return 0;
}