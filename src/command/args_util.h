#pragma once

#include <vector>
#include <string>

std::vector<std::string> argsToArgsVector(int argc, char* argv[]);

std::string pullArg(std::vector<std::string>& args);

std::string peakArg(std::vector<std::string>& args);

std::string argsVectorToString(std::vector<std::string>& args, std::string delim);