
#include "ArtistNameCriterion.hpp"

#include <utility>

ArtistNameCriterion::ArtistNameCriterion(string name) : name(std::move(name)) {}

vector<Song> ArtistNameCriterion::apply(vector<Song> songs) {
    vector<Song> songsOut;
    for (Song song:songs)
        if (song.getArtist() == name)
            songsOut.emplace_back(song);
    return songsOut;
}

int ArtistNameCriterion::getType() {
    return Criterion::CRITERION_TYPE_ARTIST;
}
