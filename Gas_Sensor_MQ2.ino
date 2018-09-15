#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include<String.h>



const char* ssid = "safa";
const char* password =  "wifiwifi";
const char* mqttServer = "10.42.0.2";
const char* mqttTopicgaz= "/cuisine/gaz";
const char *mqttTopicalerte= "/cuisine/alerte";
const int mqttPort = 1883;
 long lastMsg = 0;
long now;
int gazPin=A1 ; //digital pin connected to the gaz sensor's output
int max_gaz= 250; //valeur au delà de laquel c'est danger
int gazvaleur=0;
char gazVal[10];
WiFiClient espClient; 
PubSubClient client(espClient);
void setup() {
    
  
  
 

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(gazPin,INPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) {
 
      Serial.println("connected");
      Serial.println();
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
}
 
void loop() {
   
gazvaleur = analogRead(gazPin); 
itoa(gazvaleur,gazVal,10);
  client.loop();
  now = millis();
if (now - lastMsg > 80000) {
    lastMsg = now;
   client.publish(mqttTopicgaz,gazVal);
 
 }
 else if (gazvaleur>max_gaz)
   {
       client.publish(mqttTopicalerte,"warning");
   }
}
 
  

