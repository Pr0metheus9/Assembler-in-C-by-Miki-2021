/*include datastructs/defintion file*/
#include "datastructs.h"

/*function to get the next significant part of the line*/
char *nextpart(char *line){
  while(*line != ' ' && *line != '\t' && *line != ',' && *line != ':')
  {
    if(*line == '\0')
    {
      line++;
      return line;
    }
    line++;
  }
  line++;
  line = clearspace(line);
  return line;
}

/*remove the whitespace (skip it) by moving the line every time and checking if it is pointing to a whitespace.*/
char *clearspace(char *line)
{
  while(*line==' ' || *line=='\t')
  {
    if(*line == '\0')
    {
      break;
    }
    line++;
  }
  return line;
}

/*gets the value of the next number from place in line*/
int get_num(char *line){
  int n = 0;
  int num = 0;
  int i;
  
  /*while the current char is a digit increment the digit count*/
  while(isdigit(*line)){
    n++;
    line++;
  }
  line -= n;

  /*loops through the digits and calculates the integer value*/
  for(i = n-1; i >= 0 ; i--){
    int x = *line - '0';
    num += (int) x * pow(10.0,(double)i);
    line++;
  }
  return num;
}

/*function to display the data types*/
void display_data()
{
  /*loop over all the words\images saved and print them to the screen*/
  int i;
  for(i = 0; i < DC; i++)
  {
    printf("%d\t%d\t%c\n",i,data_array[i].content,data_array[i].are);
  }
}

/*function to display the code types*/
void display_code()
{
  /*loop over all the words\images saved and print them to the screen*/
  int i;
  for(i = 0; i < IC-100; i++)
  {
    printf("%d\t%d\t%c\n",i,code_array[i].content,code_array[i].are);
  }
}

