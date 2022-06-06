#ifndef AP_CA7_BASE_HPP
#define AP_CA7_BASE_HPP

#include "MAPI.hpp"
#include "Song.hpp"
#include "InitialUser.hpp"

using namespace std;

class Base {
private:
    MAPI api;
    const string REQUEST_TYPE_GET = "GET";
    const string REQUEST_TYPE_POST = "POST";
    const string REQUEST_TYPE_DEL = "DELETE";

    const string REQUEST_SIGNUP = "signup";
    const int REQUEST_SIGNUP_PARAMS_COUNT = 6;
    const string REQUEST_LOGIN = "login";
    const int REQUEST_LOGIN_PARAMS_COUNT = 4;
    const string REQUEST_LOGOUT = "logout";
    const int REQUEST_LOGOUT_PARAMS_COUNT = 0;
    const string REQUEST_GET_SONGS = "songs";
    const int REQUEST_GET_SONGS_PARAMS_COUNT = 0;
    const int REQUEST_GET_SONG_PARAMS_COUNT = 2;
    const string REQUEST_LIKES = "likes";
    const int REQUEST_LIKE_PARAMS_COUNT = 2;
    const int REQUEST_GET_LIKES_PARAMS_COUNT = 0;
    const int REQUEST_UNLIKE_PARAMS_COUNT = 2;
    const string REQUEST_PLAYLISTS = "playlists";
    const int REQUEST_ADD_PLAYLIST_PARAMS_COUNT = 4;
    const int REQUEST_GET_PLAYLISTS_PARAMS_COUNT = 2;
    const string REQUEST_PLAYLISTS_SONGS = "playlists_songs";
    const int REQUEST_PLAYLISTS_SONGS_ADD_SONG_PARAMS_COUNT = 4;
    const int REQUEST_PLAYLISTS_SONGS_GET_SONGS_PARAMS_COUNT = 2;
    const int REQUEST_PLAYLISTS_SONGS_DELETE_SONG_PARAMS_COUNT = 4;
    const string REQUEST_USERS = "users";
    const int REQUEST_USERS_GET_USERNAMES_PARAMS_COUNT = 0;
    const string REQUEST_FILTERS = "filters";
    const int REQUEST_FILTERS_ADD_ARTIST_FILTER_PARAMS_COUNT = 2;
    const int REQUEST_FILTERS_ADD_YEAR_FILTER_PARAMS_COUNT = 4;
    const int REQUEST_FILTERS_ADD_LIKES_FILTER_PARAMS_COUNT = 4;
    const int REQUEST_FILTERS_DELETE_FILTERS_PARAMS_COUNT = 0;
    const string REQUEST_COMMENTS = "comments";
    const int REQUEST_COMMENTS_ADD_COMMENT_PARAMS_COUNT = 6;
    const int REQUEST_COMMENTS_GET_COMMENTS_PARAMS_COUNT = 2;
    const string REQUEST_SIMILAR_USERS = "similar_users";
    const int REQUEST_GET_SIMILAR_USERS_PARAMS_COUNT = 2;
    const string REQUEST_RECOMMENDED = "recommended";
    const int REQUEST_GET_RECOMMENDED_PARAMS_COUNT = 2;

    void initializeUsers(vector<InitialUser> initialUsers);

    void printSongs(vector<Song> songs);

    void printPlaylists(vector<Playlist> playlists);

    void printUsernames(const vector<string> &usernames);

    void printComments(vector<Comment> comments);

    void printASingleSong(Song song);

    void printSimilarUsers(vector<pair<Member, double>> usersMapping);

    void printRecommendedSongs(vector<pair<Song, double>> songsMapping);

    string getRequestType(vector<string> splittedPrompt);

    string getRequest(vector<string> splittedPrompt);

    void checkForCommandValidity(vector<string> splittedPrompt);

    void handleGetRequests(vector<string> splittedPrompt);

    void handlePostRequests(vector<string> splittedPrompt);

    void handleDelRequests(vector<string> splittedPrompt);


    void signup(vector<string> params);

    void login(vector<string> params);

    void logout(vector<string> params);

    void getSongs(vector<string> params);

    void likeSong(vector<string> params);

    void getLikes(vector<string> params);

    void unlikeSong(vector<string> params);

    void addPlaylist(vector<string> params);

    void getPlaylists(vector<string> params);

    void addSongToPlaylist(vector<string> params);

    void getPlaylistSongs(vector<string> params);

    void deleteSongFromPlaylist(vector<string> params);

    void getUserNames(vector<string> params);

    void setFilter(vector<string> params);

    void deleteFilters(vector<string> params);

    void addComment(vector<string> params);

    void getComments(vector<string> params);

    void getSimilarUsers(vector<string> params);

    void getRecommendedSongs(vector<string> params);

public:
    Base(vector<Song> songs, vector<InitialUser> initialUsers);

    void processRequest(string requestPrompt);
};


#endif
