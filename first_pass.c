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
    int opcode = 0;
    int funct = 0;

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
      translate_line(line,errorFlag,labelFlag,label_array,&opcode,&funct);
    }

    /*close the file being used*/
    fclose(file); 
    return 1;
}

/*function to do stuff with the line and tables*/
int translate_line (char *line,boolean errorFlag,boolean labelFlag, char label_array[32],int *opcode,int *funct)
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

  line = clearspace(line);

  /*instruction check if part is an instruction. if it is then move to the next part and save to table*/ 
  if(check_intruction(line,opcode,funct))
  {
    line = nextpart(line);
    /*for debug: printf("opcode: %d\tfunct: %d\n",*opcode,*funct);*/
    insert_instruction(line,labelFlag,opcode,funct,label_array);
    return 0;
  }

  else
  {
    return 1;
  }
}

/*function to check if label is a label that can be used*/
int valid_label(char label_array[32],int i)
{
  /*check if label starts with letter*/
  if(!((label_array[0] >= 'a' && label_array[0] <= 'z')||(label_array[0] >= 'A' && label_array[0] <= 'Z')))
  {
    return 0;
  }

  /*check if : shows up after whitespace*/
  if(label_array[i] == ' ' || label_array[i] == '\t')
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
    if(!strncmp(instructions[i].str, label_array,4) || !strncmp(instructions[i].str, label_array,3))
    {
      /*printf("%c%c%c\n",label_array[0],label_array[1],label_array[2]);*/
      return 0;
    }
  }

  /*check if label has the same name as register*/
  for(i = 0; i < 8; i++)
  {
    if(!strncmp(registers[i].str, label_array,2))
    {
      return 0;
    }
  }

  /*check if label has the same name as a directive*/
  for(i = 0; i < 4; i++)
  {
    if(!strncmp(directives[i].str, label_array,4))
    {
      return 0;
    }
  }
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
    if(i > 31)
    {
      return 0;
    }
    
    label_array[i] = *line;
    i++;
    line++;
  }

  /*so it is saved as a string data type*/
  label_array[i]='\0';

  if(i==0)
  {
    return 0;
  }

  /*if you reach the end of the label, check if the saved label is a valid one*/
  if(*line == ':' || *line == '\0')
  {
    if(!valid_label(label_array,i-1))
    {
      return 0;
    }

    else
    {
      return 1;
    }
  }
  return 0;
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

/*check if part is a valid instruction*/
int check_intruction(char *line,int *opcode,int *funct){
  int i;
  *funct = 0;
  /*all instructions apart from instruction with opcode 15, have three letters*/
  for(i = 0; i < 15; i++){
    /*handling all three letter instructions*/
    if(!strncmp(instructions[i].str, line, 3)){

      *opcode = i;
      
      /*To handel instructions with same opcode, use different cases. In instructions which do not "share" their opcode with other instructions, index in struct of instructions is enough. (because of the way we defined instructions array*/

      switch(*opcode)
      {
        case 2:
          *funct = 10;
          break;
        
        case 3:
          *opcode = 2;
          *funct = 11;
          break;

        case 5:
          *funct = 10;
          break;

        case 6:
          *opcode = 5;
          *funct = 11;
          break;
        
        case 7:
          *opcode = 5;
          *funct = 12;
          break;
        
        case 8:
          *opcode = 5;
          *funct = 13;
          break;

        case 9:
          *opcode = 9;
          *funct = 10;
          break;
        
        case 10:
          *opcode = 9;
          *funct = 11;
          break;
        
        case 11:
          *opcode = 9;
          *funct = 12;
          break;
      }
      return 1;
    }
  }

  /*handling stop instruction, which is the only 4 letter intruction*/
  if(!strncmp(instructions[15].str, line, 4))
  {
    *opcode = 15;
    return 1;
  }

  return 0;
}

/*function to insert data into the data array (image)*/
int insert_data(char *line, boolean labelFlag, int type, char label_array[32])
{
  /*part 6 of algorithm if it is a label insert into symbol table (linked list) as .data type*/
  if(labelFlag)
  {
    insertLabel(label_array,0,DC+IC);
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

        /*get the value of the .data number*/
        value = get_num(line);

        /*if number is negative get the two's complement of the positive value returned*/
        if(signFlag)
        {
          value = (~value)+1;
        }

        /*insert the data into the data array table*/
        data_array[DC].content |= value;
        data_array[DC].are = 'A';  
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
      while(line!=NULL && *line != '"' && *line != '\0')
      {
        /*printf("%c",*line);*/
        /*insert the string characters one by one into the data array table*/
        data_array[DC].content |= (short) *line;
        data_array[DC].are = 'A';
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
      data_array[DC].are = 'A';
      DC++;
      break;

    /*check if directive is of type .entry if so just skip it (will be handled in second pass)*/
    case 2:
      break;

    /*check if directive type is .extern if so insert the label named after it into the label table with the type of extern and with value 0*/
    case 3:
      if(check_label(line,label_array))
      {
        insertLabel(label_array, 3, 0);
      }    
      break;


    default:
      printf("Error Invalid Dir");
    }

  return 1;
}

/*function to insert instructions*/
int insert_instruction(char *line,boolean labelFlag,int *opcode, int *funct,char label_array[32])
{
  /*if there was a label before the instruction insert it to the label table with the value of the IC and type of code*/
  int L = -1;
  /*declares and array of 3 possible memory words (max number of words) one for each of command, source and target*/
  Word iwords[3]; 

  /*set the extra words content to some default value*/
  iwords[0].content = 0;
  iwords[1].content = -1;
  iwords[2].content = -1;

  if(labelFlag)
  {
    insertLabel(label_array, 1, IC);
  }

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

  else
  {
    return 0;
  }

  /*add the relevant parts into the words*/
  line = clearspace(line);

  addressfunc(iwords,L,line);

  /*insert the opcode and funct in appropriate place in word*/
  iwords[0].content |= *opcode << 8; 
  iwords[0].content |= *funct << 4;
  iwords[0].are = 'A';
  /*printf("test content value: %d\n",iwords[0].content);*/
  
  /*for each of the words, if they exist, insert them into the instruction array and increment IC*/
  code_array[IC-100] = iwords[0];
  IC++;

  if(!(iwords[1].content == -1))
  {
    code_array[IC-100] = iwords[1];
    IC++;
  }

  if(!(iwords[2].content == -1))
  {
    code_array[IC-100] = iwords[2];
    IC++;
  }
  return 1;
}

/*function to insert the source and target into word if they are relevant using correct addresing type*/
int addressfunc(Word iwords[],int L,char *line)
{
  int i;

  /*loops where number of iterations is equal to the amount of words we will need to create*/
  for(i = 1; i<=L; i++)
  {
    /*A target is requested*/
    if(*line == ',' && i == 2)
    {
      line++;
      line = clearspace(line);
    }

    /*Instant addressing (value of 0)*/ 
    if(*line == '#')
    {
      boolean signFlag = False;
      int value = 0;

      /*if it is source operand set source addressing type to 0 (bits 2,3)*/
      if(i == 1 && L == 2)
      {
        iwords[0].content |= 0 << 2;
        iwords[0].are = 'A';
      }

      /*if it is target operand set source addressing type to 0 (bits 2,3)*/
      if(i == 2 && L == 1)
      {
        iwords[0].content |= 0 << 0;
        iwords[0].are = 'A';
      }

      line++;

      /*checks if the number is positive or negative*/
      if(*line == '-')
      {
        signFlag = True;
        line++;
      }

      if(*line == '+')
      {
        line++;
      }

      /*get the value of the number*/
      value = get_num(line);

      /*if its negative get its two's complement counterpart*/
      if(signFlag)
      {
        value = ~(value)+1;
      }

      /*create the other new word that actually contains the value of source*/ 
      iwords[i].content = value;
      iwords[i].are = 'A';
      line = nextpart(line);
      continue;
    }

    /*relative addressing (value 2)*/
    else if(*line == '%')
    {
      /*relative addressing has only a source and not a destination*/
      iwords[0].content |= 2 << 2;
      iwords[0].are = '?';

      /*make empty space for the label address in future (can't be -1 cause we already have that as default for iwords[1,2]*/
      iwords[i].content = 0;
      iwords[i].are = 'A';
      continue;
    }

    /*direct register addressing (value 3)*/
    else if(*line == 'r')
    {
       /*check if the register is one that exists*/
       if(*(line + 1) <= 7 || *(line + 1) >= 0)
       {
         if(*(line+2) == ' ' || *(line+2) == '\t' || *(line+2) == ',' || line+2 == NULL || *(line+2) == '\0' || *(line+2) == '\n')
         {  
           int reg = 0;
           int value = 0;

           /*sets addressing type to direct register addressing*/

           /*if it is source operand set source addressing type to 3 (bits 2,3)*/
           if(i == 1 && L == 2)
           {
             iwords[0].content |= 3 << 2;
             iwords[0].are = 'A';
           }
           
           /*if it is destination operand set destination addressing type to 3 (bits 2,3)*/
           else if(i == 2 && L == 1)
           {
             iwords[0].content |= 3 << 0;
             iwords[0].are = 'A';
           }

           line++;

           /*reg contains register value 0-7*/
           reg = *line - '0';
           
           /*light up bit in place reg and set extra word to the register number*/
           value |= 1 << reg;
           iwords[i].content = value;
           iwords[i].are = 'A';
           /*printf("register number %d\tregister value: %d\n",reg,value);*/

           line = nextpart(line);
           continue;
         }
       }
    }

    /*direct addresing (value of 1)*/
    else
    {
       /*if it is source operand set source addressing type to 1 (bits 2,3)*/
       if(i == 1 && L == 2)
       {
         iwords[0].content |= 1 << 2;
       }

       /*if it is destination operand set destination addressing type to 1 (bits 2,3)*/
       else if(i == 2 && L == 1)
       {
         iwords[0].content |= 1 << 0;
       }

       /*placeholder will find real value in second pass*/
       iwords[0].are = '?';

       /*leaves space for word to be filled in the second scan*/
       iwords[i].content = 0;
       iwords[i].are = '?';
       line = nextpart(line);
       continue;
    }
  }
  return 0;
}

