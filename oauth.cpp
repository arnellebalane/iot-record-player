#include "secrets.h"

String getAuthorizeUrl() {
    return OAUTH_AUTHORIZE_URL
        + "?client_id=" + OAUTH_CLIENT_ID
        + "&response_type=code"
        + "&redirect_uri=" + OAUTH_REDIRECT_URL
        + "&scope=" + OAUTH_SCOPES;
}
