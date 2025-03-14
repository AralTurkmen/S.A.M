#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "time.h"
#include "sntp.h"

const char* ssid     = "####";         // The SSID (name) of the Wi-Fi network you want to connect to (MUST BE 2.4GHZ!!!)
const char* password = "####";      // Wi-Fi password
const char* ntpServer1 = "0.se.pool.ntp.org";
const char* ntpServer2 = "1.se.pool.ntp.org";
const char* timeZone = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"; // Offset for Sweden CET + 1

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Wifi setup
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    Serial.print(".");
    delay(800);
  }
  Serial.println("\n Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  //NTP setup
  configTime(0, 0, ntpServer1);
  setenv("TZ", timeZone, 1);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
}

//Function based on Epoch time
bool getNTPtime(int sec) {
  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
  }
  return true;
}

void loop() {
  // put your main code here, to run repeatedly:
  getNTPtime(10);

  Serial.print(timeinfo.tm_hour);
  Serial.print(':');
  Serial.print(timeinfo.tm_min);
  Serial.print(':');
  Serial.print(timeinfo.tm_sec);
  Serial.println("\n");
  
  delay(1000);
}