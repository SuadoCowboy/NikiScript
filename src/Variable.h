#pragma once

#include "Command.h"
#include "SweatString.h"

namespace sci {
    enum VariableType : uint8_t {
        STRING = 0,
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        INT64,
        UINT64,
        FLOAT,
        DOUBLE
    #ifdef SWEATCI_VARIABLE_TYPES_EXTRA
        ,SWEATCI_VARIABLE_TYPES_EXTRA
    #endif
    };

    struct Variable {
        VariableType type;
        void* pVariable = nullptr;

        const char* name = nullptr;
        CommandCallback callback = nullptr;
        const char* description = nullptr;

        Variable();
        Variable(const char* name, const char* description, VariableType type, void* pVariable);
    };
}
