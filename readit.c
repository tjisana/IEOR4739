#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv)
{
  int retcode = 0;
  FILE *input = NULL;
  char *buffer = NULL;
  char **lineptr;
  size_t n = 10;
  ssize_t read_len = 0;
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
    read_len = getline(lineptr, &n, input);
    printf("    %d. buffer at %p, n: %d -> %d, read_len: %d\n", j, *lineptr, (int) n, (int) strlen(buffer), (int) read_len);
    /*    printf("   %s\n", *lineptr);*/
    if(read_len <= 0) break;
  }
  /*  printf("%d %s\n", (int) read_len, lineptr);*/

  

  fclose(input);

 BACK:
  return retcode;
}
