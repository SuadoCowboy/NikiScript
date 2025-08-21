#include <iostream>
#include <string>
#include <stdint.h>
#include <unordered_map>

#include <NikiScript/NikiScript.h>
#include <NikiScript/Parser.h>

struct Job {
	ns::Lexer lexer;
	ns::CommandContext ctx;
	ns::ProgramVariable* pProgramVar = nullptr;
};

void nikiScriptPrintCallback(void*, ns::PrintLevel level, const char* msg) {
	std::cout << ns::levelToString(level) << ": " << msg;
}

bool running = false;
static void quit_command(ns::CommandContext*, void*) {
	running = false;
}

std::unordered_map<ns::CommandContext*, uint32_t> waitAmounts = {};
static void wait_command(ns::CommandContext* pCtx, void*) {
	if (pCtx->args.arguments.size() == 0)
		waitAmounts[pCtx] = 1;
	else {
		uint16_t cycles = pCtx->args.getUnsigned<uint16_t>(0);
		if (cycles < 1)
			cycles = 1;

		waitAmounts[pCtx] = cycles;
	}
}

int main() {
	ns::setPrintCallback(nullptr, nikiScriptPrintCallback);

	ns::Context ctx;
	char _cfgDirectory[] = "data/cfg/";
	ctx.cfgDirectory = _cfgDirectory;

	ns::registerCommands(&ctx);

	nsRegisterCommand(&ctx, "quit", 0,1, quit_command, "stops the main loop from running", "s[?]", "");
	nsRegisterCommand(&ctx, "wait", 0,1, wait_command, "wait n cycles before continuing the execution", "i[cycles?]", "how many cycles to wait");

	std::vector<Job*> jobs{};
	std::string input;
	
	running = true;
	while (running) {
		input.clear();

		std::cout << "> ";
		std::getline(std::cin, input);

		jobs.push_back(new Job());
		jobs.back()->lexer.input = input;
		jobs.back()->ctx.pCtx = &ctx;
		jobs.back()->ctx.pLexer = &jobs.back()->lexer;
		jobs.back()->lexer.advance();

		for (size_t i = 0; i < jobs.size();) {
			if (jobs[i]->lexer.token.type == ns::TokenType::END) {
				delete jobs[i];
				jobs.erase(jobs.begin()+i);
				continue;
			}

			while (waitAmounts.count(&jobs[i]->ctx) == 0 && jobs[i]->lexer.token.type != ns::TokenType::END)
				ns::parseUntilEOS(&jobs[i]->ctx, jobs[i]->pProgramVar);

			if (waitAmounts.count(&jobs[i]->ctx) != 0) {
				--waitAmounts[&jobs[i]->ctx];
				if (waitAmounts[&jobs[i]->ctx] == 0)
					waitAmounts.erase(&jobs[i]->ctx);
			}

			++i;
		}

		ns::updateLoopVariables(&ctx);
	}
}