#include "Parser.h"

#include "PrintCallback.h"

void sci::parse(SweatContext& ctx) {
    if (ctx.pLexer == nullptr)
        return;

    ctx.pLexer->advance();
    while (ctx.pLexer->token.type != TokenType::END) {
        switch (ctx.pLexer->token.type) {
        case TokenType::IDENTIFIER: { // can be either variable or command
            if (ctx.variables.count(ctx.pLexer->token.value) != 0) {
                ctx.variables[ctx.pLexer->token.value];
                break;
            }

            ctx.pCommand = ctx.commands.get(ctx.pLexer->token.value);
            if (ctx.pCommand == nullptr) {
                sci::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", ctx.pLexer->token.value);
                ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
            }

            ctx.pLexer->advance();
            ctx.arguments.arguments.clear();

            if (ctx.pCommand->maxArgs > 1) {
                while (!((TokenType::EOS|TokenType::END) & ctx.pLexer->token.type))
                    ctx.arguments.arguments.push_back(ctx.pLexer->token.value);

            } else if (ctx.pCommand->maxArgs == 1) {
                std::string argument = "";

                while (!((TokenType::EOS|TokenType::END) & ctx.pLexer->token.type)) {
                    argument += ctx.pLexer->token.value+' ';
                    ctx.pLexer->advance();
                }
                
                if (!argument.empty())
                    ctx.arguments.arguments = {argument};
            }

            if (ctx.pCommand->minArgs > ctx.arguments.arguments.size() || ctx.arguments.arguments.size() > ctx.pCommand->maxArgs) {
                ctx.pCommand->printUsage();
                break;
            }

            ctx.pCommand->callback(ctx);
        }

        case TokenType::EOS:
            break;

        case TokenType::NUMBER:
            break;

        case TokenType::STRING:
            break;

        default:
            break;
        }
    }
}