/*include basic libraries*/
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

/*define constants*/

/*max number of words in RAM*/
#define RAM_SIZE 4096 

/*=========================
New Structures we will need
=========================*/

/*Helps to create linked list for labels*/
typedef struct Node  
{ 
    /*max length of label name is 31 chars + 1 for \0 for array has len 32*/
    char  name [32];
    /*1.data, 2.code, 3.extern, 4.entry*/  
    int  attribute;
    /*Label Word Place in File*/ 
    int value;
    /*pointer to next label type declared so we get a linked list node --> next node --> next next node and so on.... */ 
    struct Node *next;
    /*give struct type name Label using typedef to make things easy*/ 
} Label; 

/*create machine code image struct*/
typedef struct Image
{
    /*note: we only use the first 12 bits, 8-11 opcode, 4-7 funct, 2-3 addressing mode for source, 0-1  addressing mode for target*/
    short content;
    /*"A,R,E" saved to help linker*/
    char are;
    /*give struct type name Word using typedef to make things easy*/ 
} Word; 

/*Create struct for certain keywords in asm program (.as) such as registers,directives and instructions*/
typedef struct keyword {
    /*name of keyword / keyword title*/
    char *str; 
} keyword;

/*define boolean variable for ease of use*/
typedef enum { False, True } boolean; 

/*===================
Function Declarations
===================*/

/*From first_pass*/
int first_pass (char *);
int translate_line (char *,boolean,boolean);
int check_label (char *);
char *nextpart(char *);

/*from linked_list*/
int label_exists(char []);
void insertLabel(char [],int,int);
void display();

/*=========================================================
Variable Declarations (For vars used throughout the program)
==========================================================*/

/*Tables used in program*/
/*Symbol Table*/
Label *head; 
Label *last; 
/*Main Tables. Three quarters of words are for code, and one quarter for data (Can be changed)*/
Word code_array [(RAM_SIZE/4)*3]; 
Word data_array [RAM_SIZE/4]; 

/*keywords in asm*/
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
