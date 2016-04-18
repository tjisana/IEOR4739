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
  int ID;
  int status;
  int command;
  int jobnumber;  
  int itercount;
  pthread_mutex_t *psynchro;
  pthread_mutex_t *poutputmutex;
  double *Qprime;
  double *W0;

}powerbag;

void PWRpoweralg_new(powerbag *pbag);
void PWRfreespace(powerbag **ppbag);
void PWRfree(double **pvector);
int PWRreadnload_new(char *filename, int ID, powerbag **ppbag);
int PWRreadnload(char *filename, int *pn, double **pvector, double **pnewvector, double **pmatrix, double **pQprime, double **pW0);


//newfunction - function was abandoned
int PWRread_old(char *filename,int* read_n,double* read_matrix);
//newfunction - function was abandoned
int PWRload(int ID,int* read_n,double* read_matrix, powerbag **ppbag);
//newfunction - function was abandoned
int PWRreadnload(char *filename, int *pn, double **pvector, double **pnewvector, double **pmatrix, double **pQprime, double **pW0);

//new function
int PWRread(char *filename,int* read_n,double** read_matrix);

int PWRreadnload_v2(char *filename, int *pn, double **pvector, double **pnewvector, double **pmatrix, double **pQprime, double **pW0, int *readflag);

int PWRallocatespace(int n, double **pv, double **pnewvector, double **pmatrix, double **pQprime, double **pW0);
void PWRpoweriteration(int ID, int k, 
		    int n, double *vector, double *newvector, double *matrix,
		       double *peigvalue, double *perror,
		         pthread_mutex_t *poutputmutex);

void PWRpoweralg(int n, double *vector, double *newvector, double *matrix,
		 double *peigvalue);

void PWRshowvector(int n, double *vector);

void PWRcompute_error(int n, double *perror, double *newvector, double *vector);
#endif

/*class powerunit{
 public:
  powerunit(char *inputname);
  ~powerunit();
  int readnload(char *file);
  int allocatespace();
  void iterate();
  void showvector(){ for(int j = 0; j < n; j++) printf("%g ",vector[j]);
  printf("\n");}
  char *getname(){return name;}
 private:
  void releasespace();
  int n;
  double *matrix;
  double *vector;
  double *newvector;
  char *name;
};
*/