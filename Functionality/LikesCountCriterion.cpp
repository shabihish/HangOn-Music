
#include "LikesCountCriterion.hpp"

LikesCountCriterion::LikesCountCriterion(int rangeStart, int rangeEnd) : rangeStart(rangeStart), rangeEnd(rangeEnd) {}

vector<Song> LikesCountCriterion::apply(vector<Song> songs) {
    vector<Song> songsOut;
    for (Song song:songs)
        if (song.getLikesCount() <= rangeEnd && song.getLikesCount() >= rangeStart)
            songsOut.emplace_back(song);
    return songsOut;
}

int LikesCountCriterion::getType() {
    return Criterion::CRITERION_TYPE_LIKES;
}
