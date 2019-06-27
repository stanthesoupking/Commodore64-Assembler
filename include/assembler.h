#pragma once

#include "language.h"
#include "expander.h"
#include <stdlib.h>
#include <stdio.h>

enum assemble_status {
    SUCCESS,            // Assembly was successful
    UNKNOWN_ERROR,      // Undefined error occurred
    INPUT_READ_ERROR,   // Error occurred while trying to read input file
    OUTPUT_WRITE_ERROR, // Error occurred while trying to write to output file
    SYNTAX_ERROR,       // Error in source code syntax
    DUPLICATE_TAG_ERROR,// Tag was defined more than once
    UNDEFINED_TAG_ERROR // Tag was referenced without being defined
};

enum assemble_status assemble(Language* lang, Expander* expander, char *input, char *output);

void writeByte(FILE* f, char byte);
void writeBytes(FILE* f, char* bytes, int count);