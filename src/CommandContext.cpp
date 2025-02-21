#include "CommandContext.h"

#include <string>

#include "Token.h"
#include "PrintCallback.h"

sci::SweatString& sci::Arguments::getString() {
    return arguments[offset++];
}

int8_t sci::Arguments::getInt8(CommandContext& ctx) {
    const SweatString& str = arguments[offset++];
    
    try {
        return (int8_t)std::stoi(str.format);
    } catch (...) {
        sci::print(sci::_ERROR, "\n");
        ctx.pCommand->description
    }
}

uint8_t sci::Arguments::getUInt8(CommandContext& ctx) {}

int16_t sci::Arguments::getInt16(CommandContext& ctx) {}
uint16_t sci::Arguments::getUInt16(CommandContext& ctx) {}

int32_t sci::Arguments::getInt32(CommandContext& ctx) {}
uint32_t sci::Arguments::getUInt32(CommandContext& ctx) {}

int64_t sci::Arguments::getInt64(CommandContext& ctx) {}
uint64_t sci::Arguments::getUInt64(CommandContext& ctx) {}

float sci::Arguments::getFloat(CommandContext& ctx) {}
double sci::Arguments::getDouble(CommandContext& ctx) {}