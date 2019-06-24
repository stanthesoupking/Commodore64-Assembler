#include <stdio.h>

#include "tokeniser.h"

// The total number of basic tokens
// WARNING: If a new token is added, this number needs to be updated manually.
#define NUMBER_OF_TOKENS 76

int main(int argc, char **argv)
{
   // List of basic keywords
   char* t_keywords[] = {
      "END", "FOR", "NEXT", "DATA", "INPUT#", "INPUT", "DIM", "READ", "LET",
      "GOTO", "RUN", "IF", "RESTORE", "GOSUB", "RETURN", "REM", "STOP", "ON",
      "WAIT", "LOAD", "SAVE", "VERIFY", "DEF", "POKE", "PRINT#", "PRINT", "CONT",
      "LIST", "CLR", "CMD", "SYS", "OPEN", "CLOSE", "GET", "NEW", "TAB(", "TO",
      "FN", "SPC(", "THEN", "NOT", "STEP", "+", "-", "*", "/", "↑", "AND", "OR",
      ">", "=", "<", "SGN", "INT", "ABS", "USR", "FRE", "POS", "SQR", "RND",
      "LOG", "EXP", "COS", "SIN", "TAN", "ATN", "PEEK", "LEN", "STR$", "VAL",
      "ASC", "CHR$", "LEFT$", "RIGHT$", "MID$", "GO"
   };

   // Values of each basic keyword (generated later on)
   unsigned char t_values[NUMBER_OF_TOKENS];

   Language* basicLang;

   unsigned char j;

   char *input;
   char *output;

   if (argc < 2)
   {
      printf("Error: No input file supplied.\n");
   }

   input = argv[1]; // Get path to input file

   if (argc > 2)
   {
      // Output path was supplied
      output = argv[2];
   }
   else
   {
      output = "output";
   }

   // Initialise tokeniser
   for(j = 0; j < NUMBER_OF_TOKENS; j++)
   {
      t_values[j] = j + 128;
   }

   basicLang = createLanguage(t_keywords, t_values, NUMBER_OF_TOKENS);

   printf("Assembling...\n");
   tokenise(basicLang, input, output);
   printf("Complete!\n");

   return 0;
}