#include "string.h"
#include "math.h"

size_t strlen(const char *str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

bool strchr(const char *str, char c) {
	for (size_t i = 0; str[i]; i++) {
		if (str[i] == c) {
			return true;
		}
	}

	return false;
}

static char *last_strtok_str = NULL;
char *strtok(char *str, char *delim) {
	if (str == NULL && last_strtok_str == NULL) {
		return NULL;
	}

	if (str == NULL) {
		str = last_strtok_str;
	}

	// Skip and delim chars at the beginning of the string.
	while (strchr(delim, *str)) {
		str++;
	}

	char *start = str;

	// Now skip all characters until the next delim is found.
	while (!strchr(delim, *str)) {
		str++;
	}

	// Indicate end of token with NUL.
	*str = 0;
	last_strtok_str = str + 1;

	return start;
}

uint32_t strcmp(const char *str1, const char *str2) {
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	size_t len = max(len1, len2);
	for (size_t i = 0; i < len; i++) {
		if (i >= len1) {
			return -1;
		} else if (i >= len2) {
			return 1;
		} else {
			char diff = str1[i] - str2[i];
			if (diff != 0) {
				return diff;
			}
		}
	}

	return 0;
}
