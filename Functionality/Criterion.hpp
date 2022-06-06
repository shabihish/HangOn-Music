#ifndef AP_CA7_CRITERION_HPP
#define AP_CA7_CRITERION_HPP

#include <vector>
#include "Song.hpp"

using namespace std;

class Criterion {
public:
    static const int CRITERION_TYPE_ARTIST=0;
    static const int CRITERION_TYPE_YEAR=1;
    static const int CRITERION_TYPE_LIKES=2;
    virtual vector<Song> apply(vector<Song> songs) = 0;
    virtual int getType() = 0;

};


#endif
