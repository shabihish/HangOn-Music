
#include "PublishYearCriterion.hpp"

PublishYearCriterion::PublishYearCriterion(int rangeStartingYear, int rangeEndingYear) : rangeStartingYear(
        rangeStartingYear), rangeEndingYear(rangeEndingYear) {}


vector<Song> PublishYearCriterion::apply(vector<Song> songs) {
    vector<Song> songsOut;
    for (Song song:songs)
        if (song.getReleaseYear() <= rangeEndingYear && song.getReleaseYear() >= rangeStartingYear)
            songsOut.emplace_back(song);
    return songsOut;
}

int PublishYearCriterion::getType() {
    return Criterion::CRITERION_TYPE_YEAR;
}
