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

  /*check if it is a label if so move to the next part of the line*/
  if(check_label2(line,label_array))
  {
    line = nextpart(line);
  }

  /*remove whitespace from the line to make it easily readable*/
  line = clearspace(line);

  /*Check if part could be a directive. if it is a valid directive*/
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

  
  /*instruction check if part is an instruction.*/ 
  if(check_intruction(line,opcode,funct))
  {
    line = nextpart(line);
    complete_instruction(line,labelFlag,opcode,funct,label_array);
  }
  return 0;
}

/*function to complete the missing parts in the instruction array (step 6 of algorithm)*/
int complete_instruction(char *line,boolean labelFlag,int *opcode, int *funct,char label_array[32])
{
  int L;
  int i;

  /*sets the right number of operators to be given in any type of instruction part 15 of algorithm*/
  if(*opcode <= 4)
  {
    L = 2;
  }

  else if(*opcode>=5 && *opcode<=13)
  {
    L = 1;
  }

  else if(*opcode>13)
  {
    L = 0;
  }

  /*increments the temporary IC since every instruction has an image*/
  tIC++;

  /*loop through number of words created as a result of the instruction*/
  for(i = 1; i <= L; i++)
  {
    /*Instant addressing (value of 0)*/ 
    if(*line == '#')
    {
      /*move on*/
      tIC++;
      line = nextpart(line);
      clearspace(line);
      continue;
    }

    /*relative addressing (value 2)*/
    else if(*line == '%')
    {
      int content;
      int targetIC;
      int dist;
      char are;

      line++;

      /*gets the label operator*/
      if(getlabelsec(line,label_array))
      {
        if(label_exists(label_array))
        {
          targetIC = get_label(label_array)->value;
        }
      }

      /*calculates the distance to the label*/
      dist = targetIC - (tIC-1);

      if(dist>=0)
      {
        content = dist;
      }

      /*if the distance in negative calculate its twos complement*/
      else
      {
        content = ~(dist)+1;
      }

      if(get_label(label_array) -> attribute == 3)
        {
          are = 'E';
        }

      else
      {
        are = 'R';
      }

      /*insert the value into the code_array*/
      code_array[tIC-100].content = content;
      code_array[tIC-100].are= are;


      /*move on*/
      line = nextpart(line);
      clearspace(line);
      tIC++;
      continue;
     }

     /*direct register addressing (3)*/
     /*if operator is a register move on since it has no extra word*/
     else if(*line == 'r')
     {
        tIC++;
        line = nextpart(line);  
        clearspace(line);
        printf("line: %c\n",*(line));
        continue; 
     }

    /*direct addresing (1) (temporary, if not everything else)*/
    /*gets the label operator*/
    else if (getlabelsec(line,label_array))
    {
      int targetIC;
      int content;
      char are;

      /*checks if the label exists in the label list*/
      if(label_exists(label_array))
      {
        Label *templabel;
        templabel = get_label(label_array);
        targetIC = templabel -> value;

        /*inserts the IC of the label into the word*/
        content = targetIC;

        if(templabel -> attribute == 3)
        {
          are = 'E';
        }

        else
        {
          are = 'R';
        }

        /*insert the value into the code_array*/
        code_array[tIC-100].content = content;
        code_array[tIC-100].are = are;

        /*move on*/
        line = nextpart(line);
        clearspace(line);
        tIC++;
        continue;
      }
    }
  }
  return 0;
}

/*finds a lable thats at the start of a line (ends with :)*/
int check_label2(char *line, char label_array[32])
{
  int i = 0;
  
  /*move to the end of the label and save it (end is the char :)*/
  while(*line != ':' && *line != '\0')
  {
    label_array[i] = *line;
    i++;
    line++;
  }

  label_array[i]='\0';

  if(i==0)
    return 0;

  /*if you reach the end of the label, check if the saved label is a valid one */
  if(*line == ':')
  {
      return 1;
  }

  return 0;

}