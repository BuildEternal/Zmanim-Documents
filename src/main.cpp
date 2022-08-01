#include <iostream>
#include <vector>

#include "parse/args.h"
#include "parse/parse_command.h"

int main(int argc, char* argv[])
{
    std::vector<std::string> args = argsToArgsVector(argc, argv);

    try {

        parseCommand(args);
    }
    catch (std::invalid_argument& e) {
        std::cout << "Something was wrong with your command:\n" << e.what();
    }
    catch (std::exception& e) {
        std::cout << "Something went wrong:\n" << e.what();
    }

    return 0;
}