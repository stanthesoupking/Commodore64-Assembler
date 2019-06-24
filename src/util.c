#include "util.h"

int getIndexOfChr(char* str, char c)
{
    int pos;

    pos = 0;
    while(str[pos] != '\0')
    {
        if(str[pos] == c)
        {
            return pos;
        }
        pos++;
    }

    return -1; // No match found
}