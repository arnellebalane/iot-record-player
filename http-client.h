#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <map>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <BearSSLHelpers.h>
#include "secrets.h"

struct HttpResponse {
    int status;
    String body;
};

extern BearSSL::WiFiClientSecure httpClient;
extern BearSSL::X509List trustedRoots;

void configureCertificates();

HttpResponse sendHttpRequest(String method, String url, String payload, std::map<String, String> headers);

#endif
