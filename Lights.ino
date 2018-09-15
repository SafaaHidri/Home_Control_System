#include <PubSubClient.h>
#include<WiFi101.h>
#include<Servo.h>

//int pin_servo = 7;
const int ledpin = 7; 
const char* ssid = "safa";
const char* wifi_password = "wifiwifi";
const char* mqtt_server = "10.42.0.2";
const char* mqtt_topic = "/chambre1/lampe1";
const char* mqtt_topicporte = "/maison/porte";
const char* clientID = "mkr1000";



// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
Servo myservo; 
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void callback(char* mqtt_topic, byte* payload, unsigned int length) {

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 'f') {
    digitalWrite(ledpin, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else if ((char)payload[0] == 't'){
    digitalWrite(ledpin, HIGH);  // Turn the LED off by making the voltage HIGH
  }
 /* else if ((char)payload[0] == 'o'||(char)payload[0] == 'O') {
       myservo.write(+90); 
  } else if ((char)payload[0] == 'c'||(char)payload[0] == 'C'){
     myservo.write(-90); 
  }*/

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
 
    
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      client.subscribe(mqtt_topicporte);
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
 // myservo.attach(pin_servo);
  pinMode(ledpin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  WiFi.begin(ssid, wifi_password);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
