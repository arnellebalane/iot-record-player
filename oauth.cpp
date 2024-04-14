#include "secrets.h"

String getAuthorizeUrl() {
    return secrets::OAUTH_AUTHORIZE_URL
        + "?client_id=" + secrets::OAUTH_CLIENT_ID
        + "&response_type=code"
        + "&redirect_uri=" + secrets::OAUTH_REDIRECT_URL
        + "&scope=" + secrets::OAUTH_SCOPES;
}
