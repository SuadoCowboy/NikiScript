#include "Token.h"

sci::Token::Token() {}
sci::Token::Token(TokenType type) : type(type) {}
sci::Token::Token(TokenType type, const std::string& value) : type(type), value(value) {}