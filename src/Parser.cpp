#include "Parser.h"

#include "PrintCallback.h"

uint64_t sci::maxConsoleVariableCalls = 10000;

void sci::clearStatementData(SweatContext& ctx) {
	ctx.pCommand = nullptr;
	ctx.pData = nullptr;
	ctx.arguments.clear();
}

void sci::handleCommandCall(SweatContext& ctx) {
	if (ctx.pCommand == nullptr)
		return;

	if (ctx.pCommand->minArgs > ctx.arguments.arguments.size()) {
		if (ctx.pCommand->minArgs == ctx.pCommand->maxArgs)
			sci::printf(sci::PrintLevel::ERROR, "Expected {} argument(s) but received {} arguments\n", static_cast<uint16_t>(ctx.pCommand->minArgs), ctx.arguments.arguments.size());
		else
			sci::printf(sci::PrintLevel::ERROR, "Expected arguments between [{}, {}] but received {} arguments\n", static_cast<uint16_t>(ctx.pCommand->minArgs), static_cast<uint16_t>(ctx.pCommand->maxArgs), ctx.arguments.arguments.size());

		sci::printf(sci::PrintLevel::ECHO, "{} {}\n", ctx.pCommand->name, ctx.pCommand->getArgumentsNames());
		return;
	}

	ctx.pCommand->callback(ctx);
	clearStatementData(ctx);
}

void sci::handleArgumentToken(SweatContext& ctx) {
	insertReferencesInToken(ctx, ctx.pLexer->token);

	if (ctx.pCommand->maxArgs == 0) {
		sci::printf(sci::PrintLevel::ERROR, "Expected 0 arguments for {} command\n", ctx.pCommand->name);
		clearStatementData(ctx);
		ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return;
	}

	if (ctx.arguments.arguments.size() == ctx.pCommand->maxArgs) {
		ctx.pLexer->token.value = ctx.arguments.arguments.back()+' '+ctx.pLexer->token.value;
		ctx.arguments.arguments.pop_back();
	}

	const std::string_view& arg = ctx.pCommand->argsDescriptions[ctx.arguments.arguments.size()*2];
	switch (arg[0]) {
	case 'i':
		try {
			std::stoi(ctx.pLexer->token.value);
		} catch (...) {
			sci::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (i)nteger number\n", arg);
			clearStatementData(ctx);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		}
		break;

	case 'd':
		try {
			std::stof(ctx.pLexer->token.value);
		} catch (...) {
			sci::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (d)ecimal number\n", arg);
			clearStatementData(ctx);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		}
		break;

	case 's':
		break;

	case 'v':
		if (ctx.programVariables.count(ctx.pLexer->token.value) == 0 && ctx.consoleVariables.count(ctx.pLexer->token.value) == 0) {
			sci::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (v)ariable\n", arg);
			clearStatementData(ctx);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		}
		break;

	default: // should never happen
		break;
	}

	ctx.arguments.arguments.push_back(ctx.pLexer->token.value);
}

void sci::handleConsoleVariableCall(SweatContext& ctx) {
	Lexer* pOriginalLexer = ctx.pLexer;

	std::vector<Lexer> tempLexers;
	tempLexers.emplace_back(ctx.consoleVariables[ctx.pLexer->token.value]);
	
	ctx.pLexer = &tempLexers.back();
	ctx.pLexer->advance();
	
	//ctx.runningFrom |= VARIABLE;

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

		case TokenType::ARGUMENT:
			handleArgumentToken(ctx);
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

	//ctx.runningFrom &= ~VARIABLE;
	ctx.pLexer = pOriginalLexer;
}

void sci::updateLoopVariables(sci::SweatContext& ctx) {
	for (auto& pVar : ctx.loopVariablesRunning) {
		ctx.pLexer->clear();
		ctx.pLexer->input = pVar->second;
		parse(ctx);
	}
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
				ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
				break;

			} else if (ctx.programVariables.count(ctx.pLexer->token.value) != 0) {
				ctx.pCommand = ctx.commands.get("_program_variable_callback");
				if (ctx.pCommand != nullptr)
					ctx.pData = &ctx.programVariables[ctx.pLexer->token.value];

				ctx.pLexer->advance();
				break;
			}

			ctx.pCommand = ctx.commands.get(ctx.pLexer->token.value);
			if (ctx.pCommand == nullptr) {
				sci::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", ctx.pLexer->token.value);
				ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			} else
				ctx.pLexer->advance();

			break;

		case TokenType::ARGUMENT:
			handleArgumentToken(ctx);
			ctx.pLexer->advance();
			break;

		case TokenType::EOS:
			handleCommandCall(ctx);
			ctx.pLexer->advance();
			break;

		default:
			ctx.pLexer->advance();
			break;
		}
	}

	handleCommandCall(ctx);
}