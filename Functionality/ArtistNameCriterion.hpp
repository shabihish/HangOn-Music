#ifndef AP_CA7_ARTISTNAMECRITERION_HPP
#define AP_CA7_ARTISTNAMECRITERION_HPP

#include <string>
#include "Song.hpp"
#include "Criterion.hpp"

using namespace std;

class ArtistNameCriterion : public Criterion {
private:
    const string name;
public:
    ArtistNameCriterion(string name);

    vector<Song> apply(vector<Song> songs) override;

    int getType() override;
};


#endif
