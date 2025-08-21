#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <locale>

bool ns::isSpaceNotNewline(char c) {
	return isspace(c) && c != '\n';
}