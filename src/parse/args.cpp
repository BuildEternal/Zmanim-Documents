#include <vector>
#include <string>

#include "args.h"

std::vector<std::string> argsToArgsVector(int argc, char* argv[]) {
    std::vector<std::string> args;
    if (argc > 1) args.assign(argv + 1, argv + argc);
    return args;
}

std::string pullArg(std::vector<std::string>& args) {
    if (args.size() > 0) {
        auto arg = args[0];
        args.erase(args.begin());
        return arg;
    }

    return "";
}

std::string peakArg(std::vector<std::string>& args) {
    if (args.size() > 0) {
        auto arg = args[0];
        return arg;
    }

    return "";
}

std::string argsVectorToString(std::vector<std::string>& args, std::string delim) {
    std::string str;

    for (auto it = args.begin(); it != args.end(); it++) {
        str += *it;
        if (it != args.end() - 1) str += delim;
    }

    return str;
}