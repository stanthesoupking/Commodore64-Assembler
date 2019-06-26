#pragma once

#include "language.h"
#include "expander.h"
#include <stdlib.h>
#include <stdio.h>

void assemble(Language* lang, Expander* expander, char *input, char *output);

void writeByte(FILE* f, char byte);
void writeBytes(FILE* f, char* bytes, int count);