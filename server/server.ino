

#include <ESP8266mDNS.h>
#include <DNSServer.h>
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



//DNSServer dnsServer;


//IPAddress apIP(192, 168, 0, 200);
//IPAddress netMsk(255, 255, 255, 0);

int led = 13;

void setup(void) {

  Serial.begin(115200);
  pinMode(13, INPUT_PULLUP);

  EEPROM.begin(sizeof(config_t));
  //EEPROM.put(0, conf);
  //EEPROM.commit();

  EEPROM.get(0, conf);
 
 

  //WiFi.softAPConfig(apIP, apIP, netMsk);
  //WiFi.softAP("TestIOT-rmm", "");

  // Wait for connection

  //Serial.print("IP address: ");
  //Serial.println(WiFi.softAPIP());

  /*if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
    } else {
    Serial.println("mDNS responder started");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
    }*/
  connectNetwork();
  WebServer_init();

  //dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //dnsServer.start(53, "*", apIP);


  if (isNetworkConnected()) {
    connectMQTT();
  }
}
void loop(void) {
  //DNS
  //dnsServer.processNextRequest();
  //HTTP
  WebServer_loop();

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
