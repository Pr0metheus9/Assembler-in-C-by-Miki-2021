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

    /*create label array to store label if it exists*/
    char label_array[32]; 

    /*create ints to store opcode and funct if line part is a instruction*/
    int opcode;
    int funct;

    /*go over each line in file*/
    while (fgets(line, sizeof(line), file)) 
    {
      /*call function to translate line into things we can work with each time*/
      translate_line(line,errorFlag,labelFlag,label_array);
    }

    /*close the file being used*/
    fclose(file); 
}

/*function to do stuff with the line and tables*/
int translate_line (char *line,boolean errorFlag,boolean labelFlag, char label_array[32])
{
  /*check if the line is of comment or whitespace type if so we can just skip it*/
  if(*line == ';' || *line == '\t' || *line == '\n' || *line == ' ')
    return 0;
  
  /*check if it is a label if so set the label flag to true (following algorithm) and move to the next part of the line*/
  if(check_label(line,label_array))
  {
    labelFlag = True;
    line = nextpart(line); 
  }

  /*remove whitespace from the line to make it easily readable*/
  line = clearspace(line);

  /*Check if part could be a directive. if it is a valid directive, save to the word table and continue*/
  if(*line == '.')
  {
    int type = check_dir(line);
    line = nextpart(line);
  }
}