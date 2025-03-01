#include "Command.h"

#include <sstream>

#include "PrintCallback.h"

sci::Command::Command() {}

sci::Command::Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs,
    CommandCallback callback, const std::string_view& usage, const std::vector<std::string_view>& argsDescriptions/*, void* pData*/)
  : name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), usage(usage), argsDescriptions(argsDescriptions)/*,
    pData(pData)*/ {}

void sci::Command::printUsage() {
    std::stringstream oss;
    bool isName = true;

    for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
        if (isName)
            oss << argsDescriptions[i] << ": ";
        else
            oss << argsDescriptions[i] << ';';

        isName = !isName;
    }

    sci::printf(PrintLevel::WARNING, "Usage: {}\nArguments: {}", usage, oss.str());
}