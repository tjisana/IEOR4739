#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gsock.h"

int compare2substr(char *buffer, char *target);

int workeralgorithm(int hSocket, char *pBuffer);

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
    fflush(stdout);

    sprintf(pBuffer,"pong");
    printf("Writing \"%s\" to master ",pBuffer); fflush(stdout);
    write(hSocket,pBuffer, strlen(pBuffer) + 1);

    retcode = workeralgorithm(hSocket, pBuffer);



    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR){
      printf("\nCould not close socket\n");
      retcode = 1;
    }

 BACK:
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
