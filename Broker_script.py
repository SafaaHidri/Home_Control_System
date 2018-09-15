
import paho.mqtt.client as mqtt
import datetime
import time

now=datetime.datetime.now()
h=now.hour+1


mqtt_topiclampe = "/chambre1/lampe"
mqtt_topictemperature = "/chambre1/temperature"
mqtt_topicporte="/maison/porte"
mqtt_topicgaz="/cuisine/gaz"
mqtt_topicmvt="/chambre1/mvt"
mqtt_topicalerte="/cuisine/alerte"
mqtt_broker_ip = "10.42.0.2"

client = mqtt.Client()


def on_connect(client, userdata, rc):
    # rc is the error code returned when connecting to the broker
    print "Connected!", str(rc)
    
    # Once the client has connected to the broker, subscribe to the topic
    client.subscribe(mqtt_topiclampe)
    client.subscribe(mqtt_topictemperature)
    client.subscribe(mqtt_topicgaz)
    client.subscribe(mqtt_topicporte)
    client.subscribe(mqtt_topicmvt)
    client.subscribe(mqtt_topicalerte)
    
def on_message(client, userdata, msg):
    
    if msg.topic == mqtt_topiclampe :
    	if msg.payload== "t" :
	        print("la lampe est allumée")
			client.publish(mqtt_topiclampe,msg.payload)
        else :
            print("la lampe est eteinte")
            client.publish(mqtt_topiclampe,msg.payload)
    else if msg.topic == mqtt_topicporte :
        if msg.payload== "o" :
            print("la porte est ouverte")
            client.publish(mqtt_topicporte,msg.payload)
        else :
        	print("la porte est fermée")
        	client.publish(mqtt_topicporte,msg.payload)
    else if msg.mqtt_topicgaz :
    	print("taux de gaz reçu")
    	client.publish(mqtt_topicgaz,msg.payload)
    else if msg.topic == mqtt_topictemperature :
    	print("valeur de temperature recu")
    	client.publish(mqtt_topictemperature,msg.payload)
    else if msg.topic == mqtt_topicalerte :
    	print("alerte lancée")
    	client.publish(mqtt_topicalerte,msg.payload)
    else if msg.topic == mqtt_topicmvt:
    	if (msg.payload=="Motion detected" && h>=18 && h=<4):
    		client.publish(mqtt_topiclampe,"t")
    	





    # The message itself is stored in the msg variable
    # and details about who sent it are stored in userdata

# Here, we are telling the client which functions are to be run
# on connecting, and on receiving a message
client.on_connect = on_connect
client.on_message = on_message

# Once everything has been set up, we can (finally) connect to the broker
# 1883 is the listener port that the MQTT broker is using
client.connect(mqtt_broker_ip, 1883)

# Once we have told the client to connect, let the client object run itself
client.loop_forever()
#client.disconnect()
