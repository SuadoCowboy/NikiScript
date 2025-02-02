#pragma once

#include "Token.h"

namespace sci {
    class Lexer {
    public:
        const char* input = nullptr;
        size_t inputSize = 0;
        size_t position = 0;
        Token firstToken = NONE;

        Lexer(const char* input = nullptr, size_t inputSize = 0);

        Token advance();

        /// @brief when an identifier is found
        /// @return 
        Token parseArgument();
        Token parseQuotedString();
        Token parseString();

        /// @brief only sets the first token if firstToken.type is NONE 
        void setFirstToken(const Token& token);
    };
}