#include "LoginHandler.hpp"
#include "../../../Exceptions/BadRequestException.hpp"
#include "../URL.hpp"

Response *LoginHandler::callback(Request *req) {
    string user = req->getBodyParam(USER_PARAM_NAME);
    string pass = req->getBodyParam(PASS_PARAM_NAME);

    if (user.empty() || pass.empty())
        throw BadRequestException();
    string sid = api.login(user, pass);
    Response *response = Response::redirect(HOME_PAGE_URL);
    response->setSessionId(sid);
    return response;
}

LoginHandler::LoginHandler(MAPI &api) : api(&api) {}