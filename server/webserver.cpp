#include "webserver.h"
#include "config.h"
#include "mqtt.h"
#include "network.h"
#include "index.html.gz.h"
#include <EEPROM.h>

ESP8266WebServer server(80);

void handleRoot() {
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
}

void handleConfig() {
  config_t antConf;
  config_t newConf;

  EEPROM.get(0, antConf);

  strcpy(newConf.essid, antConf.essid);
  strcpy(newConf.pass, antConf.pass);
   
  //server.arg("essid").toCharArray(newConf.essid, sizeof(newConf.essid));
  //server.arg("pass").toCharArray(newConf.pass, sizeof(newConf.pass));
  server.arg("mqttServer").toCharArray(newConf.broker_add, sizeof(newConf.broker_add));
  newConf.broker_puerto = server.arg("mqttPort").toInt();
  server.arg("mqttPass").toCharArray(newConf.broker_pass, sizeof(newConf.broker_pass));
  server.arg("mqttUser").toCharArray(newConf.broker_user, sizeof(newConf.broker_user));
  server.arg("clientId").toCharArray(newConf.clientID, sizeof(newConf.clientID));
  server.arg("mqttTopic").toCharArray(newConf.broker_topic, sizeof(newConf.broker_topic));

  Serial.println("SSID");
  Serial.println(newConf.essid);
  Serial.println("BROKER");
  Serial.println(newConf.broker_add);
  Serial.println("PORT");
  Serial.println(newConf.broker_puerto);
  Serial.println("TOPIC");
  Serial.println(newConf.broker_topic);
  Serial.println("USER");
  Serial.println(newConf.broker_user);
  Serial.println("PASS");
  Serial.println(newConf.broker_pass);

  //EEPROM.begin(sizeof(config_t));
  EEPROM.put(0, newConf);
  EEPROM.commit();  

  connectMQTT();

  server.sendHeader("Location", String("/?status=OK"), true);
  server.send ( 302, "text/plain", "");
  
  //server.sendHeader("Content-Encoding", "gzip");
  //server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
}

void handleConfigNetwork() {
  config_t antConf;
  config_t newConf;

  EEPROM.get(0, antConf);

  strcpy(newConf.broker_add, antConf.broker_add);
  newConf.broker_puerto = antConf.broker_puerto;
  
  strcpy(newConf.broker_pass, antConf.broker_pass);
  strcpy(newConf.broker_user, antConf.broker_user);
  
  strcpy(newConf.clientID, antConf.clientID);
  strcpy(newConf.broker_topic, antConf.broker_topic);
   
  server.arg("essid").toCharArray(newConf.essid, sizeof(newConf.essid));
  server.arg("pass").toCharArray(newConf.pass, sizeof(newConf.pass));
  
  /*server.arg("mqttServer").toCharArray(newConf.broker_add, sizeof(newConf.broker_add));
  newConf.broker_puerto = server.arg("mqttPort").toInt();
  server.arg("mqttPass").toCharArray(newConf.broker_pass, sizeof(newConf.broker_pass));
  server.arg("mqttUser").toCharArray(newConf.broker_user, sizeof(newConf.broker_user));
  server.arg("clientId").toCharArray(newConf.clientID, sizeof(newConf.clientID));
  server.arg("mqttTopic").toCharArray(newConf.broker_topic, sizeof(newConf.broker_topic));*/

  Serial.println("SSID");
  Serial.println(newConf.essid);
   Serial.println("PASSWORD");
  Serial.println(newConf.pass);
  Serial.println("BROKER");
  Serial.println(newConf.broker_add);
  Serial.println("PORT");
  Serial.println(newConf.broker_puerto);
  Serial.println("TOPIC");
  Serial.println(newConf.broker_topic);
  Serial.println("USER");
  Serial.println(newConf.broker_user);
  Serial.println("PASS");
  Serial.println(newConf.broker_pass);

  //EEPROM.begin(sizeof(config_t));
  EEPROM.put(0, newConf);
  EEPROM.commit();  

  connectNetwork();
  connectMQTT();
  
  server.sendHeader("Location", String("/?status=OK"), true);
  server.send ( 302, "text/plain", "");
  
}

void handleNotFound() {
  String message = "File Not Found\n\nURI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleGetData() {
   config_t conf;
   
   EEPROM.get(0, conf);
  
  String ret = "{ ";
  
  ret += "\"essid\" : \"";
  ret += conf.essid;
  ret += "\", ";

  ret += "\"pass\" : \"";
  ret += conf.pass;
  ret += "\", ";

  ret += "\"brokerAdd\" : \"";
  ret += conf.broker_add;
  ret += "\", ";

  ret += "\"brokerPort\" : \"";
  ret += conf.broker_puerto;
  ret += "\", ";

  ret += "\"brokerUser\" : \"";
  ret += conf.broker_user;
  ret += "\", ";

  ret += "\"brokerPass\" : \"";
  ret += conf.broker_pass;
  ret += "\", ";

  ret += "\"brokerTopic\" : \"";
  ret += conf.broker_topic;
  ret += "\", ";

  ret += "\"clientId\" : \"";
  ret += conf.clientID;
  ret += "\"";
  ret += " }";

  Serial.println(ret);
  server.send(200, "application/json", ret);
}

void handleGetNetworks() {
   int n = WiFi.scanNetworks();

    String ret = "[";
    
   for (int i = 0; i < n; ++i) {

      float signalLevel = WiFi.RSSI(i);
      signalLevel = isnan(signalLevel) ? -100.0 : signalLevel;
      signalLevel = min(max(2 * (signalLevel + 100.0), 0.0), 100.0);
      
      ret += "{ ";
      ret += "\"ESSID\" : ";
      
      ret += "\"";
      ret += WiFi.SSID(i);
      ret += "\",";

      ret += "\"RSSI\" : ";
      
      ret += "\"";
      ret += WiFi.RSSI(i);
      ret += "\",";

      ret += "\"SIGNAL_LEVEL\" : ";
      
      ret += "\"";
      ret += signalLevel;
      ret += "\",";

      ret += "\"ENCRYPTION\" : ";
      
      ret += "\"";
      ret += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? false : true;
      ret += "\" },";

   }

    ret = ret.substring(0, ret.length() - 1);
   ret += "]";

  Serial.println(ret);
  server.send(200, "application/json", ret);
}

void handleGetMqttStatus() {

  String ret = "";
 
 if (isMqttConnected()) {
  ret = "{\"CONNECTED\" : true}";
 }
 else {
  ret = "{\"CONNECTED\" : false}";
 }
  Serial.println(ret);
  server.send(200, "application/json", ret);
}

void handleGetSensorData() {
  String ret = "";
 
 if (digitalRead(13)) {
  ret = "{\"SENSOR\" : true}";
 }
 else {
  ret = "{\"SENSOR\" : false}";
 }
  Serial.println(ret);
  server.send(200, "application/json", ret);
}


void WebServer_init() {
  server.on("/", handleRoot);
  server.on("/configMqtt", handleConfig);
  server.on("/configNetwork", handleConfigNetwork);
  server.on("/getData", handleGetData);
  server.on("/getMqttStatus", handleGetMqttStatus);
  server.on("/getSensorData", handleGetSensorData);
  server.on("/getNetworks", handleGetNetworks);
  server.onNotFound(handleNotFound);
  server.begin();

    Serial.println("HTTP server started");
}

void WebServer_stop() {
  server.stop();
}

void WebServer_loop() {
  server.handleClient();
}
