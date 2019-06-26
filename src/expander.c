#include "expander.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Expander* createExpander(char** from, char** to, unsigned int count)
{
    Expander* e;
    Expansion* expansion;
    unsigned int i;

    e = (Expander*) malloc(sizeof(struct expander_int));

    e->expansions = (Expansion**) malloc(sizeof(struct expansion_int*) * count);
    e->expansion_count = count;

    // Populate expansion array
    for(i = 0; i < count; i++)
    {
        expansion = (Expansion*) malloc(sizeof(struct expansion_int));
        expansion->from = from[i];
        expansion->to = to[i];
        expansion->from_length = strlen(from[i]);
        expansion->to_length = strlen(to[i]);

        e->expansions[i] = expansion;
    }

    return e;
}

void destoryExpander(Expander* e)
{
    unsigned int i;

    // Free expansions
    for(i = 0; i < e->expansion_count; i++)
    {
        free(e->expansions[i]->from);
        free(e->expansions[i]->to);
        free(e->expansions[i]);
    }

    free(e->expansions);
    free(e);

    e = NULL;
}

char* expandString(Expander* e, char* input, unsigned int length)
{
    char* result;
    unsigned int rpos;
    unsigned int spos;
    Expansion* expansion;

    // Initialise result buffer
    result = (char*) malloc(sizeof(char) * length + 1);
    bzero(result, length + 1);

    // For each character in string
    rpos = 0;
    for(spos = 0; spos < length; spos++)
    {
        // Attempt to find match for macro expansion
        expansion = findExpansionMatch(e, (char*) input + spos, length - spos);

        if(expansion) // If an expansion was found...
        {
            // Copy expansion
            strcat(result, expansion->to);

            // Move position in input by token length
            spos += expansion->from_length - 1;
            rpos += expansion->to_length;
        }
        else // No expansion found...
        {
            result[rpos++] = input[spos];
        }
    }

    return result;
}

Expansion* findExpansionMatch(Expander* e, char* input, unsigned int length)
{
    int r;
    unsigned int i;
    Expansion* expansion;

    // Attempt to find match
    for(i = 0; i < e->expansion_count; i++)
    {
        expansion = e->expansions[i];

        if(expansion->from_length <= length) {
            r = strncmp(expansion->from, input, expansion->from_length);
            if(r == 0) { // Exact match found
                return expansion;
            }
        }
    }

    return NULL;
}