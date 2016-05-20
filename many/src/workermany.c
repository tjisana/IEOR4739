#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "gsock.h"


int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    char ack[100];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort, N, ID;
    int retcode = 0, count;

    if(argc != 4){
	printf("\nUsage: worker host-name host-port N\n");
	return 0;
    }
    else{
	strcpy(strHostName,argv[1]);
	nHostPort = atoi(argv[2]);
	N = atoi(argv[3]); /** amount of 'work' per call **/
    } 

    sprintf(ack,"MASTERACK%d", nHostPort);
    printf("\nCreating the socket"); 
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR){
      printf("\nCould not make a socket\n");
      retcode = 1; goto BACK;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr = nHostAddress;
    Address.sin_port = htons(nHostPort);
    Address.sin_family = AF_INET;

    printf("\nConnecting to %s on port %d",strHostName,nHostPort);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nworker could not connect to host\n");
        retcode = 1; goto BACK;
    }



    nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
    printf("\nReceived \"%s\" from master (%d bytes)\n",pBuffer, nReadAmount);
    if(nReadAmount <= 0){
      retcode = 1; goto BACK;
    }

    ID = atoi(pBuffer + 4); 
    /** pBuffer should be of the form 'hey_k' where k is the ID, so 
     the above line extracts the ID**/
    printf("\n Using ID = %d\n", ID);
    sprintf(pBuffer,"OKfrom%d", ID);
    printf("Writing \"%s\" to master\n",pBuffer); fflush(stdout);
    write(hSocket,pBuffer, strlen(pBuffer) + 1); /** send ack back **/

    int k;

    for(count = 0;; ++count){
      printf("starting signal loop  %d for this worker\n", count); fflush(stdout);

      /** next, wait to be told to stop or continue **/
      printf("now entering stop/cont loop %d\n", count); fflush(stdout);
      
      for(;;){
	usleep(5000);
	nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
	if(strcmp(pBuffer,"stop")==0){
	  printf("got stop\n"); fflush(stdout);
	  goto DONE;
	}
	else if(strcmp(pBuffer,"cont")==0){
	  printf("got cont\n"); fflush(stdout);
	  break;
	}
	else{
	  printf("got illegal %s\n", pBuffer); goto DONE;
	}
      }
      /** begin to work **/

      /** the 'work' is to send back N integers.  we modify the buffer being
	  sent back by appending the worker ID.  **/

      for(k = 0; k < N; k++){
	usleep(1000000); /** sleep a second so that the program runs slower **/

	sprintf(pBuffer,"%d_%d", k, ID);
	printf("Writing \"%s\" to master ",pBuffer); fflush(stdout);

	write(hSocket,pBuffer, strlen(pBuffer) + 1);
	nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
	printf(", got ack %s\n", pBuffer); fflush(stdout);
      }
      /** when done with the "work", send 'finished' **/
      sprintf(pBuffer,"finished");
      write(hSocket,pBuffer, strlen(pBuffer) + 1);
      /*      nReadAmount=read(hSocket,pBuffer,BUFFER_SIZE);
	      printf("got ack %s for 'finished'\n", pBuffer); fflush(stdout);*/

    }
    
 DONE:    
    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR){
      printf("\nCould not close socket\n");
      retcode = 1;
    }


 BACK:
    return retcode;
}



