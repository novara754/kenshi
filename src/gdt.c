#include "gdt.h"
#include "stdio.h"

extern void segment_reload(void);

gdt_entry gdt[GDT_ENTRIES];

struct __attribute__((__packed__)) gdtd {
	uint16_t size;
	gdt_entry *offset;
} gdtd = {
    .size = GDT_ENTRIES * 8 - 1,
    .offset = gdt,
};

void gdt_set_entry(size_t index, uint32_t base, uint32_t limit, uint8_t access) {
	if ((limit > 65536) && ((limit & 0xFFF) != 0xFFF)) {
		printf("[GDT] invalid segment limit for entry #%zi, skipping...\n", index);
		return;
	}

	gdt_entry entry;

	if (limit > 65536) {
		// If the intended limit is greater than 65536 we need to enable page granularity (4 KiB blocks).
		// This also means we can divide the limit by 4 KiB (2^12).
		limit >>= 12;
		entry.flags = 0xC0;
	} else {
		entry.flags = 0x40;
	}

	entry.limit_lo = limit & 0xFFFF;
	entry.flags |= limit >> 16;
	entry.base_lo = base & 0xFFFF;
	entry.base_mid = (base >> 16) & 0xFF;
	entry.base_hi = (base >> 24) & 0xFFFF;
	entry.access = access;

	gdt[index] = entry;
}

gdt_entry *gdt_get_entry(size_t index) {
	return &gdt[index];
}

void gdt_load(void) {
	printf("[GDT] Loading GDT... ");
	asm volatile("lgdt %0" ::"m"(gdtd));
	segment_reload();
	printf("Done\n");
}
