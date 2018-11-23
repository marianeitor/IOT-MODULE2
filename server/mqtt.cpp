#include "mqtt.h"
#include "config.h"
#include <EEPROM.h>

WiFiClient espClient;
PubSubClient clientMQTT(espClient);

void connectMQTT() {
  int timeout = 10;
   config_t conf;
   EEPROM.get(0, conf);

    clientMQTT.setServer(conf.broker_add, conf.broker_puerto);
    clientMQTT.connect(conf.clientID, conf.broker_user, conf.broker_pass);
    
    Serial.println("Start MQTT Connection");
    
    while (!clientMQTT.connected() && timeout--){
      Serial.print("."); 
      delay(500);
    }
    if (clientMQTT.connected()) {
       Serial.println("MQTT Connected!"); 
    }
}

void publishMQTT(const char data[]) {
  config_t conf;
   EEPROM.get(0, conf);
   
   clientMQTT.publish(conf.broker_topic ,data);
}

boolean isMqttConnected() {
  return clientMQTT.connected();
}

