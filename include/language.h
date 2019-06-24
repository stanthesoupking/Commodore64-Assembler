#pragma once

#include "linked_list.h"

typedef struct language_token_int
{
    char* keyword;
    unsigned int keyword_length;
    unsigned char value;
} LanguageToken;

typedef struct language_int {
    LinkedList* tokens; // Tokens are ordered from longest to shortest length
} Language;

Language* createLanguage(char** keywords, unsigned char* values, unsigned int count);
void destroyLanguage(Language* lang);
char* tokeniseString(Language* lang, char* input, unsigned int length);
LanguageToken* findMatch(Language* lang, char* input, unsigned int length);
int getIndexOfChr(char* str, char c);