#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <ESP8266WebServer.h>
#include "oauth.h"
#include "nfc.h"
#include "storage.h"
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
void receiveWifiCredentials();
void writeContextToNfcCard();

extern HttpHandlersMap httpHandlersStation;
extern HttpHandlersMap httpHandlersAccessPoint;
#endif
