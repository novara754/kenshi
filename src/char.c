#include "char.h"

inline bool isdigit(char c) {
	return c >= '0' && c <= '9';
}

inline bool isalpha(char c) {
	return c >= 'a' && c <= 'z';
}

inline bool isalphanum(char c) {
	return isdigit(c) || isalpha(c);
}

bool isprinting(char c) {
	// 0x7F : DEL
	return c >= ' ' && c != 0x7F;
}
