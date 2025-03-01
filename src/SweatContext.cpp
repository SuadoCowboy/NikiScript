#include "SweatContext.h"

std::string& sci::Arguments::getString() {
    return arguments[offset++];
}

float sci::Arguments::getFloat() {
    return std::stof(arguments[offset++]);
}

double sci::Arguments::getDouble() {
    return std::stod(arguments[offset++]);
}