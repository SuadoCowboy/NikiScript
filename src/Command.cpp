#include "Command.h"

sci::Command::Command() {}

sci::Command::Command(const char* name, unsigned char minArgs, unsigned char maxArgs,
    CommandCallback callback, char* usage, char** argsDescriptions, void* pData)
  : name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), usage(usage), argsDescriptions(argsDescriptions),
    pData(pData) {}

void sci::Command::call(CommandContext& ctx) {
    ctx.pData = pData;
    ctx.pCommand = this;

    callback(ctx);
}