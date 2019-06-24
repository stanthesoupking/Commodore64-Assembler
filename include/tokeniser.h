#pragma once

#include "hash_table.h"
#include "language.h"
#include <stdlib.h>
#include <stdio.h>

void tokenise(Language* lang, char *input, char *output);

void writeByte(FILE* f, char byte);
void writeBytes(FILE* f, char* bytes, int count);