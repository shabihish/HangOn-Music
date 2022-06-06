#include "Member.hpp"
#include "../Exceptions/BadRequestException.hpp"
#include "../Exceptions/EmptyException.hpp"

void Member::addToLikedSongs(Song song) {
    if (hasLikedSong(song.getId()))
        throw BadRequestException();
    likedPodcasts.emplace_back(song);
}

void Member::deleteFromLikedSongs(int id) {
    if (!hasLikedSong(id))
        throw BadRequestException();
    for (auto it = likedPodcasts.begin(); it != likedPodcasts.end(); it++) {
        if (it->getId() == id) {
            likedPodcasts.erase(it);
            break;
        }
    }
}

bool Member::hasLikedSong(int id) {
    for (Song song:likedPodcasts)
        if (song.getId() == id)
            return true;
    return false;
}

vector<Song> Member::getLikedSongs() {
    return likedPodcasts;
}

void Member::addToPlaylists(Playlist *playlist) {
    playlists.emplace_back(playlist);
}


vector<Playlist *> Member::getAllPlaylists() {
    return playlists;
}

vector<Playlist *> Member::getPublicPlaylists() {
    vector<Playlist *> out;
    for (Playlist *playlist:playlists)
        if (playlist->isPublic())
            out.emplace_back(playlist);
    return out;
}

Member::Member(string emailAddress, string userName, string password) {
    this->emailAddress = emailAddress;
    this->userName = userName;
    this->password = password;
}

void Member::changePassword(string password) {
    this->password = password;
}

string Member::getEmailAddress() {
    return emailAddress;
}

string Member::getUserName() {
    return userName;
}

bool Member::passwordMatches(string passwordToCheckFor) {
    return this->password == passwordToCheckFor;
}

void Member::setOtherMembersToSmilarityMapping(vector<pair<Member *, double>> otherMembersToSimilarity) {
    this->otherMembersToSimilarity = std::move(otherMembersToSimilarity);
}

vector<pair<Member *, double>> Member::getOtherMembersToSmilarityMapping() {
    return otherMembersToSimilarity;
}

void Member::setSessionId(string session) {
    sessionId=session;
}

string Member::getSessionId() {
    return sessionId;
}


