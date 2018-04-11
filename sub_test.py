#------------------------------------------
#--- Author: Pradeep Singh
#--- Date: 20th January 2017
#--- Version: 1.0
#--- Python Ver: 2.7
#--- Details At: https://iotbytes.wordpress.com/store-mqtt-data-from-sensors-into-sql-database/
#------------------------------------------

import paho.mqtt.client as mqtt
from store_Sensor_Data_to_DB import sensor_Data_Handler

import random, threading, json
from datetime import datetime

# MQTT Settings 
MQTT_Broker = "192.168.7.1"
pub_MQTT_Broker = "localhost"
MQTT_Port = 1883
Keep_Alive_Interval = 45
MQTT_Topic = "dht11/temperature"




#Subscribe to all Sensors at Base Topic
def on_connect(self, mosq, obj, rc):
	mqttc.subscribe(MQTT_Topic, 0)

def pub_on_connect(client, userdata, rc):
	if rc != 0:
		pass
		print "Unable to connect to MQTT Broker..."
	else:
		print "Connected with MQTT Broker: " + str(MQTT_Broker)

#Save Data into DB Table
def on_message( mosq, obj, msg):
	# This is the Master Call for saving MQTT Data into DB
	# For details of "sensor_Data_Handler" function please refer "sensor_data_to_db.py"
	print "MQTT Data Received..."
	print "MQTT Topic: " + msg.topic  
	print "Data: " + msg.payload
	
	publish_Sensor_Values_to_MQTT(msg.payload)
	#sensor_Data_Handler(msg.topic, msg.payload)

def on_subscribe( mosq, obj, mid, granted_qos):
    pass

def on_publish(client, userdata, mid):
	pass
		
def on_disconnect(client, userdata, rc):
	if rc !=0:
		pass

def publish_To_Topic(topic, message):
	mqttp.publish(topic,message)
	print ("Published: " + str(message) + " " + "on MQTT Topic: " + str(topic))
	print ""



def publish_Sensor_Values_to_MQTT(temp_msg):
	print "Publishing Temperature Value: " + str(temp_msg) + "..."		
	publish_To_Topic (MQTT_Topic, temp_msg)

mqttc = mqtt.Client()

# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe

# Connect
mqttc.connect(MQTT_Broker, int(MQTT_Port), int(Keep_Alive_Interval))


mqttp = mqtt.Client()
mqttp.on_connect = pub_on_connect
mqttp.on_disconnect = on_disconnect
mqttp.on_publish = on_publish

mqttp.connect(pub_MQTT_Broker, int(MQTT_Port), int(Keep_Alive_Interval))





# Continue the network loop
mqttc.loop_forever()
#mqttp.loop_forever()
