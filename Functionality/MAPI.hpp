#ifndef AP_CA7_MAPI_HPP
#define AP_CA7_MAPI_HPP

#include <vector>
#include <string>
#include <map>
#include "Member.hpp"
#include "Song.hpp"
#include "Criterion.hpp"
#include "SID.hpp"

using namespace std;

class MAPI {
private:
    vector<Member> members;
    vector<Song> songs;
    vector<SID> sids;
    map<Playlist *, Member *> playlistsToMember;

    Member *currentlyLoggedInMember = nullptr;
    vector<Criterion *> criteria;

    int lastPlaylistIdAssigned = 0;
    const string PRIVACY_POLICY_TYPE_PRIVATE = "private";
    const string PRIVACY_POLICY_TYPE_PUBLIC = "public";

    bool isEmailAddressAlreadyInUse(const string& emailAddress);

    bool isUserNameAlreadyInUse(const string& userName);

    Member *findMemberByEmailAddress(const string& emailAddress);

    Member *findMemberByUserName(const string& userName);

    void checkUserAuthenticity(const string sid);

    string hashPassword(string originalPassword);

    Song *findSong(int id, string sid);

    pair<Playlist *, Member *> *findPlaylistPair(int id);

    vector<Song> applySongFilters(string sid);

    double getUsersSimilarity(Member m1, Member m2);

    vector<pair<Member *, double>> getCurrentUserSortedSimilarityMapping();

    vector<pair<Song, double>> getCurrentUserSortedSongRecommendationList();

    vector<pair<Song, double>> getCurrentUserSongRecommendationMapping();

    void updateUsersSimilarityMatrices();

    void setCurrentSession(string sessionId);

    static string generateNewSid();
    bool isSidValid(const string& sid);
    void invalidateSid(const string& in);
    Member * getSidAssociatedMember(const string& sid);

public:
    MAPI(vector<Song> songsList);;

    ~MAPI();

    string signup(const string &emailAddress, const string &userName, const string &password);

    string login(const string& username, string password);

    void logout(const string& sid);

    vector<Song> getSongsList(string sid);

    Song getSong(int id);

    void likeSong(int id, string sid);

    vector<Song> getLikedSongs(string sid);

    void deleteFromLikedList(int id, string sid);

    int addPlaylist(string name, const string& privacyPolicy, string sid);

    vector<Playlist> getMemberPlaylists(string userName, string sid);

    void addSongToPlaylist(int playlistId, int songId, string sid);

    vector<Song> getPlaylistSongs(int playlistId, string sid);

    void deletePlaylistSong(int playlistId, int songId, string sid);

    vector<string> getUserNames(string sid);

    void applyArtistFilter(string artistName, string sid);

    void applyPublishYearFilter(int minYear, int maxYear, string sid);

    void applyLikesCountFilter(int minLike, int maxLike, string sid);

    void deleteAddedFilters(string sid);

    void addComment(int songId, int timeToRemarkOn, string text, string sid);

    vector<Comment> getComments(int songId, string sid);

    vector<pair<Member, double>> getSimilarUsers(int count, string sid);

    vector<pair<Song, double>> getRecommendedSongs(int count, string sid);
};

#endif
