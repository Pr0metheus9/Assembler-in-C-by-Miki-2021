/*include datastructs/defintion file*/
#include "datastructs.h"

void insertLabel(char label[],int value,int attribute){
       
  Label* new_Label;

  new_Label = (Label*) malloc(sizeof(Label));


  if(new_Label == NULL)
  {
    /*Unable to allocate memory*/
  }

  new_Label->value = value;
  new_Label->attribute = attribute;
  strcpy(new_Label->name, label);
  
  new_Label->next = NULL; 

  if(head == NULL){
    head = new_Label;
    last = new_Label;
  }
  if(head->next == NULL && head != new_Label){
    last = new_Label;
    head->next = last;
  }
  if(head->next != new_Label && head ->next != new_Label && last != new_Label){
    last->next = new_Label;
    last = new_Label;
  }

}

int label_exists(char label[]){
  
  /*check if label name already exists*/ 
  Label *temp;
  temp = head;
  
  while(temp != NULL)
  {
    if(strcmp(label,temp->name) == 0)
      {
        return 0;
      }
    temp=temp->next;
  }
  return 1;
}

void display()
{
    Label *temp;
    temp = head;
    while(temp != NULL)
    {
      
      printf("%s\t%d\t%d\n",temp->name,temp->value,temp->attribute);
      temp=temp->next;
    }
}
