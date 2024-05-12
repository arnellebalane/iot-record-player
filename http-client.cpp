#include <map>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <BearSSLHelpers.h>
#include "secrets.h"
#include "http-client.h"

BearSSL::WiFiClientSecure httpClient;
BearSSL::X509List trustedRoots;

void configureCertificates() {
    trustedRoots.append(CA_CERTIFICATE.c_str());
    httpClient.setTrustAnchors(&trustedRoots);
}

HttpResponse sendHttpRequest(String method, String url, String payload, HttpHeaders headers) {
    HTTPClient http;
    http.begin(httpClient, url);

    for (auto const &[key, value] : headers) {
        http.addHeader(key, value);
    }

    HttpResponse response;
    response.status = 0;
    response.body = "";

    if (method == "POST") {
        response.status = http.POST(payload);
    } else if (method == "GET") {
        response.status = http.GET();
    }

    if (response.status == HTTP_CODE_OK) {
        response.body = http.getString();
    }
    http.end();

    return response;
}
