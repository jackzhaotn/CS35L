#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function to print I/O error and exit
void inErr()
{
  if (ferror(stdin)) {
    fprintf(stderr, "Input error");
    exit(1);
  }
}

void outErr()
{
  if (ferror(stdout)) {
    fprintf(stderr, "Output error");
    exit(1);
  }
}


int main (int argc, char** argv)
{
  //check to see there are two arguments
  if (argc != 3) {
    fprintf(stderr, "Error: incorrect number of arguments");
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];

  //check arguments for same length
  if ( strlen(from) != strlen(to) ){
    fprintf(stderr, "Error: arguments must be of same length");
    exit(1);
  }

  //check for duplicate bytes in 'from' argument
  for (int i = 0; i != strlen(from); i++) {
    for(int j = i +1 ; j != strlen(from); j++) {
      if ( from[i] == from[j] ) {
	fprintf(stderr, "Error: Duplicate bytes in first argument");
	exit(1);
      }
    }
  }

  //tr function
  int c = getchar();
  inErr();
  while (c != EOF) {
    for (int i = 0; i != strlen(from); i++) {
      //if byte matches a byte in from, putchar the corresponding byte
      if (c == from[i]) {
	c = to[i];
	break;
      }
    }
    putchar(c);
    outErr();
    c = getchar();
    inErr();
  }
  return 0;
}
