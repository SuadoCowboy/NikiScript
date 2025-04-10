#include "BaseWrapper.h"

char* ns::allocStringToCharString(const std::string& src) {
	char* out = new char[src.size()+1];
	memcpy(out, src.c_str(), src.size());
	out[src.size()] = '\0';

	return out;
}

void ns_free2DCharArray(char* string) {
	delete[] string;
}

void ns_free2DCharArray(const char** strings) {
	delete[] strings;
}