#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include<String.h>

DHTesp dht;

const char* ssid = "safa";
const char* password =  "wifiwifi";
const char* mqttServer = "10.42.0.2";
const char* mqttTopic= "/chambre1/temperature";
const int mqttPort = 1883;
 long lastMsg = 0;
 long now;
float temperature =0;
float humidity=0;
WiFiClient espClient; 
char msg[20];
char msg1[20];
PubSubClient client(espClient);
void setup() {
    
  float humidity = dht.getHumidity();
  
 

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.setup(14); // Connect DHT sensor to GPIO 14
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
      Serial.print(dht.getStatusString());
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 


}
 
void loop() {
   

  client.loop();
   now = millis();
   temperature=dht.getTemperature();
   humidity=dht.getHumidity();
   Serial.println(String(temperature));
   Serial.println(String(humidity));
   if (now - lastMsg > 80000) {
    lastMsg = now;
 if (!isnan(temperature)&&!isnan(humidity)) {
      snprintf (msg, 20, "%lf",temperature);
      snprintf (msg1, 20, "%lf",humidity);
      /* publish the message */
       client.publish(mqttTopic,msg);
       client.publish(mqttTopic,msg1);
       Serial.println(msg);
          Serial.println(msg1);
       
    }}
}

