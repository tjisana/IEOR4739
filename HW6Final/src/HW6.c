#include <stdio.h>
#include <math.h>
#include <string.h>

double calc_Beta(int k);
int calcDK(int N,double* dkk);
double prob(int k, int kprime,double B);
int opt(int N, int T, double *p, double *optMAXrev, int *path, double *dkk);
double profit(int N, int T, double *p, double *optMAXrev, int *path, double *dkk,char* action);
//int algo(int N, int T, char* model,double *p);
int algo(int N, int T, char* model,char* prob,char* action);

int main(int argc,char** argv){
    int N=50;
    int T=25;
    char* act = (char*)calloc(1000000,sizeof(char));    
    algo(N,T,"squareinverse","squareroot",act);
    return 0;    
}

double calc_Beta(int k){
    double temp = 0.0;    
    for(int i=0;i<=k;i++) temp+= pow(k-i+1,-2);    
    return 1.0/temp;
}
    

double prob(int k, int kprime,double B){    
    return (B / pow(k-kprime+1,2) );
}

int calcDK(int N,double* dkk){
    for(int i=0;i<N;i++) dkk[i] = pow(i+1,-0.5);
    return 0;
}

int calcPR(double* p,int N){
    double Btemp=0.0;    
    for(int k=0;k<=N;k++){
        Btemp = calc_Beta(k);
        for(int kp=0;kp<=k;kp++){
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
    
    for (int n = 0; n <= N; n++) {
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
    for (int t = T-2; t >= 0; t--) {
        
        for (int n = 0; n <= N; n++) {
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
    
    for (int t = 0; t <= T-1; t++) {
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
    
    

    if(strcmp(prob, "squareroot") == 0){
        if ( calcDK(N,dk) != 0 ) return 0; 
    }
    else{
        //no other price impact model implemented
    }

    if(strcmp(model, "squareinverse") == 0){
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