#include "SignupHandler.hpp"
#include "../../../Exceptions/BadRequestException.hpp"
#include "../URL.hpp"

Response *SignupHandler::callback(Request *req) {
    string email = req->getBodyParam(EMAIL_PARAM_NAME);
    string user = req->getBodyParam(USER_PARAM_NAME);
    string pass = req->getBodyParam(PASS_PARAM_NAME);

    if (email.empty() || user.empty() || pass.empty())
        throw BadRequestException();
    string sid = api.signup(email, user, pass);
    Response *response = Response::redirect(HOME_PAGE_URL);
    response->setSessionId(sid);
    return response;
}