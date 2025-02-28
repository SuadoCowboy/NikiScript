#include "PrintCallback.h"

sci::PrintCallback sci::printCallback = nullptr;
void* sci::pPrintCallbackData = nullptr;

std::string sci::printLevelToString(PrintLevel level) {
    switch (level) {
    case PrintLevel::DEFAULT:
        return std::string("DEFAULT");
    case PrintLevel::ECHO:
        return std::string("ECHO");
    case PrintLevel::WARNING:
        return std::string("WARNING");
    case PrintLevel::ERROR:
        return std::string("ERROR");
    default:
        return std::string("UNKNOWN");
    }

}

void sci::setPrintCallback(void* pData, PrintCallback callback) {
    printCallback = callback;
    pPrintCallbackData = pData;
}

void sci::print(const sci::PrintLevel &level, const std::string& str) {
    printCallback(pPrintCallbackData, level, str);
}

void sci::printUnknownCommand(const std::string& command) {
    printf(PrintLevel::ERROR, "Unknown command \"{}\"\n", command);
}