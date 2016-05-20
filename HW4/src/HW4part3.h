#ifndef HW4part3
#define HW4part3

int getpositions(char* fname,int* nAssets, double **ppx, int** pAllocindex, double lower) {

  char buff[100];
  FILE *file;  
  int AllocCnt=0; //assets with allocation percentages
  
  file = fopen(fname, "r");
  if (file == NULL) {
    return 1;
  }

  fscanf(file, "%s", buff);
  fscanf(file, "%s", buff);
  *nAssets = atoi(buff); //read number of assets
  *ppx = (double*)calloc(*nAssets, sizeof(double));
  *pAllocindex = (int*)calloc(*nAssets, sizeof(int));
  
  //check memory allocation successful
  if ( (*ppx) == NULL || (*pAllocindex) == NULL) {
    return 1;
  }

  for (int i = 0; i < *nAssets; i++) {
    fscanf(file, "%s", buff);
    (*ppx)[i] = atof(buff);
  }
  fclose(file);
  
  for (int i = 0; i < *nAssets; i++) {
    if ( (*ppx)[i] > lower) {      
      (*pAllocindex)[AllocCnt++] = i;      
    }
  }
  
  for (int j = 0; j < AllocCnt; j++) {
    (*ppx)[j] = (*ppx)[ (*pAllocindex)[j] ];
  }
  
  *nAssets = AllocCnt; //assign actual number of assets with allocations to nAssets
  
  return 0;
}

int getprices(char* fname, double **pPrices, int n, int *pAllocindex, int *pT) {
  
  FILE *file;
  char buff[100];
  int i, cnt, j;
  int N;
   
  file = fopen(fname, "r");
  if (file == NULL) {
    return 1;
  }
  
  fscanf(file, "%s", buff);
  fscanf(file, "%s", buff);
    
  N = atoi(buff);  
  
  fscanf(file, "%s", buff);
  fscanf(file, "%s", buff);
  *pT = atoi(buff);  

  //skip dates
  fscanf(file, "%s", buff);
  for (j = 0; j < *pT; j++)
    fscanf(file, "%s", buff);

  *pPrices = calloc(n * (*pT), sizeof(double));
  if ( (*pPrices) == NULL)
    return 1;  

  cnt = 0;
  for (i = 0; i < N; i++) {
    fscanf(file, "%s", buff);// skip asset name
    fscanf(file, "%s", buff);// skip Adj_close:
    for (j = 0; j < *pT; j++) {
      fscanf(file, "%s", buff);
      if (cnt<n && i == pAllocindex[cnt] && j < (*pT) )
        (*pPrices)[cnt * (*pT) + j] = atof(buff);
    }
    if (i == pAllocindex[cnt])
      cnt++;
  }
  fclose(file); 
  
  return 0;
}

int CALC_DelSigQuant(double *prices, int nAssets, int t, double **ppDelta, double **pSigma, double budget, double *positions, double **ppQuantities) {
  double change;  
  *ppDelta = (double *) calloc(nAssets, sizeof(double));
  *pSigma = (double *) calloc(nAssets, sizeof(double));
  *ppQuantities = (double *) calloc(nAssets, sizeof(double));
  if ( (*ppDelta) == NULL || (*pSigma) == NULL ||  (*ppQuantities) == NULL )
    return 1;
  
  //delta calculation
  for (int i = 0; i < nAssets; i++) {
    (*ppDelta)[i] = 0;
    for (int k = 0; k < (t - 1); k++) {
      change = (prices[i * t + k + 1] - prices[i * t + k]);
      (*ppDelta)[i] += change;
    }    
    (*ppDelta)[i] /= (t - 1);
  }
  
  //sigma calculation
  change=0;
  for (int i = 0; i < nAssets; i++) {
    (*pSigma)[i] = 0;
    for (int k = 0; k < t - 1; k++) {
      change = (prices[i * t + k + 1] - prices[i * t + k]);
      (*pSigma)[i] += (change - (*ppDelta)[i]) * (change - (*ppDelta)[i]);
    }
    (*pSigma)[i] = sqrt( (*pSigma)[i]/(t - 1) ) ; 
    
  }  
  
  //quantities calculation
  for (int i = 0; i < nAssets; i++) {
    (*ppQuantities)[i] = positions[i] * (budget/ prices[i*t + 0]);
  }  
  
  return 0;
}

double avg(int n, double *arr) {	
	double total = 0.0;
  	
	for (int i = 0; i < n; i++) {
		total += arr[i];
	}

	return total/n;
}

int simulation(int run_ID, int nAssets, int t, double *pPrices, double *pQuantities, double *pDeltas, double *pSigmas, double **pfValues, double **pfReturns) {

  int n = nAssets;  
  double portfolio=0;
  double portfo_value_temp=0; 
  double portfo_return=0;
    
  for (int j = 0; j < t; j++) {
    portfolio = 0.0;
    for (int i = 0; i < n; i++) {
      portfolio += (pPrices[i*t + j] + (pSigmas[i]*drawnormal() + pDeltas[i])) * pQuantities[i];
    }
    if (j > 0) {
      portfo_return += (portfolio - portfo_value_temp) / portfo_value_temp;
    }
    portfo_value_temp = portfolio;
  }

  portfo_return /= (t - 1);

  (*pfReturns)[run_ID] = portfo_return;
  (*pfValues)[run_ID] = portfolio;
  printf("run %d: portfolio value: %g\n", run_ID, (*pfValues)[run_ID]);
  printf("run %d: portfolio return: %g\n", run_ID, (*pfReturns)[run_ID]);
  
  return 0;
}

#endif