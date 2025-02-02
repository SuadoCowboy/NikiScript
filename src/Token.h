#pragma once
#include <cstring>

#ifndef SWEATCI_STATEMENT_SEPARATOR
#define SWEATCI_STATEMENT_SEPARATOR ';'
#endif

#ifndef SWEATCI_REFERENCE
#define SWEATCI_REFERENCE '$'
#endif

#ifndef SWEATCI_REFERENCE_OPEN
#define SWEATCI_REFERENCE_OPEN '{'
#endif

#ifndef SWEATCI_REFERENCE_CLOSE
#define SWEATCI_REFERENCE_CLOSE '}'
#endif

namespace sci {
    enum TokenType : unsigned char {
        NONE = 0,
        IDENTIFIER,
        REFERENCE, // '$' example: var "my var" "hello there"; echo "${my var}, how are you?". Yes it can use whitespace, but it's suggested not to
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