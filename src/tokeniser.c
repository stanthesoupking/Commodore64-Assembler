#include "tokeniser.h"

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

void tokenise(Language* lang, char *input, char *output)
{
    FILE *fin;
    FILE *fout;
    char buffer[LINE_BUFFER_SIZE+1]; // + 1 for final NULL character
    union uByte line_num;
    unsigned int bpos;
    enum t_state state;
    char c;
    char* tstring;
    int cpos;
    union uByte progStart;
    union uByte currentByte;

    bzero(buffer, LINE_BUFFER_SIZE + 1);

    fin = fopen(input, "r");
    fout = fopen(output, "w");

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
            
            tstring = tokeniseString(lang, buffer + cpos + 1, bpos);
            currentByte.integer += strlen(tstring) + 5;

            // Write pointer to next line
            writeBytes(fout, currentByte.byte, 2);

            // Get line number
            line_num.integer = strtol(buffer, NULL, 10);
            writeBytes(fout, line_num.byte, 2);

            fputs(tstring, fout);

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