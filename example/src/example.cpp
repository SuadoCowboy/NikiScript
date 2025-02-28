#include <iostream>
#include <string>
#include <stdint.h>

/*struct Color {
    uint8_t r=0,g=0,b=0,a=255;
};*/

//#define SWEATCI_ARGUMENTS_EXTRA Color getColor();

#include <SweatCI.h>
#include <Token.h>
#include <Lexer.h>

/*Color sci::Arguments::getColor() {
    std::string& arg = getString();

    uint64_t argOffset = 0;
    Variable* pVariable = nullptr;
    char* c = nullptr;
    
    std::stringstream oss;
    while (true) {
        uint8_t i = arg.getCharOrVariable(argOffset, pVariable, c);
        if (i == 2)
            break;

        switch (i) {
        case 0: // char
            oss << c;
            c = nullptr;
            break;

        case 1: // variable
            pVariable->name;

        default:
            break;
        }
    }
}*/

void sweatciPrintCallback(void*, sci::PrintLevel level, const std::string& msg) {
    std::cout << sci::printLevelToString(level) << ": " << msg;
}

std::string tokenTypeToString(const sci::TokenType& type) {
    switch (type) {
    case sci::TokenType::NONE:
        return "NONE";
    case sci::TokenType::COMMAND:
        return "COMMAND";
    case sci::TokenType::STRING:
        return "STRING";
    case sci::TokenType::EOS:
        return "EOS";
    case sci::TokenType::END:
        return "END";
    default:
        return "UNKNOWN";
    }
}

std::string tokenToString(const sci::Token& token) {
    return std::string("(")+tokenTypeToString(token.type)+", \""+token.value+"\")";
}

int main(int, char**) {
    sci::setPrintCallback(nullptr, sweatciPrintCallback);

    //if (argc <= 1) {
    //    sci::printf(sci::PrintLevel::ERROR, "Usage: \"{}\" <input>\n", argv[0]);
    //    return EXIT_FAILURE;
    //}

    std::string input = "echo Hello, World!;      echo \"Ho                  w\" are you?";
    //for (int i = 1; i < argc; ++i)
    //    input += std::string(argv[i]) + " ";

    sci::SweatContext ctx;
    sci::registerCommands(ctx);

    sci::Lexer lexer{input};
    sci::Token token = {sci::TokenType::NONE};
    while (token.type != sci::TokenType::END) {
        token = lexer.advance();
        std::cout << tokenToString(token) << '\n';
    }
}