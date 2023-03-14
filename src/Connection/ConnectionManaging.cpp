/*
  Aquarium Project Pasquale
*/

#include "ConnectionManaging.h"

bool ConnectionManaging::connectWiFi(){ 
  int i = 0;
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 50) {
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

bool ConnectionManaging::disconnectWifi(){
  Serial.println("----- Disconnected -----");
  if(WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return true;
  }else{
    return false;
  }
}

bool ConnectionManaging::isConnect(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("isConnect NO!!!");
    return false;
  }else{
    Serial.println("isConnect YES!!!");
    return true;
  }
}

const char *ConnectionManaging::conectionStatus() {
        const char *status;
        switch (WiFi.status()) {
            case WL_NO_SSID_AVAIL:
            //Serial.println("Configured:[SSID]");
            status = "SSID";
            return status;
            case WL_CONNECTED:
            //Serial.println("Connection:[OK]");
            status = "OK";
            return status; 
            case WL_CONNECT_FAILED:
            //Serial.println("Connection:[KO]");
            status = "KO";
            return status;
            default: 
            //Serial.println("Connection:[KO]");
            status = "KO";
            return status;
        }
}

void ConnectionManaging::postHttpsRequest(String var1, float value1, String var2, String value2) {   
      /* set SSL/TLS certificate */
    client.setCACert(ca_cert);

    Serial.println("Connect to server via port 443");
    if (!client.connect(serverName, 443)){
        Serial.println("Connection failed!");
    } else {
        String postData; 
        //Post Data
        postData = var1 + String("=");
        postData += String(value1);
        postData += "&" + var2 + String("=");
        postData += value2;
        String pageSite = serverName + String("/take") + var1 + String (".php");
        
        HTTPClient http;  
        if (http.begin(client, "https://"+ pageSite)) {
            /* create HTTP request */
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            // Data to send with HTTP POST
            int httpResponseCode = http.POST(postData);
            //Get the response payload    
            String payload = http.getString();

            if(httpResponseCode == HTTP_CODE_OK){
              //Print HTTP return code
              Serial.print("SENT HTTP POST TO ");
              Serial.println(var1);
            }else{
              Serial.println("No HTTP");
            } 
            // Free resources
            http.end();
            delay(50);
            client.stop();
        }else{
          Serial.println("No Connect");
        } 
    }
}

bool ConnectionManaging::sendValueToWeb(float value, String key, String timeStamp) {
  //String fileName = key + "_v.jso";
  //saveValue(fileName, value, timeStamp, key);
  if(connectWiFi()){
    postHttpsRequest(key, value, "Date", timeStamp);  
    disconnectWifi();
    return true;
  }else{
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





// const char* ssid = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// const char* password = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

// const char* host = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// //const char* fingerprint = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; //https://www.grc.com/fingerprints.html
// boolean wifiConnected = false;
// float ph, temperature, tds;
// String msg = "";

// const int httpsPort = 443; // 80 is for HTTP / 443 is for HTTPS!
  
// void setup() {
// // put your setup code here, to run once:
//   Serial.begin(115200);
//   connectToWifi();
// }

// void loop() {
// // put your main code here, to run repeatedly:
//   if (Serial.available()) {
//     while (Serial.available()) {
//       msg += char(Serial.read());
//       delay(10);
//     }
//     //Serial.print(msg);
//     if (msg.indexOf("Temp") >= 0) { 
//       String parameter1 = splitString(msg, ':', 0);
//       String parameter2 = splitString(msg, ':', 1);
//       postHttpsRequest(parameter1, parameter2);
//     } else if (msg.indexOf("Ph") >= 0) {
//       String parameter1 = splitString(msg, ':', 0);
//       String parameter2 = splitString(msg, ':', 1);
//       postHttpsRequest(parameter1, parameter2);
//     } else if (msg.indexOf("Ec") >= 0) {
//       String parameter1 = splitString(msg, ':', 0);
//       String parameter2 = splitString(msg, ':', 1);
//       postHttpsRequest(parameter1, parameter2);
//     } else if (msg.indexOf("Restart") >= 0) {
//       reconnectWifi();
//     } else if (msg.indexOf("ConInfo") >= 0) {
//       conectionStatus();
//     } else {
//       Serial.println("Wrong command");
//     }
//     msg = "";
//     //Serial.flush();
//   }
// }

// //After declaring the HTTPClient object and parsing the parameters, it is built the 
// //HTTPS POST request in order to send the value to web site. When it will reveive 
// //the response from the api web it will write it on Serial channel and Arduino will
// //be able to read it. 
// void postHttpsRequest(String parameter1, String parameter2) {   
//   String postData; 
//   String var1 = splitString(parameter1, '=', 0);
//   float value1 = splitString(parameter1, '=', 1).toFloat();
//   String var2 = splitString(parameter2, '=', 0);
//   String value2 = splitString(parameter2, '=', 1);
//   //Post Data
//   postData = var1 + String("=");
//   postData += String(value1);
//   postData += "&" + var2 + String("=");
//   postData += value2;
//   String pageSite = host + String("take") + var1 + String (".php");
//   std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
//   client->setInsecure(); // this is the magical line that makes everything work
//   //client.connect(pageSite, httpsPort);
//   HTTPClient http;  
//   if (http.begin(*client, pageSite)) { //works
//     //http.begin(pageSite, fingerprint);
//     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//     //Send the request
//     int httpCode = http.POST(postData); 
//     //Get the response payload    
//     String payload = http.getString();   
//     if(httpCode == HTTP_CODE_OK){
//     //Print HTTP return code
//       Serial.println(payload + "["+String(httpCode)+"_ans]"); 
//     }else{
//       value2.trim();
//       Serial.println(var1 + ":" + String(value1) + ";" + String(value2) + "["+String(httpCode)+"_ans]");
//     } 
//     //Print request response payload
//     //Serial.println(payload);    
//     http.end();
//   }else{
//     value2.trim();
//     Serial.println(var1 + ":" + String(value1) + ";" + String(value2) + "["+String(408)+"_ans]");
//   } 
// }

// // It connects again the WiFi module to internet
// void reconnectWifi() {
//   boolean state = true;
//   int i = 0;
//   Serial.println("Reconnecting to WiFi...");
//   WiFi.disconnect();
// //Prevents reconnection issue (taking too long to connect)
//   WiFi.mode(WIFI_OFF);        
//   delay(1000);
// //This line hides the viewing of ESP as wifi hotspot  
//   WiFi.mode(WIFI_STA);        
// //Alternatively, you can restart your board
//   ESP.restart();
//   //WiFi.begin(ssid, password);
//   //while (WiFi.status() != WL_CONNECTED) {
//   //  delay(500);
//   //  if (i > 20) {
//   //    state = false; break;
//   //  }
//   //  i++;
//   //}
// }

// //It Writes the connection status on Serial channel. In this 
// //way Arduino will be able to read and will show it.
// void conectionStatus() {
//   switch (WiFi.status()) {
//     case WL_NO_SSID_AVAIL:
//       Serial.println("Configured:[SSID]");
//       break;
//     case WL_CONNECTED:
//       Serial.println("Connection:[OK]");
//       break;
//     case WL_CONNECT_FAILED:
//       Serial.println("Connection:[KO]");
//       break;
//     default: 
//       Serial.println("Connection:[KO]");
//     break;
//   }
// }

// //Connect to wifi and it returns true if successful or false if not
// boolean connectToWifi() {
//   boolean state = true;
//   int i = 0;
   
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.println("");
//   Serial.println("Connecting to WiFi");

//   // Wait for connection
//   Serial.print("Connecting...");
//   while(WiFi.status() != WL_CONNECTED){
//     delay(1000);
//     Serial.print(".");
//     if (i > 30) {
//       state = false; break;
//     }
//     i++;
//   }
//   Serial.println("");
//   if (state) {
//     Serial.print("Connected to ");
//     Serial.println(ssid);
//     Serial.print("IP address: ");
//     Serial.print(WiFi.localIP());
//     Serial.println("!");
//   }
//   else {
//     Serial.println("Connection failed!");
//   }
// //The ESP8266 tries to reconnect automatically when the connection is lost
//   WiFi.setAutoReconnect(true);
//   WiFi.persistent(true);
//   delay(100);
//   return state;
// }

// //Split the read message to process it
// String splitString(String data, char separator, int index)
// {
//   int found = 0;
//   int strIndex[] = { 0, -1 };
//   int maxIndex = data.length() - 1;

//   for (int i = 0; i <= maxIndex && found <= index; i++) {
//     if (data.charAt(i) == separator || i == maxIndex) {
//       found++;
//       strIndex[0] = strIndex[1] + 1;
//       strIndex[1] = (i == maxIndex) ? i+1 : i;
//     }
//   }
//   return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
// }