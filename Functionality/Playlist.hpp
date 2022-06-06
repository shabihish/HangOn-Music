#ifndef AP_CA7_PLAYLIST_HPP
#define AP_CA7_PLAYLIST_HPP


#include <string>
#include <vector>
#include "Song.hpp"

using namespace std;

class Playlist {
private:
     int id;
     string name;
     int privacyPolicy;

    vector<Song*> songs;

    bool hasSong(int songId);
public:
    static const int TYPE_PRIVATE = 20;
    static const int TYPE_PUBLIC = 10;

    Playlist(string name, int privacyPolicy, int id);

    int getId() const;

    string getName();

    bool isPublic();

    void addSong(Song *song);


    vector<Song> getSongs();

    void deleteSong(int songId);
};


#endif
