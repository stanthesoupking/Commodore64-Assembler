#pragma once

typedef struct expansion_int {
    char* from;
    char* to;
    unsigned int from_length;
    unsigned int to_length;
} Expansion;

typedef struct expander_int {
    Expansion** expansions;
    unsigned int expansion_count;
} Expander;

Expander* createExpander(char** from, char** to, unsigned int count);
void destoryExpander(Expander* e);

char* expandString(Expander* e, char* string, unsigned int length);
Expansion* findExpansionMatch(Expander* e, char* input, unsigned int length);