#include <algorithm>
#include "MAPI.hpp"
#include "../Exceptions/BadRequestException.hpp"
#include "../Exceptions/UserUnauthenticatedException.hpp"
#include "../Exceptions/ForbiddenException.hpp"
#include "../Exceptions/NotFoundException.hpp"
#include "ArtistNameCriterion.hpp"
#include "PublishYearCriterion.hpp"
#include "LikesCountCriterion.hpp"
#include "HashingFunctions/SHA1.hpp"
#include <ctime>
#include <random>
#include <utility>


MAPI::MAPI(vector<Song> songsList) {
    songs = std::move(songsList);
    srand(time(nullptr));
}

string MAPI::signup(const string &emailAddress, const string &userName, const string &password) {
    if (isEmailAddressAlreadyInUse(emailAddress) || isUserNameAlreadyInUse(userName))
        throw BadRequestException();
    members.emplace_back(emailAddress, userName, hashPassword(password));
    string sid = login(emailAddress, password);
    updateUsersSimilarityMatrices();
    return sid;
}

string MAPI::login(const string &username, string password) {
    if (!isUserNameAlreadyInUse(username))
        throw BadRequestException();
    if (!findMemberByUserName(username)->
            passwordMatches(hashPassword(std::move(password))))
        throw BadRequestException();
    currentlyLoggedInMember = findMemberByUserName(username);
    string newSid = generateNewSid();
    sids.emplace_back(SID(currentlyLoggedInMember, newSid));
    return newSid;
}

Member *MAPI::findMemberByEmailAddress(const string &emailAddress) {
    for (auto &member : members)
        if (member.getEmailAddress() == emailAddress)
            return &member;
    throw NotFoundException();
}

Member *MAPI::findMemberByUserName(const string &userName) {
    for (auto &member : members)
        if (member.getUserName() == userName)
            return &member;
    throw NotFoundException();
}


pair<Playlist *, Member *> *MAPI::findPlaylistPair(int id) {
    for (auto &pair : playlistsToMember)
        if (pair.first->getId() == id)
            return (std::pair<Playlist *, Member *> *) &pair;
    return nullptr;
}

bool MAPI::isEmailAddressAlreadyInUse(const string &emailAddress) {
    for (auto &member : members)
        if (member.getEmailAddress() == emailAddress)
            return true;
    return false;
}

bool MAPI::isUserNameAlreadyInUse(const string &userName) {
    for (auto &member : members)
        if (member.getUserName() == userName)
            return true;
    return false;
}

void MAPI::checkUserAuthenticity(const string sid) {
    if (isSidValid(sid)) {
        currentlyLoggedInMember = getSidAssociatedMember(sid);
    } else
        throw UserUnauthenticatedException();
}

string MAPI::hashPassword(string originalPassword) {
    return sha1(originalPassword);
}

void MAPI::logout(const string &sid) {
    checkUserAuthenticity(sid);
    invalidateSid(sid);
    currentlyLoggedInMember = nullptr;
}

vector<Song> MAPI::getSongsList(string sid) {
    checkUserAuthenticity(sid);
    vector<Song> out = applySongFilters(std::__cxx11::string());
    return out;
}

Song *MAPI::findSong(int id, string sid) {
    checkUserAuthenticity(std::move(sid));
    for (auto &song : songs)
        if (song.getId() == id)
            return &song;
    throw NotFoundException();
}

Song MAPI::getSong(int id) {
    return *findSong(id, std::__cxx11::string());
}

void MAPI::likeSong(int id, string sid) {
    checkUserAuthenticity(std::move(sid));
    Song *song = findSong(id, std::__cxx11::string());
    song->incrementNumberOfLikes();
    currentlyLoggedInMember->addToLikedSongs(*song);
    updateUsersSimilarityMatrices();
}

vector<Song> MAPI::getLikedSongs(string sid) {
    checkUserAuthenticity(sid);
    return currentlyLoggedInMember->getLikedSongs();
}

void MAPI::deleteFromLikedList(int id, string sid) {
    checkUserAuthenticity(std::move(sid));
    currentlyLoggedInMember->deleteFromLikedSongs(id);
}

int MAPI::addPlaylist(string name, const string &privacyPolicy, string sid) {
    checkUserAuthenticity(std::move(sid));
    if (privacyPolicy != PRIVACY_POLICY_TYPE_PRIVATE && privacyPolicy != PRIVACY_POLICY_TYPE_PUBLIC)
        throw BadRequestException();
    int playlistType =
            privacyPolicy == PRIVACY_POLICY_TYPE_PRIVATE ? Playlist::TYPE_PRIVATE : Playlist::TYPE_PUBLIC;
    currentlyLoggedInMember->addToPlaylists(new Playlist(name, playlistType, ++lastPlaylistIdAssigned));
    playlistsToMember.insert(make_pair(currentlyLoggedInMember->getAllPlaylists().back(), currentlyLoggedInMember));
    return lastPlaylistIdAssigned;
}

vector<Playlist> MAPI::getMemberPlaylists(string userName, string sid) {
    checkUserAuthenticity(std::move(sid));
    Member *targetMember = findMemberByUserName(userName);
    if (targetMember == nullptr)
        throw NotFoundException();
    vector<Playlist *> targetPlaylists;
    if (targetMember == currentlyLoggedInMember)
        targetPlaylists = targetMember->getAllPlaylists();
    else
        targetPlaylists = targetMember->getPublicPlaylists();
    vector<Playlist> out;
    out.reserve(targetPlaylists.size());
    for (Playlist *playlist:targetPlaylists)
        out.emplace_back(*playlist);
    return out;
}

void MAPI::addSongToPlaylist(int playlistId, int songId, string sid) {
    checkUserAuthenticity(std::move(sid));
    auto pmPair = findPlaylistPair(playlistId);
    auto targetSong = findSong(songId, std::__cxx11::string());
    if (pmPair == nullptr || targetSong == nullptr)
        throw NotFoundException();
    if (pmPair->second != currentlyLoggedInMember)
        throw ForbiddenException();
    pmPair->first->addSong(targetSong);
    targetSong->incrementNumberOfPlaylistsAddedTo();
}

vector<Song> MAPI::getPlaylistSongs(int playlistId, string sid) {
    checkUserAuthenticity(std::move(sid));
    auto pmPair = findPlaylistPair(playlistId);
    if (pmPair == nullptr)
        throw NotFoundException();
    if (pmPair->second != currentlyLoggedInMember && !pmPair->first->isPublic())
        throw ForbiddenException();
    return pmPair->first->getSongs();
}

void MAPI::deletePlaylistSong(int playlistId, int songId, string sid) {
    checkUserAuthenticity(std::move(sid));
    auto pmPair = findPlaylistPair(playlistId);
    auto targetSong = findSong(songId, std::__cxx11::string());
    if (pmPair == nullptr)
        throw NotFoundException();
    if (targetSong == nullptr)
        throw BadRequestException();
    if (pmPair->second != currentlyLoggedInMember)
        throw ForbiddenException();
    pmPair->first->deleteSong(targetSong->getId());
}

vector<string> MAPI::getUserNames(string sid) {
    checkUserAuthenticity(std::move(sid));
    vector<string> userNames;
    userNames.reserve(members.size());
    for (auto &member : members) {
        if (member.getUserName() == currentlyLoggedInMember->getUserName())
            continue;
        userNames.emplace_back(member.getUserName());
    }
    return userNames;
}

void MAPI::applyArtistFilter(string artistName, string sid) {
    checkUserAuthenticity(std::move(sid));
    remove_if(criteria.begin(), criteria.end(),
              [](Criterion *criterion) {
                  return criterion->getType() == Criterion::CRITERION_TYPE_ARTIST;
              });
    criteria.emplace_back(new ArtistNameCriterion(artistName));
}

void MAPI::applyPublishYearFilter(int minYear, int maxYear, string sid) {
    checkUserAuthenticity(std::move(sid));
    remove_if(criteria.begin(), criteria.end(),
              [](Criterion *criterion) {
                  return criterion->getType() == Criterion::CRITERION_TYPE_YEAR;
              });
    criteria.emplace_back(new PublishYearCriterion(minYear, maxYear));
}

void MAPI::applyLikesCountFilter(int minLike, int maxLike, string sid) {
    checkUserAuthenticity(std::move(sid));
    remove_if(criteria.begin(), criteria.end(),
              [](Criterion *criterion) {
                  return criterion->getType() == Criterion::CRITERION_TYPE_LIKES;
              });
    criteria.emplace_back(new LikesCountCriterion(minLike, maxLike));
}

vector<Song> MAPI::applySongFilters(string sid) {
    checkUserAuthenticity(std::move(sid));
    vector<Song> out = songs;
    for (Criterion *cr:criteria)
        out = cr->apply(out);
    return out;
}

void MAPI::deleteAddedFilters(string sid) {
    checkUserAuthenticity(std::move(sid));
    for (auto &cr : criteria)
        delete cr;
    criteria.clear();
}

void MAPI::addComment(int songId, int timeToRemarkOn, string text, string sid) {
    checkUserAuthenticity(std::move(sid));
    Song *targetSong = findSong(songId, std::__cxx11::string());
    targetSong->addComment(Comment{std::move(text), timeToRemarkOn, currentlyLoggedInMember->getUserName()});
}

vector<Comment> MAPI::getComments(int songId, string sid) {
    checkUserAuthenticity(std::move(sid));
    auto targetSong = findSong(songId, std::__cxx11::string());
    if (targetSong == nullptr)
        throw NotFoundException();
    return targetSong->getComments();
}

MAPI::~MAPI() {
    for (auto &cr : criteria)
        delete cr;
    for (auto &pair : playlistsToMember)
        delete pair.first;
}

void MAPI::updateUsersSimilarityMatrices() {
    for (size_t i = 0; i < members.size(); i++) {
        vector<pair<Member *, double>> otherMembersToSimilarity;
        for (int j = 0; j < members.size(); j++) {
            if (i == j)
                continue;
            double simPercentage = getUsersSimilarity(members[i], members[j]);
            otherMembersToSimilarity.emplace_back(make_pair(&members[j], simPercentage));
        }
        members[i].setOtherMembersToSmilarityMapping(otherMembersToSimilarity);
    }
}

double MAPI::getUsersSimilarity(Member m1, Member m2) {
    vector<Song> user1Songs = m1.getLikedSongs();
    int numberOfCommonSongs = 0;
    for (Song s:user1Songs) {
        if (m2.hasLikedSong(s.getId()))
            numberOfCommonSongs++;
    }
    return ((double) numberOfCommonSongs / songs.size());
}

vector<pair<Member, double>> MAPI::getSimilarUsers(int count, string sid) {
    checkUserAuthenticity(std::move(sid));
    vector<pair<Member, double>> out;
    vector<pair<Member *, double>> mapping = getCurrentUserSortedSimilarityMapping();
    out.reserve(min(count, (int) mapping.size()));
    for (size_t i = 0; i < mapping.size(); i++) {
        if (i >= count)
            break;
        out.emplace_back(make_pair(*mapping[i].first, mapping[i].second));
    }
    return out;
}

vector<pair<Song, double>> MAPI::getRecommendedSongs(int count, string sid) {
    checkUserAuthenticity(std::move(sid));
    vector<pair<Song, double>> songsMapping = getCurrentUserSortedSongRecommendationList();
    vector<pair<Song, double>> out;
    out.reserve(min(count, (int) songsMapping.size()));
    for (size_t i = 0; i < count && i < songsMapping.size(); i++)
        out.emplace_back(make_pair(songsMapping[i].first, songsMapping[i].second));
    return out;
}

vector<pair<Member *, double>> MAPI::getCurrentUserSortedSimilarityMapping() {
    vector<pair<Member *, double>> mapping = currentlyLoggedInMember->getOtherMembersToSmilarityMapping();
    sort(mapping.begin(), mapping.end(), [](pair<Member *, double> const &p1,
                                            pair<Member *, double> const &p2) {
        if (p1.second > p2.second)
            return true;
        else if (p1.second == p2.second && p1.first->getUserName() < p2.first->getUserName())
            return true;
        return false;
    });
    return mapping;
}

vector<pair<Song, double>> MAPI::getCurrentUserSortedSongRecommendationList() {
    vector<pair<Song, double>> songsMapping = getCurrentUserSongRecommendationMapping();
    sort(songsMapping.begin(), songsMapping.end(), [](pair<Song, double> p1,
                                                      pair<Song, double> p2) -> bool {
        if (p1.second > p2.second)
            return true;
        else if (p1.second == p2.second && p1.first.getId() < p2.first.getId())
            return true;
        return false;
    });
    return songsMapping;
}

vector<pair<Song, double>> MAPI::getCurrentUserSongRecommendationMapping() {
    vector<pair<Member *, double>> mapping = getCurrentUserSortedSimilarityMapping();
    vector<pair<Song, double>> songsMapping;
    for (Song song:songs) {
        if (currentlyLoggedInMember->hasLikedSong(song.getId()))
            continue;
        double confidenceInSongRecommendation = 0;
        for (auto &itr:mapping)
            confidenceInSongRecommendation +=
                    ((double) itr.first->hasLikedSong(song.getId()) * itr.second) / mapping.size();
        songsMapping.emplace_back(make_pair(song, confidenceInSongRecommendation));
    }
    return songsMapping;
}

string MAPI::generateNewSid() {
    return sha1(to_string(rand()));
}

bool MAPI::isSidValid(const string &in) {
    for (SID sid:sids)
        if (sid.toString() == in)
            return true;
    return false;
}

void MAPI::invalidateSid(const string &in) {
    for (size_t i = 0; i < sids.size(); i++) {
        if (sids[i].toString() == in) {
            sids.erase(sids.begin() + i);
            break;
        }
        throw NotFoundException();
    }
}

Member *MAPI::getSidAssociatedMember(const string &sid) {
    for (size_t i = 0; i < sids.size(); i++) {
        if (sids[i].toString() == sid)
            return sids[i].getMember();
    }
    return nullptr;
}