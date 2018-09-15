#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <PubSubClient.h>

const char* ssid = "safa";
const char* password = "safa12348";
const char* mqttServer = "10.42.0.2";
const int mqttPort = 1883;
const char* mqtttopic ="/chambre1/mvt";

WiFiClient espClient;
PubSubClient client(espClient);

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;       
 
//the time when the sensor outputs a low impulse
long unsigned int lowIn;        
 
//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
//int pause = 5000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
int pirPin = 16;    //the digital pin connected to the PIR sensor's output

 
 

void setup(){
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
 
  digitalWrite(pirPin, LOW);
 
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);

     WiFi.begin(ssid, password);
  
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
 
void loop(){
 
     if(digitalRead(pirPin) == HIGH){
       
       client.publish(mqtttopic,"Motion detected");
         
       }
 
     if(digitalRead(pirPin) == LOW){      
     
       client.publish(mqtttopic,"Motion ended");
           }
       
  }
