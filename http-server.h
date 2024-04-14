#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "WiFiServer.h"
#include "WiFiClient.h"

void startHttpServer();
void maintainHttpClientConnections();
void handleHttpClientRequests();

#endif
