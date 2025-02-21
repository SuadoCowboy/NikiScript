#include "SweatString.h"

#include <sstream>

#include "Token.h"

uint8_t sci::SweatString::getCharOrVariable(uint64_t& offset, Variable*& pVariableOut, char*& charOut) {
    for (; offset < format.size(); ++offset) {
        if (format[offset] == SWEATCI_REFERENCE && references.count(offset) != 0) {
            pVariableOut = references[offset];
            return 1;
        }

        *charOut = format[offset];
        return 0;
    }

    return 2;
}