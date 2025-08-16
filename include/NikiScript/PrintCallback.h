#pragma once

#include <stdint.h>
#include <string>

#include "DLL.h"
#include "Utils.h"

namespace ns {
	enum PrintLevel : uint8_t {
		DEFAULT = 0, ///< any user input
		ECHO, ///< any text that came from a command that is not an error
		WARNING, ///< anything that is wrong but can continue
		ERROR, ///< anything that went wrong and can not continue
	};

	NS_API const char* levelToString(PrintLevel level);

	typedef void(*PrintCallback)(void* pData, PrintLevel level, const char* message);

	extern NS_API PrintCallback printCallback;
	extern NS_API void* pPrintCallbackData;

	NS_API void setPrintCallback(void* pData, PrintCallback callback);
	NS_API void print(PrintLevel level, const char* str);
	NS_API void printUnknownCommand(const char* command);

	template<typename... Args>
	void printf(PrintLevel level, const char* format, Args&& ... args) {
		std::string formatted = formatString(format, args...);
		ns::print(level, formatted.c_str());
	}

	/**
	 * @brief appends string to pBuffer if level is PrintLevel::ECHO
	 * @param pBuffer should be std::string*
	 * @param level should be ECHO to be appended to the string
	 * @param string string to append to pBuffer
	 */
	NS_API void printAppendToStringEchoOnly(void* pBuffer, PrintLevel level, const char* string);
}
