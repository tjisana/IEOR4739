#ifndef POWER

#define POWER


#define WAITING 100
#define WORKING 101
#define PREANYTHING 102
#define DONEWITHWORK 103


#define QUIT 200
#define WORK 201
#define STANDBY 202
#define INTERRUPT 203

typedef struct powerbag{
  int n;
  double *vector;
  double *newvector;
  double *matrix;
  double *matcopy;
  double *scratch;
  double topeigvalue;
  double *vectortopeigvalue;
  int ID;
  int status;
  int command;
  int jobnumber;  
  int itercount;
  pthread_mutex_t *psynchro;
  pthread_mutex_t *poutputmutex;
   //new members
  double *Qprime;
  double *W0;
  int eigvaluecount;
}powerbag;

void PWRpoweralg_new(powerbag *pbag);
void PWRfreespace(powerbag **ppbag);
void PWRfree(double **pvector);
int PWRreadnload_new(char *filename, int ID, powerbag **ppbag);

int PWRallocatespace(int n, double **pvector, double **pnewvector, double **pmatrix);
int PWRreadnload(char *filename, int *pn, double **pvector, double **pnewvector, double **pmatrix);

void PWRpoweriteration(int ID, int k, 
		    int n, double *vector, double *newvector, double *matrix,
		       double *peigvalue, double *perror,
		         pthread_mutex_t *poutputmutex);

void PWRpoweralg(int n, double *vector, double *newvector, double *matrix,
		 double *peigvalue);

void PWRshowvector(int n, double *vector);

void PWRcompute_error(int n, double *perror, double *newvector, double *vector);

//NEW FUNCTIONS
int PWRread(char *filename,int* read_n,double** read_matrix);
//int PWRload(int ID,int n,double* read_matrix, powerbag **ppbag);
//int PWRallocateNload(int n,double* read_matrix, powerbag **ppbag, double **pvector, double **pnewvector, double **pmatrix,
//                     double **pmatcopy,double **pscratch, double **pvectortopeigvalue,double **pQprime,double **pW0);
int PWRallocateNload(int n,int ID, double* read_matrix, powerbag **ppbag,int numeigenvalue);
#endif