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

using HttpHeaders = std::map<String, String>;

extern BearSSL::WiFiClientSecure httpClient;
extern BearSSL::X509List trustedRoots;

void configureCertificates();

HttpResponse sendHttpRequest(String method, String url, String payload, HttpHeaders headers);

#endif
