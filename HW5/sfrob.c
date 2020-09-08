#include <stdio.h>
#include <stdlib.h>

//compares unfrobicated versions of the char arrays
int frobcmp (char const* a, char const* b) {

  int i = 0;
  char c1 = a[i];
  char c2 = b[i];
  
  for (;;) {
    if (c1 == ' ' && c2 == ' ')
      return 0;
    else if (c1 == ' ')
      return -1;
    else if (c2 == ' ')
      return 1;
    else if ((c1 ^ 42) > (c2 ^ 42))
      return 1;
    else if ((c1 ^ 42) < (c2 ^ 42))
      return -1;
    else {
      i++;
      c1 = a[i];
      c2 = b[i];
    }
  }
}

int compare(const void* a, const void* b) {
  return frobcmp(*(char**) a, *(char**) b);
}

//function to print memory alloc error and exit
void merror()
{
  fprintf(stderr, "Memory Allocation Error.");
  exit(1);
}

//function to print I/O error and exit
void IOcheck()
{
  if (ferror(stdin)) { 
    fprintf(stderr, "Input error");
    exit(1);
  }
}


  
int main(){

  //malloc for one char
  char* word = (char*) malloc(sizeof(char));
  //malloc for one row and one cell of a 2d array
  char** wordList = (char**) malloc(sizeof(char*));
  if ( (word == NULL) | (wordList == NULL) )
    merror();
  
  int c = getchar();
  IOcheck();
  int charCount = 0;
  int wordCount = 0;

  //loops untill EOF
  while (c != EOF) {
    //end of a word 
    if (c == ' ') {
      word[charCount] = c;
      //add word to the word list
      wordList[wordCount] = word;
      wordCount++;
      //realloc the word list for one more row
      wordList = (char**) realloc(wordList, (wordCount + 1) * sizeof(char*));
      if (wordList == NULL){
	free(word);
	for (int i = 0; i <= wordCount; i++)
	  free(wordList[i]);
	free(wordList); 
	merror();
      }
      charCount = 0;
      //reset word memory
      word = (char*) malloc(sizeof(char));
      if (word == NULL){
        free(word);
        for (int i = 0; i <= wordCount; i++)
          free(wordList[i]);
        free(wordList);
	merror();
      }
    }

    else {
      //add char to the 'string'
      word[charCount] = c;
      charCount++;
      //realloc another byte 
      word = realloc(word, (charCount + 1) * sizeof(char));
      if (word == NULL) {
	free(word);
        for (int i = 0; i <= wordCount; i++)
          free(wordList[i]);
        free(wordList);
	merror();
      }
    }

    c = getchar();
    IOcheck();
    //adds last word before EOF to the wordlist
    if (c == EOF) {
      word[charCount] = ' ';
      wordList[wordCount] = malloc(charCount * sizeof(char));
      if (wordList[wordCount] == NULL) {
        free(word);
        for (int i = 0; i <= wordCount; i++)
          free(wordList[i]);
        free(wordList);
	merror();
      }
      wordList[wordCount] = word;
    }
    
  }

  //sort the word list with compare() function
  qsort(wordList,(wordCount+1), sizeof(char*), compare);

  //print each word by accessing each element in the 2d array
  for (int i = 0; i <= wordCount; i++) {
    for (int j = 0; wordList[i][j] !=  ' '; j++){
      putchar(wordList[i][j]);
    }
    putchar(' ');
  }

  //free memory
  free(word);
  for (int i = 0; i <=wordCount; i++) {
    free(wordList[i]);
  }
  free(wordList);
}
