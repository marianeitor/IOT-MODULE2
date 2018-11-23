#include <EEPROM.h>
#include "config.h"
#include "mqtt.h"
#include "webserver.h"
#include "network.h"

int timerConnectMqtt = 0;

/* const char* mqttServer = "m15.cloudmqtt.com";
  const int mqttPort = 10396;
  const char* mqttUser = "altegzoe";
  const char* mqttPass = "X5V9Dh-ci8Ts";*/

config_t conf;


int timer = 0;
bool lastState = false;
int rstCounter = 0;



//DNSServer dnsServer;


//IPAddress apIP(192, 168, 0, 200);
//IPAddress netMsk(255, 255, 255, 0);


void setup(void) {

  Serial.begin(115200);
  pinMode(13, INPUT_PULLUP); //PIN D7
  pinMode(15, INPUT); //PIN D8

  EEPROM.begin(sizeof(config_t));
 
  /*if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
    } else {
    Serial.println("mDNS responder started");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
    }*/
  connectNetwork();
  WebServer_init();


  if (isNetworkConnected()) {
    connectMQTT();
  }
}
void loop(void) {

  /*if (!digitalRead(15)) {
    if (rstCounter >= 10) {
      factoryReset();
    }
    else {
      rstCounter++;
      Serial.println("Resetting");
      Serial.println(rstCounter);
    }
  }*/
  
  WebServer_loop();
  proccessNextDnsReq();

  timer++;
  timerConnectMqtt++;
  delay(10);

  bool currState = digitalRead(13);

if (isNetworkConnected()) {
  if (!isMqttConnected() && timerConnectMqtt >= 500) {
    timerConnectMqtt = 0;
    connectMQTT();
  }
}

  if (lastState != currState) {
    publishMQTT(currState ? "true" : "false");
    lastState = currState;
  }

  if (timer >= 3000) {
    timer = 0;
    if (isMqttConnected()) {
      if (digitalRead(13))  {
        publishMQTT("true");
        Serial.println("Sent True");
      }
      else {
        publishMQTT("false");
        Serial.println("Sent False");
      }
    }
    else {
      Serial.println("Not Connected MQTT");
    }
  }
}
