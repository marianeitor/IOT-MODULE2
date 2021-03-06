#include "network.h"
#include "config.h"
#include <EEPROM.h>
#include <DNSServer.h>

DNSServer dnsServer;              // Create the DNS object

void connectNetwork() {
  config_t conf;
  int timeout = 30;
  EEPROM.get(0, conf);

  stopAP();
  WiFi.mode(WIFI_STA);
  Serial.println("Attemping to connect to");
  Serial.println(conf.essid);

  WiFi.disconnect();
  WiFi.begin(conf.essid, conf.pass);

  while (WiFi.status() != WL_CONNECTED && timeout--) {
    delay(500);
    Serial.print(".");
  }
  if (isNetworkConnected()) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else {
      Serial.println("Imposible to connect to network");
      startAP();
  }
}

boolean isNetworkConnected() {
  if (WiFi.isConnected())
  {
    return true;
  }
  else {
    return false;
  }
}

void startDnsServer(IPAddress localIp) {
   Serial.print("Starting DNS server");
  dnsServer.start(53, "*", localIp);
}

void startAP() {
   Serial.print("Starting AP");
    WiFi.mode(WIFI_AP);
    
  IPAddress localIp(192,168,0,1);
  IPAddress subntMask(255,255,255,0);
  
   WiFi.softAPConfig(localIp, localIp, subntMask);
  WiFi.softAP("TestIOT-rmm", "");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  startDnsServer(localIp);

}
void proccessNextDnsReq() {
  dnsServer.processNextRequest();
}
void stopAP() {
 WiFi.softAPdisconnect(true);
 dnsServer.stop();
}

void factoryReset() {
  config_t conf;

   EEPROM.put(0, conf);
  EEPROM.commit(); 

  connectNetwork();
}


