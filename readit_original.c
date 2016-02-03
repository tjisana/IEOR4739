#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
  int retcode = 0;
  FILE *input = NULL;
  char *buffer = NULL;
  char **lineptr;
  size_t n = 800;
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

  buffer = (char *)calloc(1000, sizeof(char));
  lineptr = &buffer;

  for(j = 0; j < 2 ; j++){
    read_len = getline(lineptr, &n, input);
    printf("%d %d %d\n", j, (int) read_len, (int) n);
    printf("   %s\n", *lineptr);
    if(read_len <= 0) break;
  }
  /*  printf("%d %s\n", (int) read_len, lineptr);*/

  

  fclose(input);

 BACK:
  return retcode;
}
