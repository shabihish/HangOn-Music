#ifndef AP_CA7_PHASE2_SIGNUPHANDLER_HPP
#define AP_CA7_PHASE2_SIGNUPHANDLER_HPP

#include "../../../server/server.hpp"
#include "MHandler.hpp"
#include <string>

using namespace std;

class SignupHandler : public MHandler {
private:
    Response *callback(Request *req) override;

public:
    const string EMAIL_PARAM_NAME = "email";
    const string USER_PARAM_NAME = "user";
    const string PASS_PARAM_NAME = "pass";
};


#endif
