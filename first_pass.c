/*include datastructs/defintion file*/
#include "datastructs.h"

/*function that does the first pass it reads input file line and uses other functions to translate
we will use algorithm for two pass assembler*/
int first_pass (char *fileName)
{
    /* open the file and save file pointer */
    FILE* file = fopen(fileName, "r"); 
    
    /*Check if the file cannot be opened, if so print error to screen */
    if(file == NULL) 
    {
      printf("Error: File Cannot Open");   
      exit(1);             
    }

    /*create char to save current line content. max size for a line is 500 chars*/
    char line[501]; 
    /*set initial value for instruction counter*/
    int IC = 100; 
    /*set initial value for data counter*/
    int DC = 0; 
    /*create flags and set initial value of flags to false*/
    boolean errorFlag = False; 
    boolean labelFlag = False;

    /*go over each line in file*/
    while (fgets(line, sizeof(line), file)) 
    {
      /*call function to translate line into things we can work with each time*/
      translate_line(line,errorFlag,labelFlag);
    }

    /*close the file being used*/
    fclose(file); 
}

/*function to do stuff with the line and tables*/
int translate_line (char *line,boolean errorFlag,boolean labelFlag)
{
  /*check if the line is of comment or whitespace type if so we can just skip it*/
  if(*line == ';' || *line == '\t' || *line == '\n' || *line == ' ')
    return 0;
  
  /*check if it is a label*/
  if(check_label(line))
  {
    labelFlag = True;
    line = nextpart(line); 
  }
}

/*check if current part of line is a label*/
int check_label (char *line)
{
  char label_array[32]; 
  /*create array to store label characters as we go char by char in line until we find the end of the label (if it exists) max length is 31*/
  int i = 0;

  /*until you find something indicating the end of the label, add char to array and keep going over char*/
  while(*line != ':' && *line != '\0')
  {
    label_array[i] = *line;
    i++;
    line++;
  }

  /*so it is saved as a string data type*/
  label_array[i] = '\0'; 

  if(*line == ':')
  {
    /*check if label starts with letter*/
    if(!((label_array[0] >= 'a' && label_array[0] <= 'z')||(label_array[0] >= 'A' && label_array[0] <= 'Z')))
      return 0;
    
    /*check if : shows up after whitespace*/
    if(label_array[i-1] == ' ' || label_array[i-1] == '\t')
      return 0;

    /*check if label with same name exists if it does return 0*/
    if(!label_exists(label_array))
      return 0;

    /*check if label has the same name as instruction*/
    for(i = 0; i < 16; i++)
    {
      if(!strcmp(instructions[i].str, label_array))
        return 0;
    }

    /*check if label has the same name as register*/
    for(i = 0; i < 8; i++)
    {
      if(!strcmp(registers[i].str, label_array))
        return 0;
    }

    /*check if label has the same name as a directive*/
    for(i = 0; i < 4; i++)
    {
      if(!strcmp(directives[i].str, label_array))
        return 0;
    }
  }
  return 1;
}

/*function to get the next significant part of the line*/
char *nextpart(char *line){
  while(*line != ' ' && *line != '\t' && *line != ',' && *line != ':'){
    if(*line == '\0')
      return line;
    line++;
  }
  line++;
  return line;
}
