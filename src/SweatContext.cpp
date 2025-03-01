#include "SweatContext.h"

std::string& sci::Arguments::getString() {
    return arguments[offset++];
}

bool sci::Arguments::getFloat(SweatContext& ctx, float& output) {
    try {
        output = std::stof(arguments[offset++]);
        return true;
    } catch (...) {
        sci::printf(sci::PrintLevel::ERROR, "\"{}\" is not a valid float. Argument #{}: {}\n", arguments[offset-1], offset-1, ctx.pCommand->maxArgs == 1? ctx.pCommand->argsDescriptions[0] : ctx.pCommand->argsDescriptions[offset-1]);
        return false;
    }
}

bool sci::Arguments::getDouble(SweatContext& ctx, double& output) {
    try {
        output = std::stod(arguments[offset++]);
        return true;
    } catch (...) {
        sci::printf(sci::PrintLevel::ERROR, "\"{}\" is not a valid float. Argument #{}: {}\n", arguments[offset-1], offset-1, ctx.pCommand->maxArgs == 1? ctx.pCommand->argsDescriptions[0] : ctx.pCommand->argsDescriptions[offset-1]);
        return false;
    }
}