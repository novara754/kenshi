#pragma once

#include <stddef.h>

// Receive one character from the keyboard
char kb_getc(void);

// Receive one line (delimited by a \n) from the keyboard.
// Returns number of chars read.
void kb_gets(char *buf, size_t buf_size);

// Clear the input buffer
void kb_flush(void);
