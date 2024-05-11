#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "WiFiServer.h"
#include "WiFiClient.h"
#include "http-request.h"
#include "http-handlers.h"

void startHttpServer();
void manageHttpClientConnections();
void handleHttpClientRequests(HttpHandlersMap handlers);

#endif
