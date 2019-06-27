#include "assembler.h"

#include "util.h"
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 255
#define MAX_PROGRAM_LINES 65520

union uByte {
    unsigned int integer;
    char byte[4];
};

enum assemble_status assemble(Language *lang, Expander *expander, char *input, char *output)
{
    FILE *fin;
    FILE *fout;
    char buffer[LINE_BUFFER_SIZE + 1]; // + 1 for final NULL character
    union uByte lineNumber;
    union uByte currentByte;
    unsigned int bpos, lpos;
    char c,v;
    char *s;

    char *tokLine;
    char *expLine;
    char *linkedLine;
    char *outputLine;
    
    char *newLine;
    unsigned int newLineLength;

    int cpos;
    unsigned int i, j;
    int x;
    unsigned int counter;

    unsigned int* linePointer;
    char *linePointerS;

    // File buffer containing all lines of the program
    char *fileBuffer[MAX_PROGRAM_LINES];
    unsigned int fileLineCount; // Counter for current number of program lines

    // Storage for line links
    HashTable* lineLinks;

    // Attempt to open input file
    fin = fopen(input, "r");

    if (!fin)
    {
        printf("Error: Failed loading input file.\n");
        return INPUT_READ_ERROR;
    }

    // Attempt to open output file
    fout = fopen(output, "w");

    if (!fout)
    {
        printf("Error: Failed accessing output file.\n");
        return OUTPUT_WRITE_ERROR;
    }

    // Set buffer position to start of buffer; 0
    bpos = 0;

    // Clear input buffer
    bzero(buffer, LINE_BUFFER_SIZE + 1);

    // Read in lines from file
    printf("Reading file into memory...\n");
    bpos = 0;
    c = getc(fin);
    while (1)
    {
        if (c == '\n' || c == EOF)
        {
            // Only add line if it was not empty
            if(bpos != 0)
            {
                // Copy string into file buffer
                tokLine = tokeniseString(lang, buffer, bpos);
                // s = (char*) malloc(sizeof(char) * bpos);
                // strncpy(s, buffer, bpos);
                expLine = expandString(expander, tokLine, strlen(tokLine));
                
                free(tokLine);

                fileBuffer[fileLineCount++] = expLine;
            }

            // Reset line buffer
            bzero(buffer, bpos);
            bpos = 0;

            // If end of file, stop reading.
            if(c == EOF)
            {
                break;
            }
        }
        else
        {
            // Append character to buffer
            buffer[bpos++] = c;
        }

        // Get next character
        c = getc(fin);
    }
    printf("Read %d line(s).\n\n", fileLineCount);

    // Print file contents
    for(i = 0; i < fileLineCount; i++)
    {
        printf("%d: %s\n", i, fileBuffer[i]);
    }

    printf("Creating line link dictionary...\n");
    lineLinks = createHashTable();
    counter = 0;
    for(i = 0; i < fileLineCount; i++)
    {
        s = fileBuffer[i];

        // Extract tag
        c = s[0];
        
        while(c != '\t' && c != '\n')
        {
            buffer[bpos++] = c;
            c = s[bpos];
        }

        // If tag was found...
        if(bpos > 0)
        {
            // Check if the link has already been defined
            if(hashTableHas(lineLinks, buffer))
            {
                // Link already defined, produce error
                printf("Error: Line link repeated.\n");
                return DUPLICATE_TAG_ERROR;
            }

            // Add tag to collection
            linePointer = (int*) malloc(sizeof(int));
            *linePointer = i;
            hashTableSet(lineLinks, buffer, linePointer);

            // Remove tag from file line
            newLineLength = strlen(s) - bpos;
            newLine = (char*) malloc(sizeof(char) * newLineLength + 1);

            strncpy(newLine, s + bpos, newLineLength);

            free(fileBuffer[i]);
            fileBuffer[i] = newLine;

            bzero(buffer, bpos);
            bpos = 0;
            counter++;
        }
    }
    printf("Found %d link(s).\n\n", counter);

    printf("Linking lines...\n");
    counter = 0;
    for(i = 0; i < fileLineCount; i++)
    {
        s = fileBuffer[i];

        lpos = 0;
        linkedLine = (char*) malloc(sizeof(char) * 255);
        bzero(linkedLine, 255);

        j = 0;
        c = s[j];
        while(c != '\n' && c != '\0')
        {
            if(c == '"')
            {
                // Skip quoted content
                x = getIndexOfChr(s + j, '"');
                printf("%s\n", s + j);
                if(x == -1)
                {
                    // Final quote doesn't exist, produce error
                    printf(
                        "Error on line %d: File is missing terminating \".\n",
                        i + 1
                    );
                    return SYNTAX_ERROR;
                }
                else
                {
                    strncat(linkedLine, s + j - 1, x + 2);
                    lpos += x + 2;
                    j += x + 1;
                }
                
            }
            else
            {
                // Is token GOTO or GOSUB?
                if(c == '\x89' || c == '\x8D')
                {   
                    linkedLine[lpos++] = c;

                    // Skip spaces
                    while(s[j] == ' ')
                        j++;

                    v = s[j++];
                    while(v != ' ' && v != '\n' && v != '\0' && v != ':')
                    {
                        buffer[bpos++] = v;
                        v = s[j++];
                    }

                    if(bpos != 0)
                    {
                        printf("buffer: %s\n", buffer);

                        if(hashTableHas(lineLinks, buffer))
                        {
                            linePointer = hashTableGet(lineLinks, buffer);
                            
                            // Convert line pointer to string for inserting in file
                            linePointerS = (char*) malloc(sizeof(char) * LINE_BUFFER_SIZE + 1);
                            bzero(linePointerS, LINE_BUFFER_SIZE + 1);
                            
                            sprintf(linePointerS, "%d", *linePointer);

                            // Insert line pointer into linked line
                            strcat(linkedLine, linePointerS);

                            // Move position in line buffer along
                            lpos += strlen(linePointerS);
                        }
                        else
                        {
                            // Tag doesn't exist, produce error
                            printf("Error: Attempted to link line to tag that doesn't exist.\n");
                            return UNDEFINED_TAG_ERROR;
                        }
                        

                        bzero(buffer, bpos);
                        bpos = 0;
                    }
                }
                else
                {
                    linkedLine[lpos++] = c;
                }
            }
            c = s[j++];
        }
        free(s);
        fileBuffer[i] = linkedLine;
        counter++;
    }
    printf("Linked %d line(s).\n\n", counter);

    // Print file contents
    for(i = 0; i < fileLineCount; i++)
    {
        printf("%d: %s\n", i, fileBuffer[i]);
    }

    // --- Write to output file ---

    // Write pointer to first line of program (2049)
    currentByte.integer = 2049;
    writeBytes(fout, currentByte.byte, 2);

    for(i = 0; i < fileLineCount; i++)
    {
        s = fileBuffer[i]; // Get line from file buffer

        // Clear line buffer
        bzero(buffer, LINE_BUFFER_SIZE);
        bpos = 0;

        j = 0; // Position in file buffer line
        c = s[j++]; // Get initial character from line
        while(c != '\n' && c != '\0')
        {
            // Filter out tabs from final file
            if(c != '\t')
            {
                buffer[bpos++] = c;
            }
            c = s[j++];
        }

        // If line is empty
        if(bpos == 0)
        {
            // Skip it
            continue;
        }

        // Write pointer to next line
        currentByte.integer += bpos + 5;
        writeBytes(fout, currentByte.byte, 2);

        // Write line number
        lineNumber.integer = i;
        writeBytes(fout, lineNumber.byte, 2);

        // Write line contents
        fputs(buffer, fout);

        // Write end of line
        writeByte(fout, '\0');
    }
    // Write end of file
    currentByte.integer = 0;
    writeBytes(fout, currentByte.byte, 2);

    return SUCCESS;
}

void writeBytes(FILE *f, char *bytes, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        fputc(bytes[i], f);
    }
}

void writeByte(FILE *f, char byte)
{
    fputc(byte, f);
}