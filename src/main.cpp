#include <iostream>
#include <vector>

#include "parse/parse_date.h"
#include "invalid_date_exception.h"

using namespace std;

int main(int argc, char* argv[])
{
    vector<string> args;

    if (argc > 1) {
        args.assign(argv + 1, argv + argc);
    }

    try {
        tm date = parseDate(args);

        int month = date.tm_mon + 1;
        int day = date.tm_mday;
        int year = date.tm_year + 1900;

        cout << month << '/' << day << '/' << year << " is the date you have inputted. It is valid.";
    }
    catch (exception& e) {
        cout << e.what();
    }


    // cout << time(NULL);

    return 0;
}