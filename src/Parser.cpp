#include "Parser.h"

#include "PrintCallback.h"

uint64_t sci::maxConsoleVariableCalls = 10000;

void sci::handleCommandCall(SweatContext& ctx) {
    if (ctx.pCommand == nullptr)
        return;

    ctx.arguments.offset = 0;
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

        sci::printf(sci::PrintLevel::ECHO, "{} {}\n", ctx.pCommand->name, ctx.pCommand->getArgumentsNames());
        return;
    }

    ctx.pCommand->callback(ctx);
    ctx.arguments.arguments.clear();
    ctx.pCommand = nullptr;
    ctx.pData = nullptr;
}

void sci::handleStringToken(SweatContext& ctx) {
    expandStringToken(ctx, ctx.pLexer->token);

    char type = 0;
    bool isName = true;
    type = ctx.pCommand->argsDescriptions[ctx.arguments.arguments.size()*2][0];

    for (auto& arg : ctx.pCommand->argsDescriptions) {
        if (isName && !arg.empty()) {
            
        }

        isName = !isName;
    }

    // TODO: if parameter should be number (or reference and string is not a reference) then print usage and explain
    // TODO: if is reference, check if reference is number if parameter should be number
    if (ctx.pCommand->maxArgs == 1 && !ctx.arguments.arguments.empty())
        ctx.arguments.arguments[0] += ctx.pLexer->token.value;
    else
        ctx.arguments.arguments.push_back(ctx.pLexer->token.value);
}

void sci::handleConsoleVariableCall(SweatContext& ctx) {
    Lexer* pOriginalLexer = ctx.pLexer;

    std::vector<Lexer> tempLexers;
    tempLexers.emplace_back(ctx.consoleVariables[ctx.pLexer->token.value]);
    
    ctx.pLexer = &tempLexers.back();
    ctx.pLexer->advance();
    
    //ctx.runningFrom |= ALIAS;

    while (tempLexers.size() != 0) {
        if (ctx.pLexer->token.type == TokenType::IDENTIFIER) {
            if (ctx.consoleVariables.count(ctx.pLexer->token.value) != 0) {
                if (maxConsoleVariableCalls != 0 && tempLexers.size() >= maxConsoleVariableCalls) {
                    ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
                } else {
                    tempLexers.emplace_back(ctx.consoleVariables[ctx.pLexer->token.value]);
                    ctx.pLexer = &tempLexers.back();
                }

            } else {
                ctx.pCommand = ctx.commands.get(ctx.pLexer->token.value);

                if (ctx.pCommand == nullptr) {
                    sci::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", ctx.pLexer->token.value);
                    ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
                }
            }
        }

        switch (ctx.pLexer->token.type) {
        case TokenType::EOS:
            handleCommandCall(ctx);
            break;

        case TokenType::NUMBER:
            ctx.arguments.arguments.push_back(ctx.pLexer->token.value);
            break;

        case TokenType::STRING:
            handleStringToken(ctx);
            break;

        default:
            break;
        }

        ctx.pLexer->advance();
        while (ctx.pLexer->token.type == TokenType::END) {
            handleCommandCall(ctx);

            tempLexers.pop_back();
            if (tempLexers.size() == 0)
                break;

            ctx.pLexer = &tempLexers.back();
            ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
        }
    }

    handleCommandCall(ctx);

    //ctx.runningFrom &= ~ALIAS;
    ctx.pLexer = pOriginalLexer;
    
    // if there's something between the alias and the end of statement, we don't care!
    ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
}

void sci::parse(SweatContext& ctx) {
    if (ctx.pLexer == nullptr)
        return;

    ctx.pLexer->advance();
    while (ctx.pLexer->token.type != TokenType::END) {
        switch (ctx.pLexer->token.type) {
        case TokenType::IDENTIFIER: // can be either variable or command
            if (ctx.consoleVariables.count(ctx.pLexer->token.value) != 0) {
                handleConsoleVariableCall(ctx);
                break;
            } else if (ctx.programVariables.count(ctx.pLexer->token.value) != 0) {
                ctx.pCommand = ctx.commands.get("_program_variable_callback");
                if (ctx.pCommand != nullptr)
                    ctx.pData = &ctx.programVariables[ctx.pLexer->token.value];
                break;
            }

            ctx.pCommand = ctx.commands.get(ctx.pLexer->token.value);
            if (ctx.pCommand == nullptr) {
                sci::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", ctx.pLexer->token.value);
                ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
            }
            break;

        case TokenType::EOS:
            handleCommandCall(ctx);
            break;

        case TokenType::NUMBER:
            ctx.arguments.arguments.push_back(ctx.pLexer->token.value);
            break;

        case TokenType::STRING:
            handleStringToken(ctx);
            break;

        default:
            break;
        }
    
        ctx.pLexer->advance();
    }

    handleCommandCall(ctx);
}