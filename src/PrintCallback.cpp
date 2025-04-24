#include "PrintCallback.h"

ns::PrintCallback ns::printCallback = nullptr;
void* ns::pPrintCallbackData = nullptr;

const char* ns::levelToString(PrintLevel level) {
	switch (level) {
		case PrintLevel::DEFAULT:
			return "DEFAULT";
		case PrintLevel::ECHO:
			return "ECHO";
		case PrintLevel::WARNING:
			return "WARNING";
		case PrintLevel::ERROR:
			return "ERROR";
		default:
			return "UNKNOWN";
		}
}

void ns::setPrintCallback(void* pData, PrintCallback callback) {
	printCallback = callback;
	pPrintCallbackData = pData;
}

void ns::print(ns::PrintLevel level, const char* str) {
	printCallback(pPrintCallbackData, level, str);
}

void ns::printUnknownCommand(const char* command) {
	printf(PrintLevel::ERROR, "Unknown command \"{}\"\n", command);
}

void ns::printAppendToString(void* pBuffer, ns::PrintLevel, const char* string) {
	static_cast<std::string*>(pBuffer)->append(string);
}

void ns::printAppendToStringEchoOnly(void* pBuffer, ns::PrintLevel level, const char* string) {
	if (level == PrintLevel::ECHO)
		static_cast<std::string*>(pBuffer)->append(string);
}