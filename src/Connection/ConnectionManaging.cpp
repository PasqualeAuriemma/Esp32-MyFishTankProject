/*
  Aquarium Project Pasquale
*/

#include "ConnectionManaging.h"

bool ConnectionManaging::connectWiFi()
{
  int i = 0;
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 50)
    {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      Serial.println("\n-- WiFi not connected --");
      return false;
    }
    i++;
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.println("");
  Serial.println("---- WiFi connected ----");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

bool ConnectionManaging::disconnectWifi()
{
  Serial.println("----- Disconnected -----");
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return true;
  }
  else
  {
    return false;
  }
}

bool ConnectionManaging::isConnect()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("isConnect NO!!!");
    return false;
  }
  else
  {
    Serial.println("isConnect YES!!!");
    return true;
  }
}

const char *ConnectionManaging::conectionStatus()
{
  const char *status;
  switch (WiFi.status())
  {
  case WL_NO_SSID_AVAIL:
    // Serial.println("Configured:[SSID]");
    status = "SSID";
    return status;
  case WL_CONNECTED:
    // Serial.println("Connection:[OK]");
    status = "OK";
    return status;
  case WL_CONNECT_FAILED:
    // Serial.println("Connection:[KO]");
    status = "KO";
    return status;
  default:
    // Serial.println("Connection:[KO]");
    status = "KO";
    return status;
  }
}

void ConnectionManaging::postHttpsRequest(String var1, float value1, String var2, String value2)
{
  /* set SSL/TLS certificate */
  client.setCACert(ca_cert);

  Serial.println("Connect to server via port 443");
  if (!client.connect(serverName, 443))
  {
    Serial.println("Connection failed!");
  }
  else
  {
    String postData;
    // Post Data
    postData = var1 + String("=");
    postData += String(value1);
    postData += "&" + var2 + String("=");
    postData += value2;
    String pageSite = serverName + String("/take") + var1 + String(".php");

    HTTPClient http;
    if (http.begin(client, "https://" + pageSite))
    {
      /* create HTTP request */
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      int httpResponseCode = http.POST(postData);
      // Get the response payload
      String payload = http.getString();

      if (httpResponseCode == HTTP_CODE_OK)
      {
        // Print HTTP return code
        Serial.print("SENT HTTP POST TO ");
        Serial.println(var1);
      }
      else
      {
        Serial.println("No HTTP");
      }
      // Free resources
      http.end();
      delay(50);
      client.stop();
    }
    else
    {
      Serial.println("No Connect");
    }
  }
}

bool ConnectionManaging::sendValueToWeb(float value, String key, String timeStamp)
{
  // String fileName = key + "_v.jso";
  // saveValue(fileName, value, timeStamp, key);
  if (connectWiFi())
  {
    postHttpsRequest(key, value, "Date", timeStamp);
    disconnectWifi();
    return true;
  }
  else
  {
    return false;
  }
}

// void ConnectionManaging::chackIfSendValue(byte pivot, byte Minute, float value, String key, String now) {
//   if (key == "Temp" && value != -1000.00 && value != 0.0) {
//     if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSend) {
//       sendValueToWeb(value, key, now);
//       previousSecSend = S;
//     }
//   }else if (key == "Ec" && int(value) != 1) {
//     if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSendEC) {
//       previousSecSendEC = S;
//       sendValueToWeb(value, key, now);
//     }
//   }else (key == "Ph" && int(value) != 1) {
//     if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSendPh) {
//       previousSecSendPh = S;
//       sendValueToWeb(value, key, now);
//     }
//   }
// }

// void resendValueToWeb(String value, String key,  String timeStamp) {
//   String payload = "";
//   payload += key;
//   payload += "=";
//   payload += value;
//   payload += ":";
//   payload += "Date=";
//   payload +=  timeStamp;
//   payload += ";";
//   Serial.println(payload);
//   Serial3.println(payload);
// }

// void chackWhenResendMeasure(byte _hour, byte _minute) {
//     if (H == _hour && M == _minute && S == 0 && S != previousSecResend) {
//     Serial.println("Sono qui! 1");
//     loadingNotSendedMeasure();
//     previousSecResend = S;
//     }else{

//       if(S > 0 && previousSecResend == 0){
//         Serial.println("Sono qui! 2");
//         previousSecResend = S;
//       }
//     }
// }