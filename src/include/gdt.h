#pragma once

#include <stddef.h>
#include <stdint.h>

// null segment, kernel code segment, kernel data segment, user code segment, user data segment, tss.
#define GDT_ENTRIES 6

typedef struct __attribute__((__packed__)) gdt_entry {
	uint16_t limit_lo;
	uint16_t base_lo;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags; // Lower 4 bits contain limit 16~19
	uint8_t base_hi;
} gdt_entry;

typedef enum gdt_access {
	GDTA_ACCESSED = 1 << 0,
	GDTA_READWRITE = 1 << 1,
	GDTA_DIRECTION = 1 << 2,
	GDTA_EXECUTABLE = 1 << 3,
	GDTA_NOTSYSTEM = 1 << 4,
	GDTA_PRIV0 = 0 << 5,
	GDTA_PRIV1 = 1 << 5,
	GDTA_PRIV2 = 2 << 5,
	GDTA_PRIV3 = 3 << 5,
	GDTA_PRESENT = 1 << 7,
} gdt_access;

typedef enum gdt_flag {
	GDT_FLAG_SIZE = 1 << 2,
	GDT_FLAG_GRANULARITY = 1 << 3,
} gdt_flag;

void gdt_set_entry(size_t index, uint32_t base, uint32_t limit, uint8_t access);
void gdt_load(void);
