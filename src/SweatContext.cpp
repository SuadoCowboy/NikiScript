#include "SweatContext.h"

std::string& sci::Arguments::getString() {
    return arguments[offset++];
}

bool sci::Arguments::getFloat(SweatContext&, float&) {return 0;}
bool sci::Arguments::getDouble(SweatContext&, double&) {return 0;}