#include <ESP8266WebServer.h> //memasukan library NodeMCU sebagai web server
#include <ESP8266WiFi.h> //memasukan library NodeMCU
#include <WiFiClient.h> //memasukan library NodeMCU sebagai client
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
//WIFI DEVICE
const char *ssid = "SMART-PROJECT";
const char *password = "smart123456";

WiFiClient WifiClient;
//webserver device port 80
ESP8266WebServer server(80);
//VARIABELS
#define LAMP_INDICATOR D2
const int RELAYPIN = 5;


// SERVER SMART PROJECT CONFIGURATION ----------------------------------------------------------------------------------
String serverhost = "192.168.1.4";
String serverport = "8000";
String baseurl = "http://"+serverhost+":"+serverport;
String deviceurl = baseurl+"/user/smart-home-devices/";
// END SMART PROJECT CONFIGURATION -------------------------------------------------------------------------------------


//HTML SCRIPT ----------------------------------------------------------------------------------------------------------
char* htmlIndex = "<!DOCTYPE html><html><head><title>Smart Project</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style type=\"text/css\">*{margin: 0; padding: 0; outline: 0; font-family: 'Open Sans', sans-serif;}body{height: 100vh;}.container{padding-inline: 10rem;padding-block: 5rem;display: flex;justify-content: center;}.container-form{padding-block: 4rem; padding-inline:3rem; width: 450px; border-radius: 1rem; background-color: #044E70; box-shadow: 0 0 10px rgba(255,255,255,.3);}.container-form svg{width: 8rem;height: 8rem; border-radius: 50%; border: 1px solid white;}.container-form .logo{display: flex;justify-content: center;margin-bottom: 3rem;}.container-form h1{text-align: left; color: #fafafa; margin-bottom: 2rem; text-transform: uppercase; text-align: center;}.container-form label{text-align: left; color: #90caf9;}.container-form form input{width: calc(100% - 20px); padding: 8px 10px; margin-bottom: 15px; border: none; background-color: transparent; border-bottom: 2px solid #fafafa; color: #fff; font-size: 20px;}.container-form form button{margin-top: 3rem; width: 100%; padding: 8px 0; border: none; background-color:#fafafa; font-size: 18px; color: #005D85;}.container-form .card{margin-top: 2rem;height: 5.5rem;border: 1px solid lightgrey;border-radius: .7rem;padding: 1rem;}.container-form .card .card-body{text-align: center;color: lightgrey;}@media only screen and (max-width: 768px){.container{padding-inline: 1rem;padding-block: 1rem;display: flex;justify-content: center;}.container-form{padding-block: 3rem; padding-inline:2rem; width: 450px; border-radius: 1rem; background-color: #044E70; box-shadow: 0 0 10px rgba(255,255,255,.3);}.container-form .card .card-body{font-size: .8rem;}}</style></head><body><div class=\"container\"><div class=\"container-form\"><div class=\"logo\"><svg id=\"svg\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0, 0, 400,400\"><g id=\"svgg\"><path id=\"path0\" d=\"M86.600 157.165 L 86.603 181.540 120.128 198.221 L 153.653 214.901 179.121 202.275 C 193.129 195.331,204.588 189.580,204.587 189.495 C 204.585 189.410,194.759 184.653,182.751 178.923 C 170.743 173.192,155.588 165.959,149.075 162.849 L 137.232 157.194 112.107 169.197 C 98.289 175.798,86.932 181.199,86.869 181.199 C 86.806 181.199,86.719 170.307,86.676 156.995 L 86.597 132.791 86.600 157.165 \" stroke=\"none\" fill=\"#047bac\" fill-rule=\"evenodd\"></path><path id=\"path1\" d=\"M204.787 74.809 C 204.678 74.917,204.589 86.239,204.589 99.969 L 204.589 124.931 205.255 125.362 C 205.622 125.600,210.651 128.050,216.432 130.809 C 222.213 133.567,233.005 138.719,240.415 142.259 C 259.160 151.213,271.180 156.914,274.315 158.335 C 275.781 158.999,287.169 164.416,299.623 170.371 C 312.077 176.327,322.369 181.199,322.495 181.199 C 322.621 181.199,322.724 170.365,322.724 157.124 L 322.724 133.048 314.212 128.864 C 309.530 126.563,283.072 113.415,255.416 99.646 C 227.761 85.877,205.100 74.611,205.059 74.611 C 205.018 74.611,204.895 74.700,204.787 74.809 M204.786 227.584 C 204.678 248.374,204.589 272.955,204.589 282.210 L 204.589 299.038 229.017 299.038 L 253.444 299.038 253.369 257.371 L 253.294 215.703 229.312 202.764 C 216.121 195.647,205.251 189.816,205.156 189.805 C 205.061 189.794,204.895 206.795,204.786 227.584 \" stroke=\"none\" fill=\"#ebe304\" fill-rule=\"evenodd\"></path><path id=\"path2\" d=\"M238.296 173.280 C 219.788 182.114,204.733 189.429,204.839 189.536 C 204.946 189.643,215.959 195.611,229.312 202.797 L 253.590 215.864 287.935 198.798 C 306.825 189.412,322.367 181.646,322.474 181.540 C 322.616 181.399,273.555 157.751,272.168 157.292 C 272.046 157.252,256.804 164.446,238.296 173.280 \" stroke=\"none\" fill=\"#ccbc14\" fill-rule=\"evenodd\"></path><path id=\"path3\" d=\"M145.445 103.829 L 86.751 133.047 86.751 157.123 C 86.751 170.365,86.803 181.199,86.867 181.199 C 86.931 181.199,98.279 175.803,112.085 169.208 C 125.891 162.613,137.274 157.217,137.381 157.217 C 137.488 157.217,152.620 150.031,171.008 141.249 L 204.441 125.281 204.517 99.946 C 204.558 86.012,204.490 74.611,204.365 74.611 C 204.241 74.611,177.726 87.759,145.445 103.829 M174.833 204.332 C 158.712 212.309,132.232 225.451,115.988 233.537 L 86.454 248.238 86.454 272.327 C 86.454 293.897,86.502 296.406,86.912 296.321 C 87.164 296.268,113.741 283.634,145.973 268.246 L 204.576 240.266 204.582 215.026 C 204.586 201.144,204.489 189.795,204.367 189.807 C 204.245 189.819,190.955 196.355,174.833 204.332 \" stroke=\"none\" fill=\"#04a4eb\" fill-rule=\"evenodd\"></path><path id=\"path4\" d=\"M204.731 214.804 C 204.731 228.808,204.767 234.494,204.811 227.439 C 204.855 220.384,204.855 208.926,204.811 201.976 C 204.767 195.027,204.731 200.799,204.731 214.804 \" stroke=\"none\" fill=\"#cacc24\" fill-rule=\"evenodd\"></path></g></svg></div><h1>WiFi Login</h1><form action=\"/login\" method=\"POST\"> <label>Your WiFi SSID</label><br><input type=\"text\" name=\"ssid\"><br><label>Password</label><br><input type=\"password\" name=\"password\"><br><button type=\"submit\">Log in</button></form><div class=\"card\"><div class=\"card-body\">To control the Smart Project Device remotely make sure you connect this Smart Project Device to your WIFI. <br><br><b><i>\"Make it simple, make it smart\"</i></b></div></div></div></div></body></html>";

char* notFoundHtml = "<div style=\"text-align: center\">"
" <h1 >URL NOT FOUND !</h1>"
" <a href=\"/\" >Back To Home</a>"
"</div>";


String successHtml = "<!DOCTYPE html><html><head><title>Login Success</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style type=\"text/css\">*{margin: 0; padding: 0; outline: 0; font-family: 'Open Sans', sans-serif;}body{height: 100vh;}.container{padding-inline: 10rem;padding-block: 5rem;display: flex;justify-content: center;}.container-form{padding-block: 4rem; padding-inline:3rem; width: 450px; border-radius: 1rem; background-color: #044E70; box-shadow: 0 0 10px rgba(255,255,255,.3); display: flex; flex-direction: column;}.container-form h1{text-align: left; color: #fafafa; margin-bottom: 2rem; text-align: center; font-size: 1.5rem;}.container-form .btn{text-align: center; width: 20%; padding: 10px 14px; border: none; background-color:#fafafa; font-size: 18px; color: #005D85; text-decoration: none; border-radius: 1rem; align-self: center;}@media only screen and (max-width: 768px){.container{padding-inline: 1rem;padding-block: 1rem;display: flex;justify-content: center;}.container-form{padding-block: 3rem; padding-inline:2rem; width: 450px; border-radius: 1rem; background-color: #044E70; box-shadow: 0 0 10px rgba(255,255,255,.3);}.container-form .btn{width: 50%;}}</style></head><body><div class=\"container\"><div class=\"container-form\"><h1>Your Smart Device has been connected successful !</h1><a href=\""+baseurl+"\" class=\"btn\">Continue</a></div></div></body></html>";

//END HTML SCRIPT ------------------------------------------------------------------------------------------------------

// FUNCTIONS -----------------------------------------------------------------------------------------------------------
void handleNotFound(){
  server.send(404, "text/html", notFoundHtml); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleRoot() {
  server.send(200, "text/html", htmlIndex);
}

void handleLogin() {                         // If a POST request is made to URI /login
  if (server.method() != HTTP_POST) {
    digitalWrite(LAMP_INDICATOR, HIGH);
    server.send(405, "text/plain", "Method Not Allowed");
    delay(1000);
    digitalWrite(LAMP_INDICATOR, LOW);
  } else {
    if( ! server.hasArg("ssid") || ! server.hasArg("password") 
        || server.arg("ssid") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
      server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
      return;
    }
    const String wifiuser = server.arg("ssid");
    const String wifipassword = server.arg("password");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiuser, wifipassword);
    Serial.print("Connecting ");
    delay(3000);
    
    while (WiFi.status() != WL_CONNECTED){
        digitalWrite(LAMP_INDICATOR, 1);
        delay(500);
        Serial.print(".");
        digitalWrite(LAMP_INDICATOR, 0);
    }
      Serial.println(" ");
      Serial.println("WIFI CONNECTED!!!");
    
  }
}

// END FUNCTIONS --------------------------------------------------------------------------------------------------------

 
void setup() {
    //delay(1000); //waktu tunda 1 detik
    Serial.begin(9600);
    Serial.println("PROGRAM STARTED !");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);//bertindak sebagai access point dengan nama wifi dan pass diatas
 
    IPAddress apip = WiFi.softAPIP(); //alamat IP
    Serial.print("visit: \n"); 
    //menuliskan alamat IP yang bisa dikunjungi
    Serial.println(apip);
    pinMode(LAMP_INDICATOR, OUTPUT);
    pinMode(RELAYPIN, OUTPUT);
    //digitalWrite(RELAYPIN, LOW);
    digitalWrite(LAMP_INDICATOR, LOW);
    server.on("/", HTTP_GET, handleRoot);
    server.on("/login", HTTP_POST, handleLogin);
    server.onNotFound(handleNotFound); 
    server.begin(); //proses pemulaian menjadi server
    Serial.println("HTTP server beginned");
}
 
void loop() {
  server.handleClient();

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("belum");
    digitalWrite(LAMP_INDICATOR, HIGH);
    delay(500);
    digitalWrite(LAMP_INDICATOR, LOW);
    Serial.println("belum juga");
    delay(1000);
  }else{
    String host = deviceurl;
         host += "LP01-SP16122021082537";
  
    HTTPClient http;
    
    Serial.print("Request Link : ");
    Serial.println(host);
    http.begin(WifiClient, host);
    int httpCode = http.GET();            //Send the GET request
    String payload = http.getString();    //Get the response payload from server
          
    Serial.print("Response Code:"); //200 is OK
    Serial.println(httpCode);       //Print HTTP return code
          
    Serial.print("Returned data from Server:");
    Serial.println(payload);    //Print request response payload

    if(httpCode == 200)
          {
            DynamicJsonDocument doc(1024);
          
           // Parse JSON object
            auto error = deserializeJson(doc, payload);
            if (error) {
              Serial.print(F("deserializeJson() failed with code "));
              Serial.println(error.c_str());
              return;
            }
          
            // Decode JSON/Extract values
            String responStatus = doc["status"].as<String>();
            if(responStatus == "1"){
              digitalWrite(RELAYPIN, HIGH);
            }else{
              digitalWrite(RELAYPIN, LOW);
            }
          }
  }
}
