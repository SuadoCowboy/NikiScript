#include <iostream>

#include <Lexer.h>

std::string tokenTypeToString(const sci::TokenType& type) {
    switch (type) {
    case sci::TokenType::NONE:
        return "NONE";
    case sci::TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case sci::TokenType::STRING:
        return "STRING";
    case sci::TokenType::EOS:
        return "EOS";
    case sci::TokenType::END:
        return "END";
    }
}

std::string tokenToString(const sci::Token& token) {
    return std::string("(")+tokenTypeToString(token.type)+", "+token.value+")";
}

int main(int argc, char** argv) {
    sci::Lexer lexer{argv[1], strlen(argv[1])};
    
    sci::Token token = {sci::TokenType::NONE};
    while (token.type != sci::TokenType::END) {
        token = lexer.advance();
        std::cout << tokenToString(token) << '\n';
    }
}