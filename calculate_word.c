/*include datastructs/defintion file*/
#include "datastructs.h"

int insert_data(char *line, boolean labelFlag, int type, char label_array[32])
{
  /*part 6 of algorithm if it is a label insert into symbol table (linked list) as .data type*/
  if(labelFlag)
    insertLabel(label_array,0,DC);
  
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
          line++;

        /*get the value of the .data number as positive value*/
        value = get_num(line);

        /*if number is negative get the two's complement of the positive value returned*/
        if(signFlag)
          value = (~value)+1;
        
        /*insert the data into the data array table*/
        data_array[DC].content |= value << 8;  
        /*increment DC*/
        DC++;
        line++;
        line = clearspace(line);
        line = nextpart(line);

        /*if you reached the end*/
        if(line == NULL || *line == '\0'){
          return 0;
      }

      /*break so that we don't go to the next case*/
      break;

    /*check if directive type is .string if so add to data array*/
    case 1:
      /*string starts with a quotation mark so move one*/
      line++;

      /*loop until the end of the string or line*/
      while(line != NULL && *line != '\0' && *line != '"')
      {
        /*insert the string characters one by one into the data array table*/
        data_array[DC].content |= (short) *line << 8;
        /*increment DC*/
        DC++;
        line++;
         
        /*add a '\0' at the end of the string in the data array*/
        data_array[DC].content |= (short) '\0' << 8;
        DC++;
      }
      break;
    }
  }
}

