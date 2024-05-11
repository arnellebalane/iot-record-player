#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <map>
#include "WiFiClient.h"

struct HttpRequest {
    String method;
    String path;
    std::map<String, String> search;
};

HttpRequest parseHttpRequest(WiFiClient client);
void parseHttpRequestMethod(WiFiClient* client, HttpRequest* request);
void parseHttpRequestPath(WiFiClient* client, HttpRequest* request);
void parseHttpRequestSearchParams(HttpRequest* request, String searchParams);
void parseHttpRequestSearchParamPair(HttpRequest* request, String searchParam);

#endif
