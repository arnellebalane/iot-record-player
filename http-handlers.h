#ifndef HTTP_HANDLERS_H
#define HTTP_HANDLERS_H

#include <functional>
#include <map>
#include "WiFiClient.h"
#include "oauth.h"
#include "http-request.h"

void startAuthorization(WiFiClient* client, HttpRequest* request);
void receiveAuthorizationCode(WiFiClient* client, HttpRequest* request);

using HttpHandler = std::function<void (WiFiClient* client, HttpRequest* request)>;
using HttpHandlersMap = std::map<String, HttpHandler>;
extern HttpHandlersMap httpHandlers;

#endif
