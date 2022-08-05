#include <iostream>
#include <stdexcept>
#include <vector>

#include "command/args_util.h"
#include "command/parse_command.h"

int main(int argc, char* argv[])
{
    std::vector<std::string> args = argsToArgsVector(argc, argv);

    try { parseCommand(args); }
    catch (const std::invalid_argument& e) {
        std::cout << "Something went wrong:\n" << e.what() << "\nTry 'get help' for help.\n";
    }
    catch (const std::exception& e) {
        std::cout << "Something went wrong:\n" << e.what();
    }

    return 0;
}