#include "Command.h"

sci::Command::Command() {}

sci::Command::Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs,
    CommandCallback callback, const std::string_view& usage, const std::vector<std::string_view>& argsDescriptions/*, void* pData*/)
  : name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), usage(usage), argsDescriptions(argsDescriptions)/*,
    pData(pData)*/ {}