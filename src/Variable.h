#pragma once

#include <stdint.h>
#include <string>

namespace sci {
    enum class VariableType : uint8_t {
        NONE = 0,
        STRING,
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
        VariableType type = VariableType::NONE;
        void* pVariable = nullptr;
        std::string name = nullptr;

        Variable();
        Variable(const std::string& name, VariableType type, void* pVariable);
    };
}
