#include "Parser.h"

#include "PrintCallback.h"

void sci::handleCommandCall(SweatContext& ctx) {
    if (ctx.pCommand->maxArgs == 1 && ctx.arguments.arguments.size() > 1) {
        std::string argument = "";
        for (const auto& arg : ctx.arguments.arguments)
            argument += arg+' ';

        ctx.arguments.arguments = {argument.substr(0, argument.size()-1)};
    }

    if (ctx.pCommand->minArgs > ctx.arguments.arguments.size() || ctx.arguments.arguments.size() > ctx.pCommand->maxArgs) {
        if (ctx.pCommand->minArgs == ctx.pCommand->maxArgs)
            sci::printf(sci::PrintLevel::ERROR, "Expected {} argument(s) but received {} argument(s)\n", static_cast<uint16_t>(ctx.pCommand->minArgs), ctx.arguments.arguments.size());
        else
            sci::printf(sci::PrintLevel::ERROR, "Expected arguments between [{}, {}] but received {} argument(s)\n", static_cast<uint16_t>(ctx.pCommand->minArgs), static_cast<uint16_t>(ctx.pCommand->maxArgs), ctx.arguments.arguments.size());

        sci::printf(sci::PrintLevel::ECHO, "{} {}", ctx.pCommand->name, ctx.pCommand->getArgumentsNames());
        return;
    }

    ctx.pCommand->callback(ctx);
    ctx.arguments.arguments.clear();
}

void sci::parse(SweatContext& ctx) {
    if (ctx.pLexer == nullptr)
        return;

    ctx.pLexer->advance();
    while (ctx.pLexer->token.type != TokenType::END) {
        switch (ctx.pLexer->token.type) {
        case TokenType::IDENTIFIER: { // can be either variable or command
            if (ctx.variables.count(ctx.pLexer->token.value) != 0) {
                ctx.variables[ctx.pLexer->token.value]; // TODO
                break;
            }

            ctx.pCommand = ctx.commands.get(ctx.pLexer->token.value);
            if (ctx.pCommand == nullptr) {
                sci::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", ctx.pLexer->token.value);
                ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
            }
            break;
        }

        case TokenType::EOS:
            handleCommandCall(ctx);
            break;

        case TokenType::NUMBER:
            ctx.arguments.arguments.push_back(ctx.pLexer->token.value);
            break;

        case TokenType::STRING:
            // TODO: if parameter should be number (or reference and string is not a reference) then print usage and explain
            // TODO: if is reference, check if reference is number if parameter should be number
            ctx.arguments.arguments.push_back(ctx.pLexer->token.value);
            break;

        default:
            break;
        }
    
        ctx.pLexer->advance();
    }

    handleCommandCall(ctx);
}