#ifndef GSOCK

#define GSOCK

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         2000
#define QUEUE_SIZE          5
#define HOST_NAME_SIZE 200
#define MASTERACK "MASTERACK"
#define WORKERACK "WORKERACK"

typedef struct baggedsock{
  int id;
  char *pBuffer;
  char *pAck;  /** not strictly needed **/
  int hSocket, hServerSocket;
  struct sockaddr_in Address; 
  int nAddressSize;
  int nHostPort;
}baggedsock;


int creategsock(baggedsock **ppbsock, int id);
int startupsock(baggedsock *pbsock);
int just_read(baggedsock *pbsock);
int read_and_ack(baggedsock *pbsock, char *ackphrase);
int gotrybind(baggedsock *pbsock, int start);
int finishopenstuff(baggedsock *pbsock);
int write2socket(baggedsock *pbsock, int *pnumwritten);
int startconnection(baggedsock *pbsock);
#endif
