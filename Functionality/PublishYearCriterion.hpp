#ifndef AP_CA7_PUBLISHYEARCRITERION_HPP
#define AP_CA7_PUBLISHYEARCRITERION_HPP

#include <string>
#include "Song.hpp"
#include "Criterion.hpp"

using namespace std;

class PublishYearCriterion : public Criterion {
private:
    const int rangeStartingYear;
    const int rangeEndingYear;
public:
    PublishYearCriterion(int rangeStartingYear, int rangeEndingYear);

    vector<Song> apply(vector<Song> songs) override;

    int getType() override;
};


#endif
