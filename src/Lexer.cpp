#include "Lexer.h"

sci::Lexer::Lexer(const std::string& input) : input(input) {}

sci::Token sci::Lexer::advance() {
    while (input[position] == ' ' && position < input.size())
        position++;

    Token token;
    if (position >= input.size()) {
        token = {TokenType::END};
        setFirstToken(token);
        return token;
    }

    /*
    echo "This is a quoted string"
    echo This is not a quoted string
    "what happens" if I do this? // I belive the output would be: Unknown Command: "what
    */

    if (firstToken.type == TokenType::IDENTIFIER)
        token = getArgument();
    else
        token = getIdentifier();

    setFirstToken(token);
    return token;
}

sci::Token sci::Lexer::getArgument() {
    switch (input[position]) {
    case '"':
        return getQuotedString();

    default:
        return getString();
    }
}

sci::Token sci::Lexer::getQuotedString() {

}

sci::Token sci::Lexer::getString() {
    
}

void sci::Lexer::setFirstToken(const Token& token) {
    if (firstToken.type == NONE)
        firstToken = token;
}