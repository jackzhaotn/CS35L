#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

  char c[1];
  ssize_t charsRead = read(0, c, 1);
  for (;;) {
    if (charsRead == 0)
      break;
    if (charsRead < 0) {
      fprintf(stderr, "Error: Invalid input");
      exit(1);
    }
    for (int i = 0; i != strlen(from); i++) {
      if (c[0] == from[i]) {
	c[0] = to[i];
	break;
      }
    }
    write(1, c, 1);
    charsRead = read(0, c, 1);
  }
  return 0;
}
