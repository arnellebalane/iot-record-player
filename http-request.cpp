#include <map>
#include "WiFiClient.h"
#include "http-request.h"

HttpRequest parseHttpRequest(WiFiClient client) {
    HttpRequest request;
    parseHttpRequestMethod(&client, &request);
    parseHttpRequestPath(&client, &request);
    client.readString();
    return request;
}

void parseHttpRequestMethod(WiFiClient* client, HttpRequest* request) {
    (*request).method = (*client).readStringUntil(' ');
}

void parseHttpRequestPath(WiFiClient* client, HttpRequest* request) {
    String path = (*client).readStringUntil(' ');
    int searchIndex = path.indexOf('?');
    if (searchIndex >= 0) {
        (*request).path = path.substring(0, searchIndex);

        String searchParams = path.substring(searchIndex + 1);
        parseHttpRequestSearchParams(request, searchParams);
    } else {
        (*request).path = path;
        (*request).search = {};
    }
}

void parseHttpRequestSearchParams(HttpRequest* request, String searchParams) {
    int separatorIndex = searchParams.indexOf('&');
    if (separatorIndex >= 0) {
        int start = 0;
        do {
            String searchParam = searchParam.substring(start, separatorIndex);
            start = separatorIndex + 1;
            separatorIndex = searchParams.indexOf('&', start);
            parseHttpRequestSearchParamPair(request, searchParam);
        } while (separatorIndex >= 0);
    } else {
        parseHttpRequestSearchParamPair(request, searchParams);
    }
}

void parseHttpRequestSearchParamPair(HttpRequest* request, String searchParam) {
    int separatorIndex = searchParam.indexOf('=');
        if (separatorIndex >= 0) {
            String key = searchParam.substring(0, separatorIndex);
            String value = searchParam.substring(separatorIndex + 1);
            (*request).search[key] = value;
        }
}
