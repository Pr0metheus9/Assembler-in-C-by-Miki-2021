/*include datastructs/defintion file*/
#include "datastructs.h"

/*check if current part of line is a label*/
int check_label(char *line, char label_array[32])
{
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
    if(label_exists(label_array))
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

/*check if part of line is a directive if so return the "type" of directive*/
int check_dir(char *line)
{
  /*compare to data*/
  if(!strncmp(directives[0].str, line+1, 4)){
		return 0;
  }
  /*compare to string*/
  else if(!strncmp(directives[1].str, line+1, 6)){
    return 1;
  }
  /*compare to entry*/
  else if(!strncmp(directives[2].str, line+1, 5)){
    return 2;
  }
  /*compare to extern*/
  else if(!strncmp(directives[3].str, line+1, 6)){
    return 3;
  }
}