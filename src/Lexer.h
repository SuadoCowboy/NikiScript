#pragma once

#include "Token.h"

namespace sci {
    class Lexer {
    public:
        const char* input = nullptr;
        size_t inputSize = 0;
        size_t position = 0;
        Token previousToken = NONE;

        Lexer(const char* input = nullptr, size_t inputSize = 0);

        Token advance();

        /// @brief when an identifier is found
        /// @return 
        Token parseArguments();
        Token parseQuotedString();
        Token parseString();
    };
}