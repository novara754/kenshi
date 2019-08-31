#include "multiboot.h"

inline bool mb_flags(multiboot_info *mb, multiboot_flag flags) {
	return (mb->flags & flags) == flags;
}
