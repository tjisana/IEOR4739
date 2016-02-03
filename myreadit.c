#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int mygetline(char **pbuffer, int *pn, int *pread_in, FILE *in);

int main(int argc, char **argv)
{
  int retcode = 0;
  FILE *input = NULL;
  char *buffer = NULL;
  char **lineptr;
  int n = 10, read_len = 0, readcode = 0;
  int j;

  if(argc != 2){
    printf("usage: readit filename\n");
    retcode = 1; goto BACK;
  }

  input = fopen(argv[1], "r");
  if(!input){
    printf("cannot open file %s\n", argv[1]); retcode = 1; goto BACK;
  }

  buffer = (char *)calloc(n, sizeof(char));
  lineptr = &buffer;

  printf("start. buffer at %p, n: %d -> %d\n", *lineptr, (int) n, (int) strlen(buffer));
  for(j = 0; j < 2 ; j++){
    readcode = mygetline(lineptr, &n, &read_len, input);
    if(readcode > 0){
      printf("cannot read line %d\n", j); retcode = 1; break;
    }
    if(read_len > 0){
      printf("    %d. buffer at %p, n: %d -> %d, read_len: %d\n", j, *lineptr,  
	   n, (int) strlen(buffer), read_len);
      printf("   %s", buffer);
    }
    if(readcode < 0){
      break;
    }

  }
  /*  printf("%d %s\n", (int) read_len, lineptr);*/

  

  fclose(input);

 BACK:
  return retcode;
}

int mygetline(char **pbuffer, int *pn, int *pread_in, FILE *in)
{
  char *buffer;
  int k, s;
  int returncode = 0;


  buffer = *pbuffer; /* so now we have the buffer */


  for(k = 0; ;k++){
    if( (s = fgetc(in)) == EOF ){
      if(k > 0){
	printf(" ==> %s\n", buffer);
	printf("note: last line in file does not end with carriage return: appending one\n");
	buffer[k] = '\n';
      }
      returncode = -1;
      break;
    }
    buffer[k] = (char) s;
    if( buffer[k] == '\n'){
      break;
    }
    if (k >= *pn - 1){
      /** nead more space **/
      printf("    current buffer at %p of size %d is too small\n", buffer, *pn);
      buffer = (char *) realloc(buffer, 10*(*pn));
      *pbuffer = buffer;
      *pn *= 10;
      printf("      reallocated to buffer at %p of size %d\n", buffer, *pn);
    }
  }

  *pread_in = k;

  return returncode;
}
