#include "Token.h"

sci::Token::Token() {}
sci::Token::Token(TokenType type) : type(type), value("") {}
sci::Token::Token(TokenType type, const std::string& value) : type(type), value(value) {}

uint8_t operator|(sci::TokenType l, sci::TokenType r) {
    return static_cast<uint8_t>(l)|static_cast<uint8_t>(r);
}

uint8_t operator|(uint8_t l, sci::TokenType r) {
    return l|static_cast<uint8_t>(r);
}

uint8_t operator|(sci::TokenType l, uint8_t r) {
    return static_cast<uint8_t>(l)|r;
}

uint8_t operator&(uint8_t l, sci::TokenType r) {
    return l&static_cast<uint8_t>(r);
}

uint8_t operator&(sci::TokenType l, uint8_t r) {
    return static_cast<uint8_t>(l)&r;
}