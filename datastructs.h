/*include basic libraries*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*define constants*/

/*max number of words in RAM*/
#define RAM_SIZE 4096

/*=========================
New Structures we will need
=========================*/

/*define boolean variable for ease of use*/
typedef enum
{
    False,
    True
} boolean;

/*Helps to create linked list for labels*/
typedef struct Node
{
    /*max length of label name is 31 chars + 1 for \0 for array has len 32*/
    char name[32];
    /*0.data, 1.code, 2.extern, 3.entry*/
    int attribute;
    /*int to check if attribute also has entry type*/
    int isEntry;
    /*Label Word Place in File*/
    int value;
    /*pointer to next label type declared so we get a linked list node --> next
     * node --> next next node and so on.... */
    struct Node* next;
    /*give struct type name Label using typedef to make things easy*/
} Label;

/*create machine code image struct*/
typedef struct Image
{
    /*note: we only use the first 12 bits, 8-11 opcode, 4-7 funct, 2-3 addressing
     * mode for source, 0-1  addressing mode for target*/
    short content;
    /*"A,R,E" saved to help linker*/
    char are;
    /*give struct type name Word using typedef to make things easy*/
} Word;

/*Create struct for certain keywords in asm program (.as) such as
 * registers,directives and instructions*/
typedef struct keyword
{
    /*name of keyword / keyword title*/
    char* str;
} keyword;

/*create a struct for ext file output and create global array of external labels using default
 * number of 100 uses*/
struct ext_label
{
    int address;
    char label[32];
} ext_labels[100];

/*===================
Function Declarations
===================*/

/*From first_pass*/
int first_pass(char*);
int translate_line(char*, boolean, boolean, char[32], int*, int*);
int valid_label(char[32], int);
int check_label(char*, char[32]);
int check_dir(char* line);
int insert_data(char*, boolean, int, char[32]);
int check_intruction(char*, int*, int*);
int insert_instruction(char*, boolean, int*, int*, char[32]);
int addressfunc(Word[], int, char*, char[32]);

/*From line_help*/
char* nextpart(char*);
char* clearspace(char*);
int get_num(char*);
int getlabelsec(char*, char[32]);
void display_code();
void display_data();

/*From linked_list*/
int label_exists(char[]);
void insertLabel(char[], int, int);
void display();
Label* get_label(char label[]);
int compare_label(char label[]);

/*From second_pass*/
int second_pass(char* fileName);
int translate_line2(char* line, boolean errorFlag, boolean labelFlag, char label_array[32],
    int* opcode, int* funct);
int complete_instruction(
    char* line, boolean labelFlag, int* opcode, int* funct, char label_array[32]);
int check_label2(char* line, char label_array[32]);

/*From write_files*/
void write_obj(char* filename);
void write_ent(char* filename);
void write_ext(char* filename);

/*From error handler*/
void write_error(int error);

/*=========================================================
Variable Declarations (For vars used throughout the program)
==========================================================*/

/*Tables used in program*/
/*Symbol Table*/
Label* head;
Label* last;
/*Main Tables. Three quarters of words are for code, and one quarter for data
 * (Can be changed)*/
Word code_array[(RAM_SIZE / 4) * 3];
Word data_array[RAM_SIZE / 4];

/*add counters as global variables*/
int IC;
int DC;
int tIC;
int tDC;
int ICF;
int DCF;
int LC;
/*global variables to check if any external or entry labels exists (Make for the output files)*/
boolean existsExt;
boolean existsEnt;
/*global variable to keep track of number of external occurrences*/
int ext_num;
/*global variable to check if an error exists*/
int error;
/*make keyword arrays*/
keyword instructions[16];
keyword directives[4];
keyword registers[8];