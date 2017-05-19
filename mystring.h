#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include <stdlib.h>

#define STRING_NO_MEMORY -1
#define STRING_SUCCESS    0
#define STRING_SIZE 64

typedef struct {
    char cstring[STRING_SIZE];
    uint32_t length;
} String;

uint32_t string_copy(String *to, String *from);
uint32_t string_append(String *string, char c);

#endif // __MYSTRING_H__
