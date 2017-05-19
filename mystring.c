#include <stdlib.h>
#include <string.h>

#include "mystring.h"

uint32_t string_copy(String *to, String *from)
{
        strncpy(to->cstring, from->cstring, from->length + 1);
        to->length = from->length;
        return STRING_SUCCESS;
}

uint32_t string_append(String *string, char c)
{
    if (string->length + 1 == STRING_SIZE)
        return STRING_NO_MEMORY;
    else
        string->cstring[string->length++] = c;
    return STRING_SUCCESS;
}