#include "PrintCallbackWrapper.h"

void ns_setPrintCallback(void* pData, ns_PrintCallback callback) {
	ns::setPrintCallback(pData, callback);
}

void ns_print(ns_PrintLevel level, const char* message) {
    ns::print(level, message);
}

void ns_printUnknownCommand(const char* command) {
    ns::printUnknownCommand(command);
}

const char* ns_levelToString(ns_PrintLevel level) {
    return ns::levelToString(level);
}
