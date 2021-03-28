/*Pr0metheus9 - Miki*/
/*include datastructs/defintion file*/
#include "datastructs.h"

int main(int argc, char *argv[])
{
  int i;
  for(i = 1; i < argc; i++)
  {
    first_pass(argv[i]);
  }
  return 0;
}
