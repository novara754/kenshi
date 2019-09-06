#ifndef KERNEL_STDIO_H
#define KERNEL_STDIO_H

__attribute__((format(printf, 1, 2))) void printf(const char *format, ...);

#endif
