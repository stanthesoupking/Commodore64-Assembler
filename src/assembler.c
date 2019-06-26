#include "assembler.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 255

enum t_state {
    GET_LINE_NUM,
    GET_KEYWORD,
    GET_VALUE
};

union uByte {
    unsigned int integer;
    char byte[4];
};

void assemble(Language* lang, Expander* expander, char *input, char *output)
{
    FILE *fin;
    FILE *fout;
    char buffer[LINE_BUFFER_SIZE+1]; // + 1 for final NULL character
    union uByte line_num;
    unsigned int bpos;
    enum t_state state;
    char c;
    char* expandedString;
    char* tokenisedString;
    int cpos;
    union uByte progStart;
    union uByte currentByte;

    bzero(buffer, LINE_BUFFER_SIZE + 1);

    fin = fopen(input, "r");

    if(!fin)
    {
        printf("Error: Failed loading input file.\n");
        return;
    }

    fout = fopen(output, "w");

    if(!fout)
    {
        printf("Error: Failed accessing output file.\n");
        return;
    }

    // Write pointer to first line of program (2049)
    progStart.integer = 2049;
    writeBytes(fout, progStart.byte, 2);

    currentByte.integer = progStart.integer;
    state = GET_LINE_NUM;
    bpos = 0;
    c = getc(fin);

    while(1)
    {   
        // Check if line buffer limit is being exceeded
        if(bpos > LINE_BUFFER_SIZE)
        {
            printf("ERROR: Line buffer limit overrun (limit is %d characters).",
                LINE_BUFFER_SIZE);
            return;
        }

        // Is it the end of the line or file?
        if(c == '\n' || c == EOF)
        {   
            // Extract line number
            cpos = getIndexOfChr(buffer, ' ');

            if(cpos == -1)
            {
                printf("Error: No space after line number detected.\n");
            }
            
            // Uncomment these lines for debugging
            //printf("Pass #1: Macro Expansion\n");
            expandedString = expandString(expander, buffer + cpos + 1, bpos);
            //printf("\t%s\n", expandedString);
            //printf("Done.\n\n");

            //printf("Pass #2: Tokenisation\n");
            tokenisedString = tokeniseString(lang, expandedString, strlen(expandedString));
            currentByte.integer += strlen(tokenisedString) + 5;
            //printf("\t%s\n", tokenisedString);
            //printf("Done.\n\n");

            // Write pointer to next line
            writeBytes(fout, currentByte.byte, 2);

            // Get line number
            line_num.integer = strtol(buffer, NULL, 10);
            writeBytes(fout, line_num.byte, 2);

            fputs(tokenisedString, fout);

            writeByte(fout, 0x00); // Write end of line
            
            bzero(buffer, bpos);
            bpos = 0;

            // End read-file loop if end-of-file
            if(c == EOF)
            {
                break;
            }
        }
        else
        {
            // Regular character, add to line.
            buffer[bpos++] = c;
        }
        
        c = getc(fin);
    }
    writeByte(fout, 0x00); // Write end of file termination
    writeByte(fout, 0x00);
}

void writeBytes(FILE* f, char* bytes, int count)
{
    int i;
    for(i = 0; i < count; i++)
    {
        fputc(bytes[i], f);
    }
}

void writeByte(FILE* f, char byte)
{
    fputc(byte, f);
}