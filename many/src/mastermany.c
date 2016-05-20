#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "gsock.h"

pthread_mutex_t synchro;


typedef struct mastersock{
  baggedsock *pbsock;
  char *ackphrase;
  int localcount; /** the count of how many times this master thread has started 
		  a work loop **/
  int *pK; /** address of the quantity of jobs to do **/
  int *pjobcount; /** address of the global count of how many jobs have been done **/
  pthread_t thread_id;
  int runcode;
}mastersock;

void *interaction(void *voidedmastersock);


int main(int argc, char* argv[])
{
   
  int retcode = 0, K, wcount, h, Num, wcode = 0, numwritten;
    mastersock **ppmastersockarr, *pmastersock;

    if(argc < 3){
	printf("\nUsage: master number_of_jobs number_of_workers\n");
	return 0;
    }

    K = atoi(argv[1]);
    Num = atoi(argv[2]);
    /** K = number of jobs, Num = number of workers **/
    if(K < Num) Num = K;  /** don't need more workers than jobs **/

    wcount = 0; /** no jobs done yet **/

    ppmastersockarr = (mastersock **)calloc(Num, sizeof(mastersock*));
    if(ppmastersockarr == NULL){
      printf("no mastersockarr\n"); retcode = 1; goto BACK;
    }

    for(h = 0; h < Num; h++){

      ppmastersockarr[h] = (mastersock *)calloc(1, sizeof(mastersock));
      if(ppmastersockarr[h] == NULL){printf("no mastersock %d\n", h);retcode = 1; goto BACK;}

      pmastersock = ppmastersockarr[h];

      pmastersock->ackphrase = (char *)calloc(100,sizeof(char));
      if(pmastersock->ackphrase == NULL){printf("no mastersock %d\n", h);retcode = 1; goto BACK;}

      if((retcode = creategsock(&pmastersock->pbsock, 0))) goto BACK;
      pmastersock->pbsock->id = h;
      pmastersock->pK = &K;
      pmastersock->pjobcount = &wcount;
    }


    printf("\nStarting master");

    /** first, all the socket crud gets done for each bag **/
    int lastone = 50000;

    for(h = 0; h < Num; h++){
      printf("\ncreating socket %d", h);
      pmastersock = ppmastersockarr[h];

      retcode = startupsock(pmastersock->pbsock);

      if((retcode = gotrybind(pmastersock->pbsock, lastone))) goto BACK;

      sprintf(pmastersock->ackphrase,"MASTERACK%d", pmastersock->pbsock->nHostPort);

      lastone = pmastersock->pbsock->nHostPort + 1;

      retcode = finishopenstuff(pmastersock->pbsock);
    }

    for(h = 0; h < Num; h++){
      pmastersock = ppmastersockarr[h];
      printf("\n%d. Waiting for a connection\n", pmastersock->pbsock->id);

      retcode = startconnection(pmastersock->pbsock);
      if(retcode) goto BACK;

      sprintf(pmastersock->pbsock->pBuffer,"hey_%d", h);
      printf("\n%d. Sending \"%s\" to worker\n",pmastersock->pbsock->id, pmastersock->pbsock->pBuffer);
      wcode = write2socket(pmastersock->pbsock, &numwritten);
      /** send worker number h its ID **/
      if(wcode) {retcode =1 ; goto BACK;}
      just_read(pmastersock->pbsock);
      printf("got %s from worker %d\n", pmastersock->pbsock->pBuffer, h);

    }

    for(h = 0; h < Num; h++){
      pmastersock = ppmastersockarr[h]; /** launch each thread **/
      pthread_create(&pmastersock->thread_id, NULL, &interaction, (void *) pmastersock);
    }
    for(h = 0; h < Num; h++){
      pmastersock = ppmastersockarr[h];  /** join with each thread **/
      pthread_join(pmastersock->thread_id, NULL);
    }

    goto DONE;

 DONE:
    printf("\n<closing sockets.>\n"); 
    /* close socket */
    for(h = 0; h < Num; h++){
      pmastersock = ppmastersockarr[h];
      if(close(pmastersock->pbsock->hSocket) == SOCKET_ERROR){
	printf("\nCould not close socket %d\n", h);
	retcode = 1;
      }
    }

    usleep(1000);


 BACK:
    return retcode;
}


void *interaction(void *voidedmastersock)
{
  mastersock *pmastersock = (mastersock *) voidedmastersock;
  baggedsock *pbsock = pmastersock->pbsock;
  char oldb[100], stopit;
  int wcode = 0, numwritten;

  for(;;){
    pthread_mutex_lock( &synchro);
    ++*pmastersock->pjobcount;
    if(*pmastersock->pjobcount > *pmastersock->pK){
      /** stopit evaluates true if we have done enough work **/
      stopit = 1;
      *pmastersock->pjobcount = *pmastersock->pK; /* no need to increase it */
    }
    else{
      stopit = 0;
      printf("\n[master thread %d starting loop %d jobcount %d\n", 
	   pbsock->id, pmastersock->localcount, *pmastersock->pjobcount);
      sprintf(pmastersock->ackphrase,"MASTERACK%d#%d", pmastersock->pbsock->nHostPort,
	    *pmastersock->pjobcount);
    }

    pthread_mutex_unlock( &synchro);


    if(stopit){

      pthread_mutex_lock( &synchro);
      printf("\n>>> ordering worker %d to stop; total work %d\n", pbsock->id,
	     *pmastersock->pjobcount);
      pthread_mutex_unlock( &synchro);

      strcpy(pbsock->pBuffer,"stop");
      wcode = write2socket(pmastersock->pbsock, &numwritten);
      if(wcode) {pmastersock->runcode = 1 ; goto DONE;}

      goto DONE;
    }

    strcpy(pbsock->pBuffer,"cont");
    wcode = write2socket(pmastersock->pbsock, &numwritten);
    if(wcode) {pmastersock->runcode =1 ; goto DONE;}


    /** now receive the data from worker as it works **/

    for(;;){
        /* read from socket into buffer */

      just_read(pmastersock->pbsock);
      /*      read_and_ack(pbsock, pmastersock->ackphrase);*/


      printf("got '%s' from %d\n", pbsock->pBuffer, pbsock->id); 
        
      if(strcmp(pbsock->pBuffer,"finished") == 0){
	printf("**worker %d sent 'finished', (last sent is %s)\n", pbsock->id, 
	      oldb); 
	break;
	/** note that in this case the master read but did not ack **/
      }
      just_write(pbsock, pmastersock->ackphrase); /** send ack to the last read **/


      sprintf(oldb, pbsock->pBuffer);


    }


    ++pmastersock->localcount;
  }

 DONE:
  return NULL;
}


