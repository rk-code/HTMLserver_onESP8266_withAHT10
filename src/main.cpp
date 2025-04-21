#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
<<<<<<< HEAD
#include <AHTxx.h>

String prepareHtmlPage(void);

AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);
=======
#include <AHT10.h>
//#include <Adafruit_AHTX0.h>

// over the air (OTA) via command line 
// pio run -t upload --upload-port 192.168.178.180

String prepareHtmlPage(void);

//AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);
AHT10 aht10(AHT10_ADDRESS_0X38, AHT10_SENSOR);
>>>>>>> e64f544 (add platformio and smaller changes)

#include "stassid.h"

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();

  while (aht10.begin(2, 14) != true) // sda = 2, scl = 14
  {
    Serial.println(F("AHT1x not connected or fail to load calibration coefficient."));
    delay(5000);
  }
  Serial.println(F("ATH10 OK"));

  
<<<<<<< HEAD
  WiFi.hostname(F("ESP8266-AHT-1"));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
=======
  WiFi.hostname(F("ESP8266-AHT-10"));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print(F("ESP8266 MAC: "));
  Serial.println(WiFi.macAddress());

>>>>>>> e64f544 (add platformio and smaller changes)
  // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.println("Connection Failed! Rebooting...");
  //   delay(5000);
  //   ESP.restart();
  // }
  Serial.print(F("WiFi connecting "));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(F("Connected"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("Hostname: "));
  Serial.println(WiFi.getHostname());

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);
<<<<<<< HEAD

=======
>>>>>>> e64f544 (add platformio and smaller changes)
  // Hostname defaults to esp8266-[ChipID]
  //ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  
  server.begin();
  Serial.println(F("Server started"));
}

void loop() {
  ArduinoOTA.handle();

  WiFiClient client = server.accept();
  
  if (client)
  {
<<<<<<< HEAD
    Serial.println("\n[Client connected]");
=======
    Serial.println("\n[Client connected ...]");
>>>>>>> e64f544 (add platformio and smaller changes)
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }

    while (client.available()) {
      // but first, let client finish its request
      // that's diplomatic compliance to protocols
      // (and otherwise some clients may complain, like curl)
      // (that is an example, prefer using a proper webserver library)
      client.read();
    }

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
  }
}

String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024);
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 10\r\n"        // refresh the page automatically every 10 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html lang=\"de\">"
               "<head>"
                  "<meta charset=\"utf-8\">"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                  "<title>MHT10</title>"
               "</head>"
               "<body>" // content
                  "<p>"
                  "Temperature:  ");
  htmlPage += String(aht10.readTemperature(),1);
  delay(100);
  htmlPage += F("&pm;0.3℃<br>");
  htmlPage += F("Humidity:              ");
  htmlPage += String(aht10.readHumidity(),0);
  htmlPage += F("±2%<br>");
  htmlPage += F("</p>");
  htmlPage += F("</body>");
  htmlPage += F("</html>"
                "\r\n");
  delay(100);
  return htmlPage;
}