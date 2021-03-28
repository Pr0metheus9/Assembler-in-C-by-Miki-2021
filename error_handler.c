/*include datastructs/defintion file*/
#include "datastructs.h"

/*function to print errors to the screen*/
void write_error(int error)
{
    switch (error)
    {
        case 1:
            printf("line %d:\tSyntax Error\n", LC);
            break;
        case 2:
            printf("line %d:\tlabel cannot be an instruction.\n", LC);
            break;
        case 3:
            printf("line %d:\tlabel cannot be a directive.\n", LC);
            break;
        case 4:
            printf("line %d:\tlabel is already defined.\n", LC);
            break;
        case 5:
            printf("line %d:\tlabel cannot contain more than 30 characters\n", LC);
            break;
        case 6:
            printf("line %d:\tdirective doesn't exist\n", LC);
            break;
        case 7:
            printf("line %d:\tinstruction doesn't exist\n", LC);
            break;
        case 8:
            printf("line %d:\tFirst character in label is not a letter.\n", LC);
            break;
        case 9:
            printf("line %d:\tTo many characters in the line\n", LC);
            break;
        case 10:
            printf("line %d:\tbad argument[s]\n", LC);
            break;
        case 11:
            printf("Line %d:\tline only contains label\n", LC);
            break;
        case 12:
            printf("Line %d:\tdata input is not a number\n", LC);
            break;
        case 13:
            printf("line %d:\tlabel is set to register\n", LC);
            break;
        case 14:
            printf("FILE NOT FOUND\n");
            break;
        case 15:
            printf("line %d:\tlabel was not defined in the file\n", LC);
            break;
        case 16:
            printf("line %d:\tlabel already defined as external\n", LC);
            break;
        case 17:
            printf("line %d:\tcannot define label as external.", LC);
            break;
        case 18:
            printf("line %d:\tinvalid operator", LC);
            break;
        case 19:
            printf("line %d:\tstring missing quotation mark.\n", LC);
            break;
        case 20:
            printf("line %d:\toperator addressing type not allowed.\n", LC);
            break;
        case 21:
            printf("line %d:\toperator was expected but not found.\n", LC);
            break;
        case 22:
            printf("line %d:\tnumber was expected but not found.\n", LC);
            break;
        case 23:
            printf("line %d:\twhitespace between label name and:\n", LC);
            break;
        case 24:
            printf("line %d:\tlabel was expected but not found.\n", LC);
            break;
        case 25:
            printf("line %d:\ttoo many operators.\n", LC);
            break;
        case 26:
            printf("line %d:\ttoo few operators.\n", LC);
            break;
    }
}