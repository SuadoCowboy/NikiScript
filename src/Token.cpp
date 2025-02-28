#include "Token.h"

sci::Token::Token() {}
sci::Token::Token(TokenType type) : type(type) {}
sci::Token::Token(TokenType type, const std::string& value, uint64_t valueSize) : type(type), value(value), valueSize(valueSize) {}