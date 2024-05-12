#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <map>
#include "WiFiClient.h"

using SearchParams = std::map<String, String>;

struct HttpRequest {
    String method;
    String path;
    SearchParams search;
};

HttpRequest parseHttpRequest(WiFiClient client);
void parseHttpRequestMethod(WiFiClient* client, HttpRequest* request);
void parseHttpRequestPath(WiFiClient* client, HttpRequest* request);
void parseHttpRequestSearchParams(HttpRequest* request, String searchParams);
void parseHttpRequestSearchParamPair(HttpRequest* request, String searchParam);

#endif
