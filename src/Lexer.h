#pragma once

#include "Token.h"

namespace sci {
    class Lexer {
    public:
        const char* input = nullptr;
        size_t inputSize = 0;

        Lexer(const char* input = nullptr, size_t inputSize = 0);

        Token advance();
    };
}