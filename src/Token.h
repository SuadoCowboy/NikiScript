#pragma once
#include <cstring>

#ifndef SWEATCI_STATEMENT_SEPARATOR
#define SWEATCI_STATEMENT_SEPARATOR ';'
#endif

namespace sci {
    enum TokenType {
        NONE = 0,
        IDENTIFIER,
        STRING,
        EOS,
        EOF,
    };

    /// @brief either keep the original char array alive so that value won't have issues, or allocate to value
    struct Token {
        TokenType type = NONE;
        const char* value = nullptr;
        size_t valueSize = 0;
    };
}