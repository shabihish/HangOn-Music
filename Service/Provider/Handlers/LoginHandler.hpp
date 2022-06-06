#ifndef AP_CA7_PHASE2_LOGINHANDLER_HPP
#define AP_CA7_PHASE2_LOGINHANDLER_HPP

#include "../../../server/server.hpp"
#include "../../../Functionality/MAPI.hpp"
#include <string>

using namespace std;

class LoginHandler : public RequestHandler {
private:
    Response *callback(Request *req) override;
    MAPI* api;
public:
    const string USER_PARAM_NAME = "user";
    const string PASS_PARAM_NAME = "pass";
    LoginHandler(MAPI &api);
};


#endif
