#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <string.h>

//String para impressoes de palavras via serial. Acho que poderá ser excluída
String valor;

int api_call;

//Const to CI manager (74hc595) ATENCAO as portas do ESP32 ou arduino
const int bits = 13; 
const int storage = 12;
const int shift = 14;

//Const to connection with network
const char *SSID = "brisa-149191";
const char *PWD = "u8twfbnk";
 
// Web server running on port 80
WebServer server(80);
 
// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];
 
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (i > 10){
      off_sequence();
    }
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
    i++;
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}
void setup_routing() {	 	 
  server.on("/temperature", getTemperature);	 	 
  //server.on("/pressure", getPressure);	 	 
  //server.on("/humidity", getHumidity);	 	 
  //server.on("/env", getEnv);	 	 
  server.on("/led", HTTP_POST, handlePost);	 	 
  	 	 
  // start server	 	 
  server.begin();	 	 
}
 
void create_json(char *tag, String value, char *unit) {  
  jsonDocument.clear();  
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}
 
void add_json_object(char *tag, String value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit; 
}

void cross0_sequence(String status0, String status1, String status2, String status3) {

  //Como é um empurrador de bits, devemos começar pelo fim para que quem utiliza trabalhe de forma normal
  //Perceba que a sequencia 1100 será empurrada 1 -> 1-> -> 0 -> 0 o que resulta em 0011

  //cruzamento 3
  for (int j = 0; j < 3; j++) {
    if (status3[j] == '1') {
      digitalWrite(bits, HIGH);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);

    } else if (status3[j] == '0') {
      digitalWrite(bits, LOW);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);
    }
    digitalWrite(storage, HIGH);
    digitalWrite(storage, LOW);
  }

  //Cruzamento 2
  for (int j = 0; j < 3; j++) {
    if (status2[j] == '1') {
      digitalWrite(bits, HIGH);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);

    } else if (status2[j] == '0') {
      digitalWrite(bits, LOW);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);
    }
    digitalWrite(storage, HIGH);
    digitalWrite(storage, LOW);
  }

  //Cruzamento 1
  for (int j = 0; j < 3; j++) {
    if (status1[j] == '1') {
      digitalWrite(bits, HIGH);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);

    } else if (status1[j] == '0') {
      digitalWrite(bits, LOW);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);
    }
    digitalWrite(storage, HIGH);
    digitalWrite(storage, LOW);
  }

  //Cruzamento 0
  for (int j = 0; j < 3; j++) {
    if (status0[j] == '1') {
      digitalWrite(bits, HIGH);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);

    } else if (status0[j] == '0') {
      digitalWrite(bits, LOW);
      digitalWrite(shift, HIGH);
      digitalWrite(shift, LOW);
    }
    digitalWrite(storage, HIGH);
    digitalWrite(storage, LOW);
  }

  digitalWrite(storage, HIGH);
  digitalWrite(storage, LOW);

}

void getTemperature() {
  Serial.println("Get temperature");
  create_json("temperature", "152", "°C");
  server.send(200, "application/json", buffer);
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  // Get RGB components
  String rua0 = jsonDocument["rua0"];
  String rua1 = jsonDocument["rua1"];
  String rua2 = jsonDocument["rua2"];
  String rua3 = jsonDocument["rua3"];
  Serial.print("status do cruzamento: ");
  Serial.print(rua0);
  Serial.print(rua1);
  Serial.print(rua2);
  Serial.println(rua3);

  api_call = 1;
  cross0_sequence(rua0, rua1, rua2, rua3);

  rua0.concat(rua1);
  rua0.concat(rua2);
  rua0.concat(rua3);
  Serial.println(rua0);
  
  create_json("status", rua0 , "aceso");
  server.send(200, "application/json", buffer);
}

void off_sequence(){
  //Adotamos para o amarelo apenas metade do tempo das outras luzes
  //Isso porque na realidade o amarelo sempre dura menos
  //-> 100 == RED -> 010 == YELLOW 001 -> GREEN +++ (sign 0, sign 1, sign 2, sign 3, status time)
  int time = 1000;
  int i = 0;
  for (int j=0;j<4;j++){
    switch (i){
      case 0:
        cross0_sequence("001", "100", "100", "100");
        delay(time);
        cross0_sequence("010", "100", "100", "100");
        delay(time/(1.5));
        break;
      case 1:
        cross0_sequence("100", "001", "100", "100");
        delay(time);
        cross0_sequence("100", "010", "100", "100");
        delay(time/(1.5));
        break;
      case 2:
        cross0_sequence("100", "100", "001", "100");
        delay(time);
        cross0_sequence("100", "100", "010", "100");
        delay(time/(1.5));
      case 3:
        cross0_sequence("100", "100", "100", "001");
        delay(time);
        cross0_sequence("100", "100", "100", "010");
        delay(time/(1.5));
        break;
    }
    if (i == 3){
      i = 0;
    } else {
      i++;
    }
  }
}

void setup() {
  pinMode(bits, OUTPUT);
  pinMode(storage, OUTPUT);
  pinMode(shift, OUTPUT);	 	
  Serial.begin(9600);	 	 
  connectToWiFi();	 	 
  setup_routing();
  cross0_sequence("100", "100", "100", "100");
}	 	 
  	 	 
void loop() {	 	 
  server.handleClient(); 	 
}