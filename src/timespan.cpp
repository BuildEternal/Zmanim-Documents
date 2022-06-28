#include <ctime>

#include <string>
#include <vector>

using namespace std;

class Timespan
{
    time_t start;
    time_t end;

public:
    Timespan(time_t start, time_t end)
    {
        this->start = start;
        this->end = end;
    }

    Timespan(time_t time)
    {
        this->start = time;
        this->end = time;
    }

    time_t getStart() { return start; }

    time_t getEnd() { return end; }
};