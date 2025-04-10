#pragma once

#include "DLLExport.h"
#include "PrintCallback.h"

typedef ns::PrintLevel ns_PrintLevel;
typedef ns::PrintCallback ns_PrintCallback;

extern "C" {
	// ns::setPrintCallback
	NIKIAPI void ns_setPrintCallback(void* pData, ns_PrintCallback callback);

	// wraps ns::print
	NIKIAPI void ns_print(ns_PrintLevel level, const char* message);

	// ns::printUnknownCommand
	NIKIAPI void ns_printUnknownCommand(const char* command);

	// ns::printLevelToString
	NIKIAPI const char* ns_levelToString(ns_PrintLevel level);
}