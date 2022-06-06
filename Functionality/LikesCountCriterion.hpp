
#ifndef AP_CA7_LIKESCOUNTCRITERION_HPP
#define AP_CA7_LIKESCOUNTCRITERION_HPP

#include <string>
#include "Song.hpp"
#include "Criterion.hpp"

using namespace std;

class LikesCountCriterion : public Criterion {
private:
    const int rangeStart;
    const int rangeEnd;
public:
    LikesCountCriterion(int rangeStart, int rangeEnd);

    vector<Song> apply(vector<Song> songs) override;

    int getType() override;
};


#endif
