#pragma once
#include <stdint.h>
#include <string>

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
    enum class TokenType : uint8_t {
        NONE = 0,
        IDENTIFIER,
        REFERENCE, // '$' example: var "my var" "hello there"; echo "${my var}, how are you?". Yes it can use whitespace, but it's suggested not to
        STRING,
        EOS, // End Of Statement
        END
    };

    /// @brief either keep the original char array alive so that value won't have issues, or allocate to value
    struct Token {
        TokenType type = TokenType::NONE;
        std::string value;
        uint64_t valueSize = 0;

        Token();
        Token(TokenType type);
        Token(TokenType type, const std::string& value, uint64_t valueSize);
    };
}