#include "webserver.h"
#include "config.h"
#include "index.html.gz.h"
#include <EEPROM.h>

ESP8266WebServer server(80);

void handleRoot() {
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
}

void handleConfig() {
  config_t newConf;

  char essid[50];
  char pass[50];
  char mqttServer[50];
  char mqttPass[50];
  char clientId[50];
  char mqttTopic[100];

  Serial.println("SSID");
  Serial.println(server.arg("essid"));
  
  server.arg("essid").toCharArray(newConf.essid, sizeof(newConf.essid));
  server.arg("pass").toCharArray(newConf.pass, sizeof(newConf.pass));
  server.arg("mqttServer").toCharArray(newConf.broker_add, sizeof(newConf.broker_add));
  newConf.broker_puerto = server.arg("mqttPort").toInt();
  //server.arg("mqttPort").toCharArray(newConf.broker_puerto, sizeof(newConf.broker_puerto));
  server.arg("mqttPass").toCharArray(newConf.broker_pass, sizeof(sizeof(newConf.broker_pass)));
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

  server.send(200,"text/plain",newConf.essid);
  //server.sendHeader("Content-Encoding", "gzip");
  //server.send_P(200, "text/html", index_html_gz, index_html_gz_len);
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

void WebServer_init() {
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.on("/getData", handleGetData);
  server.onNotFound(handleNotFound);
  server.begin();
}

void WebServer_stop() {
  server.stop();
}

void WebServer_loop() {
  server.handleClient();
}
