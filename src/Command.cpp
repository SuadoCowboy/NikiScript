#include "Command.h"

#include <sstream>

#include "PrintCallback.h"

sci::Command::Command() {}

sci::Command::Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs,
    CommandCallback callback, const std::string_view& description, const std::vector<std::string_view>& argsDescriptions)
  : name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), description(description), argsDescriptions(argsDescriptions) {}

std::string sci::Command::getArgumentsNames() {
    std::stringstream oss;
    bool isName = true;

    for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
        if (isName)
            oss << argsDescriptions[i] << ' ';

        isName = !isName;
    }

    std::string out = oss.str();
    out.erase(out.size()-1);

    return out;
}

std::string sci::Command::getUsage() {
    std::stringstream usageOss;
    
    usageOss << name << ' ' << getArgumentsNames() << '\n';
    
    std::string usage = usageOss.str();
    usage.erase(usage.size()-1);

    return usage;
}

void sci::Command::printAsDataTree() {
    std::stringstream descriptions;
    std::stringstream usage;
    bool isName = true;

    usage << "- Usage: " << name << ' ';
    descriptions << "- Description: " << description << "\n- Arguments:\n";
    for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
        if (isName) {
            descriptions << "- - " << argsDescriptions[i] << ": ";
            usage << argsDescriptions[i] << ' ';

        } else
            descriptions << argsDescriptions[i] << '\n';

        isName = !isName;
    }
    usage << '\n' << descriptions.str();

    sci::print(PrintLevel::ECHO, usage.str());
}