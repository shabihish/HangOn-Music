
#include "Helper.hpp"
#include <cmath>

vector<string> Helper::split(const string &str, const string &delim) {
    vector<string> tokens;
    int prevPos = 0, pos;
    do {
        pos = str.find(delim, prevPos);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prevPos, pos - prevPos);
        if (!token.empty()) tokens.push_back(token);
        prevPos = pos + delim.length();
    } while (pos < str.length() && prevPos < str.length());
    return tokens;
}

vector<string> Helper::truncateFromTheBeginning(vector<string> in, int upperBound) {
    vector<string> out;
    for (int i = upperBound; i < in.size(); i++)
        out.emplace_back(in[i]);
    return out;
}

string Helper::vectorToEndToString(vector<string> in, int startingIndex) {
    string out = "";
    for (int i = startingIndex; i < in.size(); i++) {
        out.append(in[i]);
        if (i < in.size() - 1)
            out.append(" ");
    }
    return out;
}
