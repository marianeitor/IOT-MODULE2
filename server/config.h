typedef struct {
  // Accesspoint 
  char essid[50];
  char pass[50];

  // Broker
  char broker_add[50];
  char broker_user[50];
  char broker_pass[100];
  int broker_puerto;
  char broker_topic[100];
  char clientID[10];

  //extras
  char admin_user[50];
  char admin_password[50];
  int admin_protected;
  char has_batt;  
} config_t;
