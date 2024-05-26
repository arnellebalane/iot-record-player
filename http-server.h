#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <ESP8266WebServer.h>
#include "oauth.h"
#include "secrets.h"
#include "http-client.h"

using HttpHandler = std::function<void ()>;
using HttpHandlersMap = std::map<String, HttpHandler>;

void startHttpServer();
void handleHttpClients();
void setHttpHandlers(HttpHandlersMap handlers);

// HTTP Request Handlers
void startAuthorization();
void receiveAuthorizationCode();

extern HttpHandlersMap httpHandlersStation;
#endif
