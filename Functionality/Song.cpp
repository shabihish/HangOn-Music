
#include "Song.hpp"

#include <utility>

Song::Song(int id, int releaseYear, string title, string artist, string link) {
    this->id = id;
    this->title = std::move(title);
    this->artist = std::move(artist);
    this->link = link;
    this->releaseYear = releaseYear;

    comments.reserve(0);
}

int Song::getId() {
    return id;
}

int Song::getReleaseYear() {
    return releaseYear;
}

int Song::getLikesCount() {
    return likesCount;
}

int Song::getCommentsCount() {
    return comments.size();
}

void Song::addComment(Comment cm) {
    comments.emplace_back(cm);
}

int Song::getNumberOfPlaylistsAddedTo() {
    return numberOfPlaylistsAddedTo;
}

void Song::incrementNumberOfPlaylistsAddedTo() {
    numberOfPlaylistsAddedTo++;
}

std::string Song::getTitle() {
    return title;
}

std::string Song::getArtist() {
    return artist;
}

vector<Comment> Song::getComments() {
    return comments;
}

void Song::incrementNumberOfLikes() {
    likesCount++;
}
