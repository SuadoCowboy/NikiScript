#pragma once
#include <cstring>

#ifndef SWEATCI_STATEMENT_SEPARATOR
#define SWEATCI_STATEMENT_SEPARATOR ';'
#endif

namespace sci {
    enum TokenType : unsigned char {
        NONE = 0,
        IDENTIFIER,
        STRING,
        EOS, // End Of Statement
        END
    };

    /// @brief either keep the original char array alive so that value won't have issues, or allocate to value
    struct Token {
        TokenType type = NONE;
        const char* value = nullptr;
        size_t valueSize = 0;

        Token();
        Token(TokenType type);
        Token(TokenType type, const char* value, size_t valueSize);
    };
}