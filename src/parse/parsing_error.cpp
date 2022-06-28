#include <iostream>

using namespace std;

class ParsingError : public runtime_error
{
    string msg;

public:
    explicit ParsingError(string what, string format)
        : runtime_error("A " + what + " is supposed to be formatted as " + format) {}
};