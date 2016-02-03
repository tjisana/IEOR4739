#include <stdio.h>
#include <stdlib.h>

int readit(char *name, double **pdata, int *pn, int *pT);

int main(int argc, char *argv[])
{
  int code = 0, n, T;
  double *data = NULL;

  if(argc < 2){ 
    printf(" usage: algo filename\n");
    code = 1; goto BACK;
  }

  if((code = readit(argv[1], &data, &n, &T)))
    goto BACK;

	 
BACK:
  if(data) free(data);
  return code;
}

int readit(char *name, double **pdata, int *pn, int *pT)
{
  int retcode = 0, n, T, t, j;
  FILE *input = NULL;
  char buffer[100];
  double *data;

  input = fopen(name, "r");
  if(!input){
    printf("cannot open file %s\n", name); retcode = 1; goto BACK;
  }
  printf("reading file %s\n", name);
  fscanf(input,"%s",buffer);  fscanf(input,"%s",buffer);
  n = atoi(buffer);

  fscanf(input,"%s",buffer);  fscanf(input,"%s",buffer);
  T = atoi(buffer);

  printf("n = %d T = %d\n", n, T);
  data = (double *)calloc(n*T, sizeof(double));
  if(!data){
    printf("not enough memory\n"); retcode = 1; goto BACK;
  }
  *pn = n;
  *pT = T;
  *pdata = data;

  for(t = 0; t < T; t++){
    for(j = 0; j < n; j++){
      if(EOF == fscanf(input,"%s",buffer)){
	printf("premature file termination at t = %d\n", T); retcode = 1;
	goto BACK;
      }
      data[t*n + j] = atof(buffer);
    }
  }
  fclose(input);
  

 BACK:
  return retcode;
}





















