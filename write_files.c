/*include datastructs/defintion file*/
#include "datastructs.h"

/*write to object file*/
void write_obj(char *filename)
{
  int i;

  FILE * fp;

  /*give file extension ob using the filename*/
  char file_ob[32];
  strcpy(file_ob, filename);
  strcat(file_ob, ".ob");

  /*open the file for writing */
  fp = fopen (file_ob,"w");
  
  /*first line */
  fprintf (fp, "%d\t%d\n",ICF-100,DCF);
  
  /*rest of the lines*/

  /*for instruction words*/
  for(i = 100; i < IC; i++)
  {
    fprintf(fp,"%04d %03X %c\n",i,code_array[i-100].content,code_array[i-100].are);
  }

  /*for data words*/
  for(i = IC; i < IC+DC; i++)
  {
    fprintf(fp,"%04d %03x %c\n",i,data_array[i-IC].content,data_array[i-IC].are);
  }

  /*close the file  */
  fclose (fp);
}

/*write to entry file*/
void write_ent(char *filename)
{
  FILE * fp;
  Label* temp;
  
  /*give file extension ent using the filename*/
  char file_ext[32];
  strcpy(file_ext, filename);
  strcat(file_ext, ".ent");

  /*open the file for writing */
  fp = fopen (file_ext,"w");
  
  /*for labels if entry, print out the label and label address*/
  temp = head;
  while (temp != NULL)
  {
      if (temp->isEntry == 1)
      {
        fprintf(fp,"%-8s%-8d\n", temp->name, temp->value);
      }
      temp = temp->next;
    }
  /*close the file  */
  fclose (fp);
}


/*write to external file*/
void write_ext(char *filename)
{
  FILE * fp;

  int i;
  
  /*give file extension ext using the filename*/
  char file_ext[32];
  strcpy(file_ext, filename);
  strcat(file_ext, ".ext");

  /*open the file for writing */
  fp = fopen (file_ext,"w");
  
  /*for extern labels, print out the label */
  for(i = 0; i < ext_num; i++)
  {
    fprintf(fp,"%s %04d\n",ext_labels[i].label,ext_labels[i].address);
  }  
   
  /*close the file  */
  fclose (fp);
}