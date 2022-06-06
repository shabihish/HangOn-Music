#include "Playlist.hpp"

#include <utility>
#include "../Exceptions/BadRequestException.hpp"

Playlist::Playlist(string name, int privacyPolicy, int id) : name(std::move(name)), privacyPolicy(privacyPolicy),
                                                             id(id) {
    if (privacyPolicy != TYPE_PUBLIC && privacyPolicy != TYPE_PRIVATE)
        throw BadRequestException();
}

void Playlist::deleteSong(int songId) {
    if (!hasSong(songId))
        throw BadRequestException();
    for (int i = 0; i < songs.size(); i++) {
        if (songs[i]->getId() == songId) {
            songs.erase(songs.begin() + i);
            break;
        }
    }
}

bool Playlist::hasSong(int songId) {
    for (Song *song:songs)
        if (song->getId() == songId)
            return true;
    return false;
}

int Playlist::getId() const {
    return id;
}

string Playlist::getName() {
    return name;
}

bool Playlist::isPublic() {
    return privacyPolicy == TYPE_PUBLIC;
}

void Playlist::addSong(Song *song) {
    songs.emplace_back(song);
}

vector<Song> Playlist::getSongs() {
    vector<Song> out;
    out.reserve(songs.size());
    for (auto &song : songs) {
        out.emplace_back(*song);
    }
    return out;
}
