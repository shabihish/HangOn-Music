#ifndef AP_CA7_HELPER_HPP
#define AP_CA7_HELPER_HPP

#include <string>
#include <vector>

using namespace std;

class Helper {
public:
    static vector<string> split(const string &str, const string &delimiter);

    static vector<string> truncateFromTheBeginning(vector<string> in, int upperBound);

    static string vectorToEndToString(vector<string> in, int startingIndex);
};


#endif
