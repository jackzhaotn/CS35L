#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

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

//Case-insensitive version
int frobcmpF (char const* a, char const* b) {

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
    else if (toupper((c1 ^ 42)) > toupper((c2 ^ 42)))
      return 1;
    else if (toupper((c1 ^ 42)) < toupper((c2 ^ 42)))
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

int compareF(const void* a, const void* b) {
  return frobcmpF(*(char**) a, *(char**) b);
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

void rerror()
{
  fprintf(stderr, "Read in Error");
  exit(1);
}

int main(int argc, char const *argv[]){

  int f = 0;
  if (argc > 2) {
    fprintf(stderr, "Error: Too many arguments");
    exit(1);
  }
  if (argc == 2) {
    if (strcmp(argv[1], "-f") == 0) {
      f = 1;
    } else {
      fprintf(stderr, "Error: Invalid option");
      exit(1);
    }
  }

  struct stat fileData;
  if (fstat(0, &fileData) < 0) {
    fprintf(stderr, "Error: Fstat error");
    exit(1);
  }

  char* buffer;
  long bufferSize;
  if (S_ISREG(fileData.st_mode)) {
    bufferSize = fileData.st_size;
    buffer = (char*) malloc(bufferSize * sizeof(char));
    if (buffer == NULL) {
      free(buffer);
      merror();
    }
  } else {
    bufferSize = 8000;
    buffer = (char*) malloc(bufferSize * sizeof(char));
    if (buffer == NULL) {
      free(buffer);
      merror();
    }
  }

  long bytesRead = read(0, buffer, bufferSize);
  long bufferCount = bytesRead;
  if (bytesRead < 0) {
    free(buffer);
    rerror();
  }
  while(bytesRead != 0) {
    if (bufferCount > bufferSize/2) {
      bufferSize *= 2;
      buffer = (char*) realloc(buffer, bufferSize * sizeof(char));
      if (buffer == NULL) {
	free(buffer);
	merror();
      }
    }
    bytesRead = read(0, &buffer[bufferCount], bufferSize);
    if (bytesRead < 0) {
      free(buffer);
      rerror();
    }
    bufferCount += bytesRead;
  }
  
 
  //malloc for one char
  char* word = (char*) malloc(sizeof(char));
  //malloc for one row and one cell of a 2d array
  char** wordList = (char**) malloc(sizeof(char*));
  if ( (word == NULL) | (wordList == NULL) )
    merror();

  ssize_t count = 0;
  int c = buffer[count];
  IOcheck();
  int charCount = 0;
  int wordCount = 0;

  //loops untill EOF
  while (count != bufferCount) {
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
	free(buffer);
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
	free(buffer);
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
	free(buffer);
	merror();
      }
    }

    
    count++;
    //adds last word before EOF to the wordlist
    if (count == bufferCount) {
      if (buffer[count-1] != ' ') {
	c = ' ';
	word[charCount] = c;
	wordList[wordCount] = word;
	wordCount++;
      }
    }  else {
      c =  buffer[count];
      IOcheck();
    }
    
  }

 
  //sort the word list with compare() function
  if(f == 1) {
    qsort(wordList,(wordCount), sizeof(char*), compareF);
  } else {
    qsort(wordList,(wordCount), sizeof(char*), compare);
  }
  
  ssize_t wordLength, bytesWritten;
  //print each word by accessing each element in the 2d array
  for (int i = 0; i < wordCount; i++) {
    wordLength = strlen(wordList[i]);
    bytesWritten = write(1, wordList[i], wordLength);
    if (bytesWritten < 0) {
      fprintf(stderr, "Write out error");
      exit(1);
    }
    
  }

  //free memory
  for (int i = 0; i <wordCount; i++) {
    free(wordList[i]);
  }
  free(wordList);
  free(buffer);
}
