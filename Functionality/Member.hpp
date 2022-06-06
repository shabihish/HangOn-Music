#ifndef AP_CA7_MEMBER_HPP
#define AP_CA7_MEMBER_HPP

#include <string>
#include <vector>
#include <vector>
#include <map>
#include "Song.hpp"
#include "Playlist.hpp"

using namespace std;

class Member {
private:
    string emailAddress;
    string userName;
    string password;

    string sessionId;

    vector<Song> likedPodcasts;

    vector<Playlist *> playlists;

    vector<pair<Member *, double>> otherMembersToSimilarity;

public:
    Member(string emailAddress, string userName, string password);

    void changePassword(string password);

    string getEmailAddress();

    string getUserName();

    bool passwordMatches(string passwordToCheckFor);

    void addToLikedSongs(Song song);

    void deleteFromLikedSongs(int id);

    vector<Song> getLikedSongs();

    bool hasLikedSong(int id);


    void addToPlaylists(Playlist *playlist);

    vector<Playlist *> getAllPlaylists();

    vector<Playlist *> getPublicPlaylists();

    void setOtherMembersToSmilarityMapping(vector<pair<Member *, double>> otherMembersToSimilarity);

    vector<pair<Member *, double>> getOtherMembersToSmilarityMapping();

    void setSessionId(string session);
    string getSessionId();
};


#endif
