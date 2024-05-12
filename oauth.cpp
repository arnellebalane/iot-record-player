#include "base64.h"
#include "secrets.h"

base64 b64;

String getAuthorizeUrl() {
    return OAUTH_AUTHORIZE_URL
        + "?client_id=" + OAUTH_CLIENT_ID
        + "&response_type=code"
        + "&redirect_uri=" + OAUTH_REDIRECT_URL
        + "&scope=" + OAUTH_SCOPES;
}

String getTokenRequestPayload(String authCode) {
    return "client_id=" + OAUTH_CLIENT_ID
        + "&grant_type=authorization_code"
        + "&code=" + authCode
        + "&redirect_uri=" + OAUTH_REDIRECT_URL;
}

String getBasicAuthorizationHeader() {
    String original = OAUTH_CLIENT_ID + ":" + OAUTH_CLIENT_SECRET;
    String encoded = b64.encode(original);
    return "Basic " + encoded;
}
