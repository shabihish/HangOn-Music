#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Base.hpp"
#include "Helper.hpp"
#include "../Exceptions/BadRequestException.hpp"
#include "Member.hpp"


Base::Base(vector<Song> songs, vector<InitialUser> initialUsers) : api(songs) {
    initializeUsers(std::move(initialUsers));
}

void Base::processRequest(string requestPrompt) {
    vector<string> splittedPrompt = Helper::split(requestPrompt, " ");
    checkForCommandValidity(splittedPrompt);
    string commandType = getRequestType(splittedPrompt);
    if (commandType == REQUEST_TYPE_GET)
        handleGetRequests(splittedPrompt);
    else if (commandType == REQUEST_TYPE_POST)
        handlePostRequests(splittedPrompt);
    else if (commandType == REQUEST_TYPE_DEL)
        handleDelRequests(splittedPrompt);
    else
        throw BadRequestException();
}

void Base::checkForCommandValidity(vector<string> splittedPrompt) {
    if (splittedPrompt.size() < 2)
        throw BadRequestException();
    string typeSpecifier = getRequestType(splittedPrompt);
    if (typeSpecifier != REQUEST_TYPE_GET &&
        typeSpecifier != REQUEST_TYPE_POST &&
        typeSpecifier != REQUEST_TYPE_DEL)
        throw BadRequestException();
}

string Base::getRequestType(vector<string> splittedPrompt) {
    return splittedPrompt.front();
}

void Base::handleGetRequests(vector<string> splittedPrompt) {
    string request = getRequest(splittedPrompt);
    vector<string> params = Helper::truncateFromTheBeginning(splittedPrompt, 3);
    if (request == REQUEST_GET_SONGS)
        getSongs(params);
    else if (request == REQUEST_LIKES)
        getLikes(params);
    else if (request == REQUEST_PLAYLISTS)
        getPlaylists(params);
    else if (request == REQUEST_PLAYLISTS_SONGS)
        getPlaylistSongs(params);
    else if (request == REQUEST_USERS)
        getUserNames(params);
    else if (request == REQUEST_COMMENTS)
        getComments(params);
    else if (request == REQUEST_SIMILAR_USERS)
        getSimilarUsers(params);
    else if (request == REQUEST_RECOMMENDED)
        getRecommendedSongs(params);
}

void Base::handlePostRequests(vector<string> splittedPrompt) {
    string request = getRequest(splittedPrompt);
    vector<string> params = Helper::truncateFromTheBeginning(splittedPrompt, 3);
    if (request == REQUEST_SIGNUP)
        signup(params);
    else if (request == REQUEST_LOGIN)
        login(params);
    else if (request == REQUEST_LOGOUT)
        logout(params);
    else if (request == REQUEST_LIKES)
        likeSong(params);
    else if (request == REQUEST_PLAYLISTS)
        addPlaylist(params);
    else if (request == REQUEST_PLAYLISTS_SONGS)
        addSongToPlaylist(params);
    else if (request == REQUEST_FILTERS)
        setFilter(params);
    else if (request == REQUEST_COMMENTS)
        addComment(params);
}

void Base::handleDelRequests(vector<string> splittedPrompt) {
    string request = getRequest(splittedPrompt);
    vector<string> params = Helper::truncateFromTheBeginning(splittedPrompt, 3);
    if (request == REQUEST_LIKES)
        unlikeSong(params);
    else if (request == REQUEST_PLAYLISTS_SONGS)
        deleteSongFromPlaylist(params);
    else if (request == REQUEST_FILTERS)
        deleteFilters(params);

}

void Base::signup(vector<string> params) {
    if (params.size() != REQUEST_SIGNUP_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "email" || params[2] != "username" || params[4] != "password")
        throw BadRequestException();
    api.signup(params[1], params[3], params[5]);
    cout << "OK\n";
}

string Base::getRequest(vector<string> splittedPrompt) {
    return splittedPrompt[1];
}

void Base::login(vector<string> params) {
    if (params.size() != REQUEST_LOGIN_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "username" || params[2] != "password")
        throw BadRequestException();
    api.login(params[1], params[3]);
    cout << "OK\n";
}

void Base::logout(vector<string> params) {
    if (params.size() != REQUEST_LOGOUT_PARAMS_COUNT)
        throw BadRequestException();
    api.logout(std::__cxx11::string());
    cout << "OK\n";
}

void Base::getSongs(vector<string> params) {
    if (params.size() == REQUEST_GET_SONGS_PARAMS_COUNT)
        printSongs(api.getSongsList(std::__cxx11::string()));
    else if (params.size() == REQUEST_GET_SONG_PARAMS_COUNT) {
        if (params[0] != "id") {
            throw BadRequestException();
        }
        printASingleSong(api.getSong(stoi(params[1])));
    } else
        throw BadRequestException();
}

void Base::printSongs(vector<Song> songs) {
    if (songs.empty())
        cout << "Empty" << endl;
    sort(songs.begin(), songs.end(), [](Song a, Song b) {
        return a.getId() < b.getId();
    });
    for (Song s:songs)
        cout << s.getId() << " " << s.getTitle() << " "
             << s.getArtist() << " " << s.getReleaseYear() << endl;
}

void Base::printPlaylists(vector<Playlist> playlists) {
    if (playlists.empty())
        cout << "Empty" << endl;
    sort(playlists.begin(), playlists.end(), [](Playlist const &a, Playlist const &b) {
        return a.getId() < b.getId();
    });
    for (Playlist pl:playlists)
        cout << pl.getId() << " " << pl.getName() << " "
             << (pl.isPublic() ? "public" : "private") << endl;
}

void Base::printUsernames(const vector<string> &usernames) {
    if (usernames.empty())
        cout << "Empty" << endl;
    for (const string &str:usernames)
        cout << str << endl;
}

void Base::printComments(vector<Comment> comments) {
    if (comments.empty())
        cout << "Empty" << endl;
    sort(comments.begin(), comments.end(), [](Comment const &a, Comment const &b) -> bool {
        if (a.timeOnSong < b.timeOnSong) return true;
        else if (a.timeOnSong == b.timeOnSong && a.username < b.username)
            return true;
        return false;
    });
    for (Comment cm:comments)
        cout << cm.timeOnSong << " " << cm.username << ": "
             << cm.text << endl;
}


void Base::printASingleSong(Song song) {
    cout << song.getId() << endl;
    cout << song.getTitle() << endl;
    cout << song.getArtist() << endl;
    cout << song.getReleaseYear() << endl;
    cout << "#likes: " << song.getLikesCount() << endl;
    cout << "#comments: " << song.getCommentsCount() << endl;
    cout << "#playlists: " << song.getNumberOfPlaylistsAddedTo() << endl;
}

void Base::printSimilarUsers(vector<pair<Member, double>> usersMapping) {
    cout << setprecision(2) << fixed;
    for (pair<Member, double> itr:usersMapping)
        cout << (itr.second * 100.f) << "% " << itr.first.getUserName() << endl;
}

void Base::printRecommendedSongs(vector<pair<Song, double>> songsMapping) {
    cout << setprecision(2) << fixed;
    for (pair<Song, double> itr:songsMapping)
        cout << itr.first.getId() << " " << (itr.second * 100.f) << "% " << itr.first.getTitle() << " "<<itr.first.getArtist() << " "
             << itr.first.getReleaseYear() << endl;
}


void Base::likeSong(vector<string> params) {
    if (params.size() != REQUEST_LIKE_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "id")
        throw BadRequestException();
    api.likeSong(stoi(params[1]), std::__cxx11::string());
    cout << "OK\n";
}

void Base::getLikes(vector<string> params) {
    if (params.size() != REQUEST_GET_LIKES_PARAMS_COUNT)
        throw BadRequestException();
    vector<Song> songs = api.getLikedSongs(std::__cxx11::string());
    printSongs(songs);
}

void Base::unlikeSong(vector<string> params) {
    if (params.size() != REQUEST_UNLIKE_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "id")
        throw BadRequestException();
    api.deleteFromLikedList(stoi(params[1]), std::__cxx11::string());
    cout << "OK\n";
}

void Base::addPlaylist(vector<string> params) {
    if (params.size() != REQUEST_ADD_PLAYLIST_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "name" || params[2] != "privacy")
        throw BadRequestException();
    cout << api.addPlaylist(params[1], params[3], std::__cxx11::string()) << endl;
}

void Base::getPlaylists(vector<string> params) {
    if (params.size() != REQUEST_GET_PLAYLISTS_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "username")
        throw BadRequestException();
    printPlaylists(api.getMemberPlaylists(params[1], std::__cxx11::string()));
}

void Base::addSongToPlaylist(vector<string> params) {
    if (params.size() != REQUEST_PLAYLISTS_SONGS_ADD_SONG_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "playlist_id" || params[2] != "song_id")
        throw BadRequestException();
    api.addSongToPlaylist(stoi(params[1]), stoi(params[3]), std::__cxx11::string());
    cout << "OK\n";
}

void Base::getPlaylistSongs(vector<string> params) {
    if (params.size() != REQUEST_PLAYLISTS_SONGS_GET_SONGS_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "playlist_id")
        throw BadRequestException();
    printSongs(api.getPlaylistSongs(stoi(params[1]), std::__cxx11::string()));
}

void Base::deleteSongFromPlaylist(vector<string> params) {
    if (params.size() != REQUEST_PLAYLISTS_SONGS_DELETE_SONG_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "playlist_id" || params[2] != "song_id")
        throw BadRequestException();
    api.deletePlaylistSong(stoi(params[1]), stoi(params[3]), std::__cxx11::string());
    cout << "OK\n";
}

void Base::getUserNames(vector<string> params) {
    if (params.size() != REQUEST_USERS_GET_USERNAMES_PARAMS_COUNT)
        throw BadRequestException();
    printUsernames(api.getUserNames(std::__cxx11::string()));
}

void Base::setFilter(vector<string> params) {
    if (params[0] == "artist")
        api.applyArtistFilter(Helper::vectorToEndToString(params, 1), std::__cxx11::string());
    else if (params[0] == "min_year" && params[2] == "max_year") {
        api.applyPublishYearFilter(stoi(params[1]), stoi(params[3]), std::__cxx11::string());
    } else if (params[0] == "min_like" && params[2] == "max_like") {
        api.applyLikesCountFilter(stoi(params[1]), stoi(params[3]), std::__cxx11::string());
    } else
        throw BadRequestException();
    cout << "OK\n";
}

void Base::deleteFilters(vector<string> params) {
    if (params.size() != REQUEST_FILTERS_DELETE_FILTERS_PARAMS_COUNT)
        throw BadRequestException();
    api.deleteAddedFilters(std::__cxx11::string());
    cout << "OK\n";
}

void Base::addComment(vector<string> params) {
    if (params.size() != REQUEST_COMMENTS_ADD_COMMENT_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "song_id" || params[2] != "time" || params[4] != "comment")
        throw BadRequestException();
    api.addComment(stoi(params[1]), stoi(params[3]), params[5], std::__cxx11::string());
    cout << "OK\n";
}

void Base::getComments(vector<string> params) {
    if (params.size() != REQUEST_COMMENTS_GET_COMMENTS_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "song_id")
        throw BadRequestException();
    printComments(api.getComments(stoi(params[1]), std::__cxx11::string()));
}

void Base::getSimilarUsers(vector<string> params) {
    if (params.size() != REQUEST_GET_SIMILAR_USERS_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "count")
        throw BadRequestException();
    printSimilarUsers(api.getSimilarUsers(stoi(params[1]), std::__cxx11::string()));
}

void Base::getRecommendedSongs(vector<string> params) {
    if (params.size() != REQUEST_GET_RECOMMENDED_PARAMS_COUNT)
        throw BadRequestException();
    if (params[0] != "count")
        throw BadRequestException();
    printRecommendedSongs(api.getRecommendedSongs(stoi(params[1]), std::__cxx11::string()));
}

void Base::initializeUsers(vector<InitialUser> initialUsers) {
    string lastUserName;
    for (InitialUser iu:initialUsers) {
        if (!lastUserName.empty() && lastUserName != iu.username) {
            api.logout(std::__cxx11::string());
            lastUserName = "";
        }
        if (lastUserName.empty()) {
            api.signup(iu.email, iu.username, iu.pass);
        }
        api.likeSong(iu.likedSongId, std::__cxx11::string());
        lastUserName = iu.username;
    }
    if (!initialUsers.empty())
        api.logout(std::__cxx11::string());
}