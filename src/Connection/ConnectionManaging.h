/*
  Aquarium Project Pasquale
*/

#ifndef ConnectionManaging_H_
#define ConnectionManaging_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
class ConnectionManaging
{
private:
    const char *ssid;
    const char *password;
    const char *serverName; // Server URL
    const char *ntpServer = "pool.ntp.org";
    long gmtOffset_sec = 3600; // one hour more
    int daylightOffset_sec = 3600;
    byte previousSec = 0;
    byte previousSecSend = 55;
    byte previousSecSendEC = 55;
    byte previousSecSendPh = 55;
    /* create an instance of WiFiClientSecure */
    WiFiClientSecure client;

    /* use openssl s_client -showcerts -connect www.myfishtank.altervista.org:443
     * openssl s_client -showcerts -connect www.howsmyssl.com:443 </dev/null
     * to get this certificate with
     * 2 s:C = US, O = Internet Security Research Group, CN = ISRG Root X1
     *  i:O = Digital Signature Trust Co., CN = DST Root CA X3
     */
    const char *ca_cert;

public:
    ConnectionManaging(const char *_ssid, const char *_password, const char *_server, const char *_ca_cert)
    {
        this->ssid = _ssid;
        this->password = _password;
        this->serverName = _server;
        this->ca_cert = _ca_cert;
    }

    ~ConnectionManaging(){};

    /**
     * @fn connectWiFi
     * @brief Connecting to a WiFi network.
     */
    bool connectWiFi();

    /**
     * @fn connectWiFi
     * @brief Disconnect WiFi as it's no longer needed.
     */
    bool disconnectWifi();

    /**
     * @fn isConnect
     * @brief Return the connect status.
     */
    bool isConnect();

    /**
     * @fn postHttpsRequest
     * @brief After declaring the HTTPClient object and parsing the parameters, it is built the
     * HTTPS POST request in order to send the value to web site. When it will reveive
     * the response from the api web it will write it on Serial channel and Arduino will
     * be able to read it.
     * @param var1: name of the sensor.
     * @param value1: sensor's value.
     * @param var2: time variable name.
     * @param value2: time value.
     */
    void postHttpsRequest(String var1, float value1, String var2, String value2);

    /**
     * @fn conectionStatus
     * @brief Getting the connection status.
     */
    const char *conectionStatus();

    /**
     * @fn sendValueToWeb
     * @brief Sending the value to the web site.
     * @param value: sensor's value.
     * @param key: name of the sensor.
     * @param timeStamp: time variable name.
     */
    bool sendValueToWeb(float value, String key, String timeStamp);
    // void chackIfSendValue(byte pivot, byte Minute, float value, String key, String now);
};

#endif