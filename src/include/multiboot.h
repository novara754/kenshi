#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((packed)) multiboot_device {
	uint8_t part3;
	uint8_t part2;
	uint8_t part1;
	uint8_t drive;
} multiboot_device;

typedef struct __attribute__((packed)) multiboot_mod {
	void *mod_start;
	void *mod_end;
	const char *str;
	uint32_t reserved;
} multiboot_mod;

typedef struct __attribute__((packed)) multiboot_syms {
	uint32_t tabsize;
	uint32_t strsize;
	void *addr;
	uint32_t reserved;
} multiboot_syms;

typedef struct __attribute__((packed)) multiboot_mmap {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} multiboot_mmap;

typedef uint8_t multiboot_drive_ports;

typedef struct __attribute__((packed)) multiboot_drive {
	uint32_t size;
	uint8_t number;
	uint8_t mode;
	uint16_t cylinders;
	uint8_t heads;
	multiboot_drive_ports ports;
} multiboot_drive;

typedef struct __attribute__((packed)) multiboot_apm_table {
	uint16_t version;
	uint16_t cseg;
	uint16_t offset;
	uint16_t cseg16;
	uint16_t dseg;
	uint16_t flags;
	uint16_t cseg_len;
	uint16_t cseg16_len;
	uint16_t dseg_len;
} multiboot_apm_table;

typedef struct __attribute__((packed)) multiboot_info {
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	multiboot_device boot_device;
	const char *cmdline;
	uint32_t mods_count;
	multiboot_mod *mods;
	multiboot_syms syms;
	uint32_t mmap_length;
	multiboot_mmap *mmaps;
	uint32_t drives_length;
	multiboot_drive *drives_addr;
	uint32_t config_table;
	const char *boot_loader_name;
	multiboot_apm_table *apm_table;
	uint32_t vba_control_info;
	uint32_t vba_mode_info;
	uint16_t vba_mode;
	uint16_t vba_interface_seg;
	uint16_t vba_interface_off;
	uint32_t vba_interface_len;
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;
	uint32_t color_info;
	uint16_t color_info2;
} multiboot_info;

typedef enum multiboot_flag {
	MB_MEM = 1 << 0,
	MB_BOOT_DEV = 1 << 1,
	MB_CMDLINE = 1 << 2,
	MB_MODS = 1 << 3,
	MB_SYMS1 = 1 << 4,
	MB_SYMS2 = 1 << 5,
	MB_MMAP = 1 << 6,
	MB_DRIVES = 1 << 7,
	MB_CONFIG = 1 << 8,
	MB_BL_NAME = 1 << 9,
	MB_APM = 1 << 10,
	MB_VBE = 1 << 11,
	MB_FB = 1 << 12,
} multiboot_flag;

bool mb_flags(multiboot_info *mb, multiboot_flag flags);

#endif
