#include <iostream>
#include <fstream>
#include "../Functionality/Base.hpp"
#include "../Functionality/Helper.hpp"

#include "../Exceptions/BadRequestException.hpp"
#include "../Exceptions/EmptyException.hpp"
#include "../Exceptions/ForbiddenException.hpp"
#include "../Exceptions/NotFoundException.hpp"
#include "../Exceptions/UserUnauthenticatedException.hpp"
#include "../Functionality/InitialUser.hpp"

#include "../server/server.hpp"
#include "Provider/URL.hpp"
#include "Provider/Handlers/LoginHandler.hpp"
#include "Provider/Handlers/SignupHandler.hpp"

vector<Song> readSongsCsv(const string &fileName) {
    vector<Song> out;
    ifstream fin(fileName);
    string line;
    bool isTheFirstLine = true;
    while (getline(fin, line)) {
        if (isTheFirstLine) {
            isTheFirstLine = false;
            continue;
        }
        vector<string> lineSplitted = Helper::split(line, ",");
        out.emplace_back(Song(stoi(lineSplitted[0]), stoi(lineSplitted[3]),
                              lineSplitted[1], lineSplitted[2], lineSplitted[4]));
    }
    return out;
}

vector<InitialUser> readLikesCsv(const string &fileName) {
    vector<InitialUser> out;
    ifstream fin(fileName);
    string line;
    bool isTheFirstLine = true;
    while (getline(fin, line)) {
        if (isTheFirstLine) {
            isTheFirstLine = false;
            continue;
        }
        vector<string> lineSplitted = Helper::split(line, ",");
        out.emplace_back(InitialUser{lineSplitted[0], lineSplitted[1],
                                     lineSplitted[2], stoi(lineSplitted[3])});
    }
    return out;
}

int main(int argc, char *argv[]) {
    vector<Song> songs =
            readSongsCsv(argv[argc - 2]);
//    Base baseService = Base(songs, readLikesCsv(argv[argc - 1]));
//    string requestLine;
    /*while (getline(cin, requestLine)) {
        try {
            baseService.processRequest(requestLine);
        } catch (BadRequestException &ex) {
            cout << "Bad Request\n";
        } catch (ForbiddenException &ex) {
            cout << "Permission Denied\n";
        } catch (UserUnauthenticatedException &ex) {
            cout << "Permission Denied\n";
        } catch (NotFoundException &ex) {
            cout << "Not Found\n";
        } catch (EmptyException &ex) {
            cout << "Empty\n";
        }
    }*/

    try {
        Server server(4999);
        server.setNotFoundErrPage("static/404.html");
        server.get(LOGIN_PAGE_URL, new ShowPage("HTML/login.html"));
        server.post(LOGIN_POST_URL, new LoginHandler());
        server.get(SIGNUP_PAGE_URL, new ShowPage("HTML/signup.html"));
        server.post(SIGNUP_POST_URL, new SignupHandler());
        server.run();
    } catch (Server::Exception e) {
        cerr << e.getMessage() << endl;
    }
}