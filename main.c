/*Pr0metheus9 - Miki*/
/*include datastructs/defintion file*/
#include "datastructs.h"

/*get arguments and create output files from them (if they exist)*/
int main(int argc, char *argv[])
{
  int i;
  for(i = 1; i < argc; i++)
  {
    first_pass(argv[i]);
    display_code();
    printf("\n");
    display_data();
    printf("\n");
    display();
  }
  return 0;
}
