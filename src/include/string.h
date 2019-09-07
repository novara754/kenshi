#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Calculates the length of a null-terminated string.
size_t strlen(const char *str);

// Split a string into tokens, by replacing the delimiters with NUL.
// Return value is the start of the token, end of the token is represented by NUL.
char *strtok(char *str, char *delim);

// Checks if char is included in a string.
bool strchr(const char *str, char c);

// Compare two strings.
// Sign of the return value is the sign of the difference; 0 if equal.
uint32_t strcmp(const char *str1, const char *str2);
