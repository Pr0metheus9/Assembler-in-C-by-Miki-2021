/*include datastructs/defintion file*/
#include "datastructs.h"


/*keywords in asm*/

/*all the possible instructions*/
keyword instructions[] = {
  {"mov"},{"cmp"},{"add"},{"sub"},{"lea"},
  {"clr"},{"not"},{"inc"},{"dec"},{"jmp"},{"bne"},
  {"jsr"},{"red"},{"prn"},{"rts"},{"stop"}
};

/*all the possible directives*/
keyword directives[] = {
  {"data"},{"string"},{"entry"},{"extern"}
};

/*all the possible registers*/ 
keyword registers[] = {
  {"r0"},{"r1"},{"r2"},{"r3"},
  {"r4"},{"r5"},{"r6"},{"r7"}
}; 

/*function that does the first pass it reads input file line and uses other functions to translate
we will use algorithm for two pass assembler*/
int first_pass (char *fileName)
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
    int opcode;
    int funct;

    /*set initial values for IC and DC*/
    IC = 100;
    DC = 0;

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
      translate_line(line,errorFlag,labelFlag,label_array);
    }

    /*close the file being used*/
    fclose(file); 
    return 1;
}

/*function to do stuff with the line and tables*/
int translate_line (char *line,boolean errorFlag,boolean labelFlag, char label_array[32])
{
  labelFlag = False;

  /*check if the line is of comment or whitespace type if so we can just skip it*/
  if(*line == ';' || line == NULL || *line == '\n' || *line == '\0')
  {
    return 0;
  }

  /*Clears empty space*/
  if(*line == ' ' || *line == '\t')
  {
    line = clearspace(line);
  }

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
    insert_data(line,labelFlag,type,label_array);
    return 0;
  }

  else
    return 1;
}

/*check if current part of line is a label*/
int check_label(char *line, char label_array[32])
{
  /*create array to store label characters as we go char by char in line until we find the end of the label (if it exists) max length is 31*/
  int i = 0;

  /*until you find something indicating the end of the label, add char to array and keep going over char*/
  while(*line != ':' && *line != '\0')
  {
    if(i <= 31)
    {
      label_array[i] = *line;
      i++;
      line++;
    }
  }

  /*so it is saved as a string data type*/
  label_array[i] = '\0';

  if(i==0)
  {
    return 0;
  }

  /*if you reach the end of the label, check if the saved label is a valid one*/
  if(*line == ':')
  {
    /*check if label starts with letter*/
    if(!((label_array[0] >= 'a' && label_array[0] <= 'z')||(label_array[0] >= 'A' && label_array[0] <= 'Z')))
    {
      return 0;
    }

    /*check if : shows up after whitespace*/
    if(label_array[i-1] == ' ' || label_array[i-1] == '\t')
    {
      return 0;
    }

    /*check if label with same name exists if it does return 0*/
    if(label_exists(label_array))
    {
      return 0;
    }

    /*check if label has the same name as instruction*/
    for(i = 0; i < 16; i++)
    {
      /*printf("testing: %s%d\n",label_array,i);*/
      if(!strcmp(instructions[i].str, label_array))
      {
        return 0;
      }
    }

    /*check if label has the same name as register*/
    for(i = 0; i < 8; i++)
    {
      if(!strcmp(registers[i].str, label_array))
      {
        return 0;
      }
    }

    /*check if label has the same name as a directive*/
    for(i = 0; i < 4; i++)
    {
      if(!strcmp(directives[i].str, label_array))
      {
        return 0;
      }
    }
  }
  return 1;
}

/*check if part of line is a directive if so return the "type" of directive*/
int check_dir(char *line)
{
  /*compare to data*/
  if(!strncmp(directives[0].str, line+1, 4))
  {
		return 0;
  }
  
  /*compare to string*/
  else if(!strncmp(directives[1].str, line+1, 6))
  {
    return 1;
  }
  
  /*compare to entry*/
  else if(!strncmp(directives[2].str, line+1, 5))
  {
    return 2;
  }
  
  /*compare to extern*/
  else if(!strncmp(directives[3].str, line+1, 6))
  {
    return 3;
  }

  else
  {
    return 10;
  }
}


int insert_data(char *line, boolean labelFlag, int type, char label_array[32])
{
  /*part 6 of algorithm if it is a label insert into symbol table (linked list) as .data type*/
  if(labelFlag)
  {
    insertLabel(label_array,0,DC);
  }

  /*parts 7+8 of algorithm*/
  switch(type)
  {    
    /*check if directive type is .data if so add to data array*/
    case 0:

      /*loop until the end of the data or line*/
      while(line != NULL && *line != '\0' && *line != ' ' &&  *line != '\t' &&  *line != '\n')
      {
        /*make sign flag that checks if number should be treated as negative*/
        boolean signFlag = False;
        int value;

        /*check if number is negative and move to the digits so that we can handle them*/
        if(*line == '-')
        {
          signFlag = True;
          line++;
        }

        else if(*line == '+')
        {
          line++;
        }

        /*check if line value is a digit*/
        if(!isdigit(*line))
        {
          return 0;
        }

        /*get the value of the .data number as positive value*/
        value = strtol(line,&line,10);

        /*if number is negative get the two's complement of the positive value returned*/
        if(signFlag)
        {
          value = (~value)+1;
        }

        /*insert the data into the data array table*/
        data_array[DC].content |= value;  
        /*increment DC*/
        DC++;
        line++;
        line = clearspace(line);
        line = nextpart(line);

        /*if you reached the end*/
        if(line == NULL || *line == '\0')
        {
          return 0;
        }
      }

      /*break so that we don't go to the next case*/
      break;

    /*check if directive type is .string if so add to data array*/
    case 1:

      if(*line != '"')
      {
        return 0;
      }

      /*string starts with a quotation mark so move one*/
      line++;

      /*loop until the end of the string or line*/
      while(line != NULL && *line != '\0' && *line != '"')
      {
        printf("%c",*line);
        /*insert the string characters one by one into the data array table*/
        data_array[DC].content |= (short) *line;
        /*increment DC*/
        DC++;
        line++;
      }

      if(*line != '"')
      {
        return 0;
      }

      /*add a '\0' at the end of the string in the data array*/
      data_array[DC].content |= 0;
      DC++;
      break;

    default:
      printf("Error Invalid Dir");
    }
  return 1;
}
