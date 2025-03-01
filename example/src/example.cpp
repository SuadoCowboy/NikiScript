#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

struct Color {
    uint8_t r=0,g=0,b=0,a=255;
};

#define SWEATCI_ARGUMENTS_EXTRA bool getColor(SweatContext& ctx, Color& output);

#include <SweatCI.h>
#include <PrintCallback.h>
#include <SweatContext.h>
#include <Token.h>
#include <Lexer.h>
#include <Parser.h>

bool parseStringToColor(const std::string& str, Color& buf, bool printError) {
    Color color = {0,0,0,255};

    try {
        size_t secondCommaIdx;
        {
            size_t firstCommaIdx = str.find(",");
            color.r = std::stoi(str.substr(0, firstCommaIdx));
            
            secondCommaIdx = str.find(",", firstCommaIdx+1);
            color.g = std::stoi(str.substr(firstCommaIdx+1, secondCommaIdx-firstCommaIdx-1));
        }

        size_t thirdCommaIdx = str.find(",", secondCommaIdx+1);

        color.b = std::stoi(str.substr(secondCommaIdx+1, thirdCommaIdx-secondCommaIdx-1));
        if (thirdCommaIdx != std::string::npos)
            color.a = std::stoi(str.substr(thirdCommaIdx+1));

        buf = color;
        return true;

    } catch (...) {
        if (printError)
            sci::printf(sci::PrintLevel::ERROR, "expected \"RRR,GGG,BBB\" or \"RRR,GGG,BBB,AAA\" but received \"{}\"\n", str);

        return false;
    }
}

bool sci::Arguments::getColor(SweatContext& ctx, Color& output) {
    if (!parseStringToColor(arguments[offset++], output, false)) {
        sci::printf(sci::PrintLevel::ERROR, "\"{}\" is not a valid color. Argument #{}: {}\n", arguments[offset-1], offset-1, ctx.pCommand->maxArgs == 1? ctx.pCommand->argsDescriptions[0] : ctx.pCommand->argsDescriptions[offset-1]);
        return false;
    }

    return true;
}

void sweatciPrintCallback(void*, sci::PrintLevel level, const std::string& msg) {
    std::cout << sci::printLevelToString(level) << ": " << msg;
}

std::string tokenTypeToString(const sci::TokenType& type) {
    switch (type) {
    case sci::TokenType::NONE:
        return "NONE";
    case sci::TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case sci::TokenType::ARGUMENT:
        return "ARGUMENT";
    case sci::TokenType::EOS:
        return "EOS";
    case sci::TokenType::END:
        return "END";
    default:
        return "UNKNOWN";
    }
}

std::string tokenToString(const sci::Token& token) {
    std::stringstream out;
    out << '(' << tokenTypeToString(token.type) << ", \"" << token.value << "\", REFS: {";

    std::string formatted = token.value;
    if (token.references.empty())
        return out.str()+"})";

    else for (auto& ref : token.references) {
        out << '(' << ref.first << ", " << ref.second << "), ";
        formatted.insert(ref.first, ref.second);
    }

    std::string outString = out.str();
    outString.replace(outString.begin()+outString.size()-2, outString.begin()+outString.size()-1, "}) -> ");

    return outString + formatted;
}

int main(int, char**) {
    sci::setPrintCallback(nullptr, sweatciPrintCallback);

    // if (argc <= 1) {
    //   sci::printf(sci::PrintLevel::ERROR, "Usage: \"{}\" <input>\n", argv[0]);
    //   return EXIT_FAILURE;
    // }

    std::string input = "test 5 abasad";
    // for (int i = 1; i < argc; ++i)
    //    input += std::string(argv[i]) + " ";

    sci::SweatContext ctx;
    sci::registerCommands(ctx);

    std::string penes = "Penes";
    sci::registerVariable(ctx, "penes", &penes, sci::getString, sci::setString);

    sci::Lexer lexer{input};
    ctx.pLexer = &lexer;

    sci::parse(ctx);
}