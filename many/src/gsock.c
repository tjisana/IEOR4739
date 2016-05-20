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

int creategsock(baggedsock **ppbsock, int id)
{
  int retcode = 0;
  baggedsock *pbsock = NULL;

  pbsock = (baggedsock *) calloc(1, sizeof(baggedsock));
  if(pbsock == NULL){
    printf(" cannot create gsock\n"); retcode = 1; goto BACK;
  }
  *ppbsock = pbsock;

  pbsock->pBuffer = (char *)calloc(2000,sizeof(char));
  if(pbsock->pBuffer == NULL){printf("no pBuf\n"); retcode = 1; goto BACK;}
  pbsock->pAck = pbsock->pBuffer + 1000;

 BACK:
  return retcode;
}

int read_and_ack(baggedsock *pbsock, char *ackphrase)
{
  int retcode = 0;

  read(pbsock->hSocket,pbsock->pBuffer,BUFFER_SIZE);

  if(ackphrase) strcpy(pbsock->pAck, ackphrase);
  else strcpy(pbsock->pAck, "ACK");
  write(pbsock->hSocket, pbsock->pAck, strlen(pbsock->pAck)+1);

  return retcode;
}

int ack(baggedsock *pbsock)
{
  int retcode = 0;

  write(pbsock->hSocket, pbsock->pAck, strlen(pbsock->pAck)+1);

  return retcode;
}

int just_write(baggedsock *pbsock, char *buffer)
{
  int retcode = 0;

  write(pbsock->hSocket, buffer, strlen(buffer) + 1);

  return retcode;
}

int just_read(baggedsock *pbsock)
{
  int retcode = 0;

  retcode = (read(pbsock->hSocket,pbsock->pBuffer,BUFFER_SIZE) <= 0);

  return retcode;
}

int startupsock(baggedsock *pbsock)
{
  int retcode = 0;

  /* create socket */

  pbsock->hServerSocket = socket(PF_INET,SOCK_STREAM,0);

  if(pbsock->hServerSocket == SOCKET_ERROR){
    printf("\nCould not make socket %d\n", pbsock->id); 
    retcode = 1; goto BACK;
  }


  /* address stuff */
  pbsock->Address.sin_addr.s_addr=INADDR_ANY;
  pbsock->Address.sin_family=AF_INET;

  pbsock->nAddressSize = sizeof(struct sockaddr_in);


 BACK:
  return retcode;
}

int gotrybind(baggedsock *pbsock, int start)
{
  int retcode = 0, lastav = 60000, j;

  for(j = start; j <= lastav; j++){
    pbsock->nHostPort = j;
    pbsock->Address.sin_port=htons(pbsock->nHostPort);
    printf("binding gsocket %d to port %d",pbsock->id, pbsock->nHostPort);
    fflush(stdout);

    if(bind(pbsock->hServerSocket,(struct sockaddr*)&pbsock->Address,
	    sizeof(pbsock->Address)) 
       == SOCKET_ERROR){
      printf(" Could not connect to port %d\n", j);     fflush(stdout);
    }
    else{
      break;
    }
  }

  if(j > lastav){
    printf(" ==>> could not bind\n"); retcode = 1; goto BACK;
  }

  printf(" : bound to port %d\n", pbsock->nHostPort);     fflush(stdout);
 BACK:
  return retcode;
}

int finishopenstuff(baggedsock *pbsock)
{
  int retcode = 0;

  getsockname( pbsock->hServerSocket, 
	       (struct sockaddr *) &pbsock->Address,
	       (socklen_t *)&pbsock->nAddressSize);
  printf("opened socket as fd (%d) on port (%d) for stream i/o\n",
	pbsock->hServerSocket, ntohs(pbsock->Address.sin_port) );
  fflush(stdout);
  
  printf("Server for %d\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
	, pbsock->id, pbsock->Address.sin_family
	, pbsock->Address.sin_addr.s_addr
	, ntohs(pbsock->Address.sin_port)
	);
  
  printf("\nusing a listen queue of %d elements",QUEUE_SIZE);

  fflush(stdout);
  /* establish listen queue */
  
  
  return retcode;
}

int write2socket(baggedsock *pbsock, int *pnumwritten)
{
  int retcode = 0;

  *pnumwritten = write(pbsock->hSocket, pbsock->pBuffer,
      strlen(pbsock->pBuffer)+1);

  retcode = (*pnumwritten <= 0);

  return retcode;
}

int startconnection(baggedsock *pbsock)
{
  int retcode = 0;
  printf("waiting for connection with %d\n", pbsock->id); fflush(stdout);
  /* get the connected socket */

  if(listen(pbsock->hServerSocket,QUEUE_SIZE) == SOCKET_ERROR){
    printf("Could not listen to worker %d\n", pbsock->id);
    fflush(stdout);
    retcode = 1;
    goto BACK;
  }

  pbsock->hSocket = accept(pbsock->hServerSocket,
			   (struct sockaddr*)&pbsock->Address,
			   (socklen_t *)&pbsock->nAddressSize);

  printf("a connection with %d", pbsock->id); fflush(stdout);

 BACK:
  return retcode;
}
