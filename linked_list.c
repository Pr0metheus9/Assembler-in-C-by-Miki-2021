/*include datastructs/defintion file*/
#include "datastructs.h"

/*add new node to linked list*/
void insertLabel(char label[], int attribute, int value)
{
    /*create new label pointer to the new label we want to add*/
    Label* new_Label;

    /*allocate space for new label*/
    new_Label = (Label*)malloc(sizeof(Label));

    /*check if new label we want to add is a NULL label*/
    if (new_Label == NULL)
    {
        /*Unable to allocate memory*/
    }
    /*give the label it's values (name,value,attribute)*/
    new_Label->value = value;
    new_Label->attribute = attribute;
    strcpy(new_Label->name, label);

    /*it is added at the end of the list so it points to null*/
    new_Label->next = NULL;

    /*check if linked list head is null, if so this is the first and only value in
     * the linked list*/
    if (head == NULL)
    {
        head = new_Label;
        last = new_Label;
    }

    /*check if it will be second node added to linked list*/
    if (head->next == NULL && head != new_Label)
    {
        last = new_Label;
        head->next = last;
    }

    /*check if we already have a formed linked list if so make last item in the
     * list*/
    if (head->next != new_Label && head->next != new_Label && last != new_Label)
    {
        last->next = new_Label;
        last = new_Label;
    }
}

/*check if the label already exists in the linked list*/
int label_exists(char label[])
{
    /*create temp label so that we can cycle over all the nodes and initialize at
     * first item (head)*/
    Label* temp;
    temp = head;

    /*go over every node and compare it's name with the input name*/
    while (temp != NULL)
    {
        if (strcmp(label, temp->name) == 0)
        {
            return 1;
        }
        temp = temp->next;
    }
    /*if label name is not already in use return false (because if doesn't already
     * exist)*/
    return 0;
}

Label* get_label(char label[])
{
    /*check if label name already exists*/
    Label* temp;
    temp = head;

    while (temp != NULL)
    {
        if (strcmp(label, temp->name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; /*label not found*/
}

/*function to display symbol table / linked list on screen*/
void display()
{
    Label* temp;
    temp = head;
    while (temp != NULL)
    {
        if (temp->isEntry == 1)
        {
            printf("%-8s%-8d%-8d%-1s\n", temp->name, temp->value, temp->attribute, "entry");
        }

        else
        {
            printf("%-8s%-8d%-8d\n", temp->name, temp->value, temp->attribute);
        }

        temp = temp->next;
    }
}
