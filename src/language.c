#include "language.h"

#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool insertToken(void* a, void* b)
{
    LanguageToken* tokenA = (LanguageToken*) a;
    LanguageToken* tokenB = (LanguageToken*) b;

    return (tokenA->keyword_length > tokenB->keyword_length);
}

Language* createLanguage(char** keywords, unsigned char* values, unsigned int count)
{
    unsigned int i;
    Language* lang;
    LanguageToken* token;

    lang = (Language*) malloc(sizeof(struct language_int));

    lang->tokens = createLinkedList();

    // Populate Keyword -> Value array
    for(i = 0; i < count; i++)
    {
        token = (LanguageToken*) malloc(sizeof(struct language_token_int));
        token->keyword = keywords[i];
        token->keyword_length = strlen(keywords[i]);
        token->value = values[i];
        linkedListInsertOrdered(lang->tokens, token, insertToken);
    }

    return lang;
}

void destroyLanguage(Language* lang)
{
    destroyLinkedList(lang->tokens);
    free(lang);
    lang = NULL;
}

char* tokeniseString(Language* lang, char* input, unsigned int length)
{
    LanguageToken* m;
    char* result;
    char* currentString;
    char c;
    unsigned int i, j;
    unsigned int rpos; // Position in result buffer

    // Initialise and clear result buffer
    result = (char*) malloc(sizeof(char) * (length + 1));
    bzero(result, length + 1);

    // Set buffer position to start of result buffer; 0
    rpos = 0;
    for(i = 0; i < length; i++)
    {
        // Move on to next character of string; ignore the first i characters
        currentString = (char*)input + i;

        // Look for keyword matches in the current string
        m = findMatch(lang, currentString, length - i);

        // Are we entering a quoted string
        if(currentString[0] == '"')
        {
            // Skip to end of quote mode; next occurence of "
            j = getIndexOfChr(currentString + 1, '"');

            if(j == -1)
            {
                printf(
                    "Error: Expected terminating \" at end of string data.\n"
                );
                return NULL;
            }

            // Copy quote directly
            strncat(result, currentString, j + 2);

            // Increase rpos to match the length of the inserted text
            rpos += j + 2;

            // Move input buffer to end of quote
            i += j + 1;
            continue;
        }

        if(m) // If match was found
        {
            result[rpos++] = m->value; // Insert token

            // Move position in input by token length
            i += m->keyword_length - 1;
        }
        else
        {
            // Input character as normal
            result[rpos++] = input[i]; 
        }
    }

    return result;
}

LanguageToken* findMatch(Language* lang, char* input, unsigned int length)
{
    LinkedListNode* n;
    LanguageToken* token;
    int r;

    // Attempt to find match
    n = linkedListGetHead(lang->tokens);
    while(n != NULL)
    {
        token = (LanguageToken*) n->data;
        if(token->keyword_length <= length)
        {
            r = strncmp(token->keyword, input, token->keyword_length);
            if(r == 0) { // Exact match found
                return token;
            }
        }
        n = n->next;
    }

    return NULL;
}