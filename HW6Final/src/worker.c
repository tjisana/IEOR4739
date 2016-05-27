#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gsock.h"

int compare2substr(char *buffer, char *target);
int workeralgorithm(int hSocket, char *pBuffer);

double calc_Beta(int k);
int calcDK(int N,double* dkk);
double prob(int k, int kprime,double B);
int opt(int N, int T, double *p, double *optMAXrev, int *path, double *dkk);
double profit(int N, int T, double *p, double *optMAXrev, int *path, double *dkk,char* action);
int algo(int N, int T, char* model,char* prob,char* action);

int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    int retcode = 0;

    int N,T;
    char im[50],pd[50];
    char* act = (char*)calloc(1000000,sizeof(char));    

    if(argc < 3){
	printf("\nUsage: worker host-name host-port\n");
	return 0;
    }
    else{
	strcpy(strHostName,argv[1]);
	nHostPort=atoi(argv[2]);
    } 


    printf("\nStarting a socket"); fflush(stdout);
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR){
      printf("\nCould not make a socket\n"); fflush(stdout);
      retcode = 1; goto BACK;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    printf("\nConnecting to %s on port %d",strHostName,nHostPort);
    fflush(stdout);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nworker could not connect to host\n");
	fflush(stdout);       retcode = 1; goto BACK;
    }

    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */

    nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);
    printf("\nReceived \"%s\" from master (%d bytes)\n",pBuffer, nReadAmount);
    N = atoi(pBuffer);
    fflush(stdout);

    nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);
    printf("Received \"%s\" from master (%d bytes)\n",pBuffer, nReadAmount);
    T = atoi(pBuffer);
    fflush(stdout);

    nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);
    printf("Received \"%s\" from master (%d bytes)\n",pBuffer, nReadAmount);
    strcpy(im,pBuffer);  
    fflush(stdout);

    nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);
    printf("Received \"%s\" from master (%d bytes)\n",pBuffer, nReadAmount);
    strcpy(pd,pBuffer);  
    fflush(stdout);
    
    //retcode = workeralgorithm(hSocket, pBuffer);
    printf("\n%d %d %s %s\n",N,T,im,pd);
    algo(N,T,im,pd,act);

    //sprintf(pBuffer,atoi(pBuffer));
    //printf("Writing \"%s\" to master \n",pBuffer); fflush(stdout);
    write(hSocket,act, strlen(act) + 1);

    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR){
      printf("\nCould not close socket\n");
      retcode = 1;
    }

 BACK:
    free(act);
    return retcode;
}


int workeralgorithm(int hSocket, char *pBuffer)
{
  int retcode = 0, nReadAmount, n, j;
  double xval;

  printf("\n worker now running algorithm\n");

  nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);

  printf("\nReceived \"%s\" from master; read %d bytes\n",pBuffer, nReadAmount);
  fflush(stdout);

  n = atoi(pBuffer);
  
  printf("actually got %d\n", n);   fflush(stdout);

  for(j = 0; j < n; j++){
    nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);
    xval = atof(pBuffer);
    printf(" -> %d. %g\n", j, xval); fflush(stdout);
    sprintf(pBuffer,"WORKERACK");
    write(hSocket, pBuffer, strlen(pBuffer)+1);
  }

  nReadAmount = read(hSocket,pBuffer,BUFFER_SIZE);
  printf(" got '%s' from master\n", pBuffer); fflush(stdout);


  sprintf(pBuffer,"bye");
  printf("Writing \"%s\" to master ",pBuffer); fflush(stdout);
  write(hSocket,pBuffer, strlen(pBuffer) + 1);

  return retcode;
}

int compare2substr(char *buffer, char *target)
{
  int retcode = 0, len, lent, j;

  len = strlen(buffer);
  lent = strlen(target);

  if(len < lent){
    retcode = -1; goto BACK;
  }

  for(j = 0; j < lent; j++){
    if( buffer[j] != target[j] ){
      retcode = 1; goto BACK;
    }
  }

 BACK:
  return retcode;
}

double calc_Beta(int k){
    double temp = 0.0;    
    int i=0;
    for(i=0;i<=k;i++) temp+= pow(k-i+1,-2);    
    return 1.0/temp;
}

double prob(int k, int kprime,double B){    
    return (B / pow(k-kprime+1,2) );
}

int calcDK(int N,double* dkk){
    int i=0;
    for(i=0;i<N;i++) dkk[i] = pow(i+1,-0.5);
    return 0;
}

int calcPR(double* p,int N){
    double Btemp=0.0;    
    int k,kp;
    for(k=0;k<=N;k++){
        Btemp = calc_Beta(k);
        for(kp=0;kp<=k;kp++){
            p[k * (N+1)+ kp] =  prob(k,kp,Btemp);            
        }        
    }    
    return 0;
}

int opt(int N, int T, double *p, double *optMAXrev, int *path, double *dkk){    
    double revenue;
    double max_revenue = 0.0;
    int max_index;
    int k, kp;
    int t = T-1;

    //calculate last period first
    int n;
    
    for (n = 0; n <= N; n++) {
        revenue = 0.0;
        //k = n; At the final period, we sell all stocks we have
        for (kp = 0; kp <= n; kp++) {
            revenue += p[n * (N+1) + kp] * (double)kp;
        }
        revenue *= dkk[n];
        if (revenue > max_revenue) max_revenue = revenue;
        optMAXrev[t*(N+1) + n]= max_revenue;
        path[t*(N+1) + n] = n;        
    }    

    //calculate other periods starting from T-2
    for (t = T-2; t >= 0; t--) {
        
        for (n = 0; n <= N; n++) {
            max_revenue = 0.0;
            max_index = 0;
            for (k = 0; k <= n; k++) {
                revenue = 0.0;
                for (kp = 0; kp <= k; kp++) {
                    revenue += p[n * (N+1) + kp]  * ( (double)kp + optMAXrev[ (t+1)*(N+1) + (n-kp) ] );
                }
                revenue *= dkk[k];                
                if (revenue > max_revenue) {
                    max_revenue = revenue;
                    max_index = k;
                }
            }            
            optMAXrev[t*(N+1) + n]= max_revenue;
            path[t*(N+1) + n] =  max_index;            
        }
    }

    return 0;
}

double profit(int N, int T, double *p, double *optMAXrev, int *path, double *dkk,char* action){    
    double profit = 0.0;
    double new_price = 1.0;
    int sell;
    int left = N;    

    //char action[1000000];
    int t;
    for (t = 0; t <= T-1; t++) {
        sell = path[t*(N+1) + left];
        sprintf(action + strlen(action),"Step %d: Optimal: %0.6f, \tSell : %d", t, optMAXrev[t*(N+1) + left], sell);
        left = left - sell;
        new_price *= dkk[sell];        
        sprintf(action + strlen(action),", \tprice: %0.6f, \tvalue: %g", new_price, new_price * (double)sell);
        profit += new_price * (double)sell;    
        sprintf(action + strlen(action),"\n");
    }

    sprintf(action + strlen(action),"Maximum Profit: %g\n", profit);    
    printf("%s\n",action);
    return 0;
}

int algo(int N, int T, char* model,char* prob,char* action){       

    //int N=4;    
    //int T=2;

    //double* v = (double*)calloc(N + (N+1) * (N+1) + T*(N+1),sizeof(double));
    /*double* dk      =v;
    double* pr      = v + N;
    double* MAXrev  = v + N + (N+1) * (N+1);*/

    double* dk=(double*)calloc(N,sizeof(double)); 
    double* MAXrev = (double*)calloc(T*(N+1), sizeof(double));    
    double* p = (double*)calloc((N+1) * (N+1),sizeof(double));    
    //char* act = (char*)calloc(1000000,sizeof(char));    
    int* path = (int*)calloc(T*(N+1), sizeof(int));
    //for(int i=0;i< T*(N+1);i++) printf("%g\n",path[i] );  
    

    if(strcmp(prob, "squareinverse") == 0){
        if ( calcDK(N,dk) != 0 ) return 0; 
    }
    else{
        //no other price impact model implemented
    }

    if(strcmp(model, "squareroot") == 0){
        if ( calcPR(p,N) != 0 ) return 0;
    }
    else{
        //no other probability distributed implemented
    }

    if ( opt(N,T,p,MAXrev,path,dk) != 0 ) return 0;        
    if ( profit(N,T,p,MAXrev,path,dk,action) != 0 ) return 0;    

    //free(v);
    free(dk);
    free(p);
    free(path);
    return 0;
}