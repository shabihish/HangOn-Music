
#ifndef AP_CA7_SONG_HPP
#define AP_CA7_SONG_HPP

#include <string>
#include <vector>
#include "Comment.hpp"

class Song {
private:
    int id;
    int releaseYear;
    int likesCount = 0;
    int numberOfPlaylistsAddedTo = 0;
    std::string title;
    std::string artist;
    std::string link;
    vector<Comment> comments;
public:
    Song(int id, int releaseYear, string title, string artist, string link);

    int getId();

    int getReleaseYear();

    int getLikesCount();

    int getCommentsCount();

    void incrementNumberOfPlaylistsAddedTo();
    void incrementNumberOfLikes();

    int getNumberOfPlaylistsAddedTo();

    std::string getTitle();

    std::string getArtist();

    void addComment(Comment cm);

    vector<Comment> getComments();

};


#endif
