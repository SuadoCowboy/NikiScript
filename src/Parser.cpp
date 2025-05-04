#include "Parser.h"

#include <filesystem>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "PrintCallback.h"

void ns::clearStatementData(Context* pCtx) {
	pCtx->pCommand = nullptr;
	pCtx->args.arguments.clear();
}

bool ns::canRunVariable(Context* pCtx) {
	switch (pCtx->pLexer->token.value[0]) {
	case NS_TOGGLE_ON: {
		ConsoleVariables::pointer pVarPair = &*pCtx->consoleVariables.find(pCtx->pLexer->token.value);
		auto it = std::find(pCtx->toggleVariablesRunning.begin(), pCtx->toggleVariablesRunning.end(), pVarPair);

		if (it == pCtx->toggleVariablesRunning.end()) {
			pCtx->toggleVariablesRunning.push_back(pVarPair);

			return true;
		}

		return false;
	}

	case NS_TOGGLE_OFF: {
		ConsoleVariables::pointer pPlusVariable = nullptr;
		{
			auto it = pCtx->consoleVariables.find('+'+pCtx->pLexer->token.value.substr(1));
			if (it == pCtx->consoleVariables.end())
				return false;

			pPlusVariable = &*it;
		}

		auto it = std::find(pCtx->toggleVariablesRunning.begin(), pCtx->toggleVariablesRunning.end(), pPlusVariable);
		if (it != pCtx->toggleVariablesRunning.end()) {
			pCtx->toggleVariablesRunning.erase(it);

			return true;
		}
		return false;
	}

	case NS_LOOP_VARIABLE: {
		ConsoleVariables::pointer pVar = &*pCtx->consoleVariables.find(pCtx->pLexer->token.value);

		auto it = std::find(pCtx->loopVariablesRunning.begin(), pCtx->loopVariablesRunning.end(), pVar);
		if (it == pCtx->loopVariablesRunning.end())
			pCtx->loopVariablesRunning.push_back(pVar);
		else
			pCtx->loopVariablesRunning.erase(it);
		return false;
	}

	default:
		return true;
	}
}

void ns::handleCommandCall(Context* pCtx, ProgramVariable*& pProgramVar) {
	if (pProgramVar != nullptr) {
		if (pCtx->args.arguments.size() == 0)
			ns::printf(ns::ECHO, "Value: {}\n{}\n", pProgramVar->get(pCtx, pProgramVar), pProgramVar->description);
		else
			pProgramVar->set(pCtx, pProgramVar, pCtx->args.arguments[0]);

		clearStatementData(pCtx);
		pProgramVar = nullptr;
		return;
	}

	if (pCtx->pCommand == nullptr)
		return;

	if (pCtx->pCommand->minArgs > pCtx->args.arguments.size()) {
		if (pCtx->pCommand->minArgs == pCtx->pCommand->maxArgs)
			ns::printf(ns::ERROR, "Expected {} argument(s) but received {} arguments\n", static_cast<uint16_t>(pCtx->pCommand->minArgs), pCtx->args.arguments.size());
		else
			ns::printf(ns::ERROR, "Expected arguments between [{}, {}] but received {} arguments\n", static_cast<uint16_t>(pCtx->pCommand->minArgs), static_cast<uint16_t>(pCtx->pCommand->maxArgs), pCtx->args.arguments.size());

		ns::printf(ns::ECHO, "{} {}\n", pCtx->pCommand->name, pCtx->pCommand->getArgumentsNames());
		clearStatementData(pCtx);
		return;
	}

	switch (pCtx->pCommand->name[0]) {
	case NS_TOGGLE_ON: {
		auto it = std::find(pCtx->toggleCommandsRunning.begin(), pCtx->toggleCommandsRunning.end(), pCtx->pCommand);

		if (it == pCtx->toggleCommandsRunning.end())
			pCtx->toggleCommandsRunning.push_back(pCtx->pCommand);

		else {
			clearStatementData(pCtx);
			return;
		}

		break;
	}
	
	case NS_TOGGLE_OFF: {
		Command* pPlusCommand = pCtx->commands.get('+'+std::string(pCtx->pCommand->name.substr(1)));
		if (pPlusCommand == nullptr)
			break;

		auto it = std::find(pCtx->toggleCommandsRunning.begin(), pCtx->toggleCommandsRunning.end(), pPlusCommand);

		if (it == pCtx->toggleCommandsRunning.end()) {
			clearStatementData(pCtx);
			return;

		} else
			pCtx->toggleCommandsRunning.erase(it);

		break;
	}
	}

	pCtx->pCommand->callback(pCtx);
	clearStatementData(pCtx);
}

uint8_t ns::handleIdentifierToken(Context* pCtx, ProgramVariable*& pProgramVar, bool printError) {
	if (pCtx->pLexer->token.value.empty()) {
		pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return 1;
	}

	if (pCtx->consoleVariables.count(pCtx->pLexer->token.value) != 0) {
		if (canRunVariable(pCtx))
			return 2;

		pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return 0;

	} else if (pCtx->programVariables.count(pCtx->pLexer->token.value) != 0) {
		pProgramVar = &pCtx->programVariables[pCtx->pLexer->token.value];
		return 1;

	} else {
		pCtx->pCommand = pCtx->commands.get(pCtx->pLexer->token.value);

		if (pCtx->pCommand == nullptr) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", pCtx->pLexer->token.value);
			pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return 0;
		} else
			return 1;
	}

	return false;
}

void ns::handleArgumentToken(Context* pCtx, bool printError) {
	insertReferencesInToken(pCtx, pCtx->pLexer->token);

	if (pCtx->pLexer->token.value.empty())
		return;

	if (pCtx->pCommand == nullptr) { // if command is nullptr then just append arguments to a single one. This is useful for ProgramVariable
		if (pCtx->args.arguments.size() == 0)
			pCtx->args.arguments.push_back(pCtx->pLexer->token.value);
		else
			pCtx->args.arguments.back() += ' '+pCtx->pLexer->token.value;
		return;
	}

	if (pCtx->pCommand->maxArgs == 0) {
		if (printError)
			ns::printf(ns::ERROR, "Expected 0 arguments for {} command\n", pCtx->pCommand->name);
		clearStatementData(pCtx);
		pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return;
	}

	if (pCtx->args.arguments.size() == pCtx->pCommand->maxArgs) {
		pCtx->pLexer->token.value = pCtx->args.arguments.back()+' '+pCtx->pLexer->token.value;
		pCtx->args.arguments.pop_back();
	}

	const std::string& arg = pCtx->pCommand->argsDescriptions[pCtx->args.arguments.size()*2];
	switch (arg[0]) {
	case 'i':
		try {
			(void)std::stoll(pCtx->pLexer->token.value);
		} catch (...) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (i)nteger number\n", arg);
			clearStatementData(pCtx);
			pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return;
		}
		break;

	case 'd':
		try {
			(void)std::stold(pCtx->pLexer->token.value);
		} catch (...) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (d)ecimal number\n", arg);
			clearStatementData(pCtx);
			pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return;
		}
		break;

	case 's':
		break;

	case 'v':
		if (pCtx->programVariables.count(pCtx->pLexer->token.value) == 0 && pCtx->consoleVariables.count(pCtx->pLexer->token.value) == 0) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (v)ariable\n", arg);
			clearStatementData(pCtx);
			pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return;
		}
		break;

	default: // should never happen
		break;
	}

	pCtx->args.arguments.push_back(pCtx->pLexer->token.value);
}

void ns::handleConsoleVariableCall(Context* pCtx, ProgramVariable*& pProgramVar, bool printError) {
	Lexer* pOriginalLexer = pCtx->pLexer;

	std::vector<Lexer> tempLexers;
	tempLexers.emplace_back(pCtx->consoleVariables[pCtx->pLexer->token.value]);

	pCtx->pLexer = &tempLexers.back();
	pCtx->pLexer->advance();

	pCtx->origin |= (pCtx->origin & OriginType::VARIABLE)<<1; // if (pCtx->origin & VARIABLE(2)) pCtx->origin |= VARIABLE_IN_VARIABLE(4)
	pCtx->origin |= OriginType::VARIABLE;

	while (tempLexers.size() != 0) {
		switch (pCtx->pLexer->token.type) {
		case TokenType::IDENTIFIER:
			if (handleIdentifierToken(pCtx, pProgramVar, printError) == 2) {
				if (pCtx->maxConsoleVariablesRecursiveDepth != 0 && tempLexers.size() >= pCtx->maxConsoleVariablesRecursiveDepth) {
					pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
					break;
				}

				tempLexers.emplace_back(pCtx->consoleVariables[pCtx->pLexer->token.value]);
				pCtx->pLexer = &tempLexers.back();
				pCtx->origin |= OriginType::VARIABLE_IN_VARIABLE;
			}
			break;

		case TokenType::EOS:
			handleCommandCall(pCtx, pProgramVar);
			break;

		case TokenType::ARGUMENT:
			handleArgumentToken(pCtx, printError);
			break;

		default:
			break;
		}

		pCtx->pLexer->advance();
		while (pCtx->pLexer->token.type == TokenType::END) {
			handleCommandCall(pCtx, pProgramVar);

			tempLexers.pop_back();
			if (tempLexers.size() == 0)
				break;
			else if (tempLexers.size() == 1 && !(pCtx->origin & OriginType::VARIABLE_LOOP))
				pCtx->origin &= ~OriginType::VARIABLE_IN_VARIABLE;

			pCtx->pLexer = &tempLexers.back();
			pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		}
	}

	if (!(pCtx->origin & OriginType::VARIABLE_LOOP))
		pCtx->origin &= ~OriginType::VARIABLE;

	pCtx->pLexer = pOriginalLexer;
}

void ns::updateLoopVariables(Context* pCtx) {
	pCtx->origin |= OriginType::VARIABLE|OriginType::VARIABLE_LOOP;

	pCtx->pLexer->clear();
	for (auto& pVar : pCtx->loopVariablesRunning) {
		pCtx->pLexer->input = pVar->second;
		parse(pCtx);
		pCtx->pLexer->clear();
	}
	
	pCtx->origin &= ~(OriginType::VARIABLE|OriginType::VARIABLE_LOOP);
}

void ns::parse(Context* pCtx, bool printError) {
	if (pCtx->pLexer == nullptr) {
		if (printError)
			ns::print(PrintLevel::ERROR, "tried to parse with a null lexer\n");
		return;
	}

	ProgramVariable* pProgramVar = nullptr;

	pCtx->pLexer->advance();
	while (pCtx->pLexer->token.type != TokenType::END) {
		switch (pCtx->pLexer->token.type) {
		case TokenType::IDENTIFIER: { // can be either variable or command
			uint8_t result = handleIdentifierToken(pCtx, pProgramVar, printError);
			if (result == 2) {
				handleConsoleVariableCall(pCtx, pProgramVar, printError);
				pCtx->pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			} else if (result == 1)
				pCtx->pLexer->advance();
			break;
		}

		case TokenType::ARGUMENT:
			handleArgumentToken(pCtx, printError);
			pCtx->pLexer->advance();
			break;

		case TokenType::EOS:
			handleCommandCall(pCtx, pProgramVar);
			pCtx->pLexer->advance();
			break;

		default:
			pCtx->pLexer->advance();
			break;
		}
	}

	handleCommandCall(pCtx, pProgramVar);
}

bool ns::parseFile(Context* pCtx, const char* _path, bool printError) {
	std::string filePath;
	{
		std::filesystem::path path{_path};
		if (!path.has_extension())
			path += NS_FILE_EXTENSION;

		if (!path.has_root_directory())// || _path.parent_path() != NS_ROOT_DIRECTORY)
			path = pCtx->cfgDirectory / path;

		filePath = path.string();
	}

	std::ifstream file{filePath};
	if (!file) {
		if (printError)
			printf(PrintLevel::ERROR, "Could not load file \"{}\"\n", filePath);
		return false;
	}

	bool runningFromAnotherFile = (pCtx->origin & OriginType::FILE);
	pCtx->origin |= OriginType::FILE;

	std::string originalFilePath = pCtx->filePath;
	pCtx->filePath = filePath;

	std::stringstream script{};
	while (file.good()) {
		std::string line;
		std::getline(file, line);

		if (!line.empty())
			script << line << '\n';
	}

	Arguments originalArguments = pCtx->args;
	pCtx->args.arguments.clear();

	Lexer* pOriginalLexer = pCtx->pLexer;

	Command* pOriginalCommand = pCtx->pCommand;
	pCtx->pCommand = nullptr;

	size_t originalLineCount = pCtx->lineCount;

	Lexer lexer{script.str()};
	pCtx->pLexer = &lexer;
	parse(pCtx);
	pCtx->pLexer = pOriginalLexer;

	pCtx->lineCount = originalLineCount;
	pCtx->pCommand = pOriginalCommand;
	pCtx->args = originalArguments;
	pCtx->filePath = originalFilePath;

	if (!runningFromAnotherFile)
		pCtx->origin &= ~OriginType::FILE;

	return true;
}

void ns::parseInsideAnotherScript(Context* pCtx, const char* input) {
	Arguments originalArgs = pCtx->args;
	Lexer *pOriginalLexer = pCtx->pLexer;
	Command *pOriginalCommand = pCtx->pCommand;
	std::string originalFilePath = pCtx->filePath;
	uint8_t originalOrigin = pCtx->origin;

	Lexer tempLexer{input};
	pCtx->pLexer = &tempLexer;

	clearStatementData(pCtx);

	parse(pCtx);

	pCtx->pLexer = pOriginalLexer;
	pCtx->pCommand = pOriginalCommand;
	pCtx->args = originalArgs;
	pCtx->filePath = originalFilePath;
	pCtx->origin = originalOrigin;
}