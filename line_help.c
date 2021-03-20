/*include datastructs/defintion file*/
#include "datastructs.h"

/*function to get the next significant part of the line*/
char *nextpart(char *line){
  while(*line != ' ' && *line != '\t' && *line != ',' && *line != ':'){
    if(*line == '\0')
      return line;
    line++;
  }
  line++;
  line = clearspace(line);
  return line;
}

/*remove the whitespace (skip it) by moving the line every time and checking if it is pointing to a whitespace.*/
char *clearspace(char *line){
  while(*line==' ' || *line=='\t'){
    if(*line == '\0')
      break;
    line++;
  }
  return line;
}

