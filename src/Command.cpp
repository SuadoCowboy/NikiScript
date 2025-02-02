#include "Command.h"

sci::Command::Command() {}

sci::Command::Command(const char* name, unsigned char minArgs, unsigned char maxArgs,
    CommandCallback callback, const char* description, void* pData)
  : name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), description(description),
    pData(pData) {}

void sci::Command::call(CommandContext& ctx) {
    ctx.pData = pData;
    ctx.pCommand = this;

    callback(ctx);
}