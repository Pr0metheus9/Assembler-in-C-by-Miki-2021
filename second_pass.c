/*include datastructs/defintion file*/
#include "datastructs.h"

int second_pass (char *fileName)
{
    /* open the file and save file pointer */
    FILE* file = fopen(fileName, "r"); 
    
    /*create char to save current line content. max size for a line is 500 chars*/
    char line[501]; 

    /*create flags and set initial value of flags to false*/
    boolean errorFlag = False; 
    boolean labelFlag = False;

    /*create label array to store label if it exists*/
    char label_array[32]; 

    /*create ints to store opcode and funct if line part is a instruction*/
    int opcode = 0;
    int funct = 0;

    /*set initial values for tIC and tDC*/
    tIC = 100;
    tDC = 0;

    /*Check if the file cannot be opened, if so print error to screen */
    if(file == NULL) 
    {
      printf("Error: File Cannot Open");   
      exit(1);             
    }

    /*go over each line in file*/
    while (fgets(line, sizeof(line), file)) 
    {
      /*call function to translate line into things we can work with each time*/
      translate_line2(line,errorFlag,labelFlag,label_array,&opcode,&funct);
    }

    /*close the file being used*/
    fclose(file); 
    return 1;
}

/*function to do stuff with the line and tables. Works using the second scan algorithm*/
int translate_line2 (char *line,boolean errorFlag,boolean labelFlag, char label_array[32],int *opcode,int *funct)
{
  /*flag to tell if label was found*/
  labelFlag = False;

  /*check if the line is of comment or whitespace type if so we can just skip it*/
  if(*line == ';')
  {
    return 0;
  }

  /*Clears empty space*/
  if(*line == ' ' || *line == '\t')
  {
    line = clearspace(line);
  }

  
  if(line == NULL || *line == '\n' || *line == '\0')
  {
    return 0;
  }

  /*check if it is a label if so set the label flag to true (following algorithm) and move to the next part of the line*/
  if(check_label(line,label_array))
  {
    return 0;
  }

  /*remove whitespace from the line to make it easily readable*/
  line = clearspace(line);

  /*Check if part could be a directive. if it is a valid directive, save to the word table and continue*/
  if(*line == '.')
  {
    int type = check_dir(line);
    /*check if the directive is of the entry type (step 4 of algorithm). if not then return 0 (step 3 of algorithm)*/
    if(type != 2)
    {
      return 0;
    }
    
    else
    {
      line = nextpart(line);

      if(getlabelsec(line,label_array))
      {
        if(label_exists(label_array))
        {
          /*if so add the entry type to the current type of the label (step 5 of algorithm)*/
          get_label(label_array) -> isEntry = 1;
        }
      }
    }
  }

  line = clearspace(line);

  /*instruction check if part is an instruction. if it is then move to the next part and save to table*/ 
  /*if(check_intruction(line,opcode,funct))
  {
    line = nextpart(line);
    insert_instruction(line,labelFlag,opcode,funct,label_array);
    return 0;
  }

  else
  {
    return 1;
  }*/
}
