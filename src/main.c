#include <stdio.h>

#include "assembler.h"
#include "expander.h"
#include <string.h>

// The total number of basic tokens
// WARNING: If a new token is added, this number needs to be updated manually.
#define NUMBER_OF_TOKENS 76

// The total number of expansions
// WARNING: If a new expansion is added, this number needs to be updated manually.
#define NUMBER_OF_EXPANSIONS 40

// The current program version
#define VERSION "0.2.0"

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

   // List of PETSCII expansions(from)
   char* e_from[] = {
      // Col 1    ...   Col 2         ...    Col3
      "{CLEAR}",        "{RUNSTOP}",         "{WHITE}",        // Row 1
      "{RETURN}",       "{CURSOR-DOWN}",     "{REVERSE-ON}",   // Row 2
      "{HOME}",         "{DELETE}",          "{RED}",          // Row 3
      "{CURSOR-RIGHT}", "{GREEN}",           "{BLUE}",         // Row 4
      "{LOAD-RUN}",     "{F1}",              "{F3}",           // Row 5
      "{F5}",           "{F7}",              "{F2}",           // Row 6
      "{F4}",           "{F6}",              "{F8}",           // Row 7
      "{LF}",           "{GRAPHICS}",        "{BLACK}",        // Row 8
      "{CURSOR-UP}",    "{REVERSE-OFF}",     "{INSERT}",       // Row 9
      "{BROWN}",        "{LIGHT-RED}",       "{DARK-GREY}",    // Row 10
      "{GREY}",         "{LIGHT-GREEN}",     "{LIGHT-BLUE}",   // Row 11
      "{LIGHT-GREY}",   "{PURPLE}",          "{CURSOR-LEFT}",  // Row 12
      "{YELLOW}",       "{CYAN}",            "{NBSP}",         // Row 13
      "{TEXT-MODE}"                                            // Row 14

   };

   // List of PETSCII expansions(to)
   char* e_to[] = {
      // Col 1    ...   Col 2         ...    Col3
      "\x93",           "\x03",              "\x05",  // Row 1
      "\x0D",           "\x11",              "\x12",  // Row 2
      "\x13",           "\x14",              "\x1C",  // Row 3
      "\x1D",           "\x1E",              "\x1F",  // Row 4
      "\x83",           "\x85",              "\x86",  // Row 5
      "\x87",           "\x88",              "\x89",  // Row 6
      "\x8A",           "\x8B",              "\x8C",  // Row 7
      "\x8D",           "\x8E",              "\x90",  // Row 8
      "\x91",           "\x92",              "\x94",  // Row 9
      "\x95",           "\x96",              "\x97",  // Row 10
      "\x98",           "\x99",              "\x9A",  // Row 11
      "\x9B",           "\x9C",              "\x9D",  // Row 12
      "\x9E",           "\x9F",              "\xA0",  // Row 13
      "\x0E"                                          // Row 14
   };

   Language* basicLang; // Commodore64 Basic keyword tokeniser
   Expander* expander;  // Macro expander

   unsigned char j;

   char *input;   // Input file path
   char *output;  // Output file path

   if (argc < 2)
   {
      printf("Error: No input file supplied.\n");
      return 1;
   }
   else
   {
      // Check if program version is to be printed
      if((strcmp("--version", argv[1]) == 0) || (strcmp("-v", argv[1]) == 0))
      {
         printf("Commodore 64 Basic Assembler v%s\n", VERSION);
         printf("Programmed by Stanley Fuller, 2019\n");
         return 0;
      }
   }

   input = argv[1]; // Get path to input file

   if (argc > 2)
   {
      // Output path was supplied
      output = argv[2];
   }
   else
   {
      // Use default path; output.prg
      output = "output.prg";
   }

   // Initialise tokeniser
   for(j = 0; j < NUMBER_OF_TOKENS; j++)
   {
      t_values[j] = j + 128;
   }

   basicLang = createLanguage(t_keywords, t_values, NUMBER_OF_TOKENS);
   
   // Initialise macro expander
   expander = createExpander(e_from, e_to, NUMBER_OF_EXPANSIONS);

   // Assemble file and write to output path
   printf("Assembling...\n\n");
   
   assemble(basicLang, expander, input, output);
   
   printf("Complete!\n");

   return 0;
}