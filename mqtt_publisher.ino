#define DHTPIN 4     // what digital pin we’re connected to
#define DHTTYPE DHT11   // DHT 11
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "murray_server";
const char* password = "77777777";
const char* mqtt_server = "192.168.0.111";

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastSend;



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(2,HIGH);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      digitalWrite(2,HIGH);
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      digitalWrite(2,LOW);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(4, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(2, OUTPUT);     // wifi connection test: red
    

  digitalWrite(2, LOW);
  Serial.begin(115200);
  Serial.println("DHT11 test!");
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  lastSend = 0;
}

void loop() {
  delay(10000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();



  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
  Serial.println("Failed to read from DHT sensor!");
  return;
  
  }
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");

  String temperature = String(t);
  String humidity = String(h);

  if (!client.connected()) {
    digitalWrite(2,LOW);
    reconnect();
  }
  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    String payload1 = "{";
    payload1 += "\"temperature\":";
    payload1 += temperature;
    payload1 += "}";

    String payload2 = "{";
    payload2 += "\"humidity\":";
    payload2 += humidity;
    payload2 += "}";
    
    char attributes1[100];
    char attributes2[100];
    payload1.toCharArray( attributes1, 100 );
    payload2.toCharArray( attributes2, 100 );
    
    Serial.print("Publish message: ");
    Serial.println(attributes1);
    Serial.println(attributes2);
    
    client.publish("dht11/temperature", attributes1);
    client.publish("dht11/humidity", attributes2);
    lastSend = millis();
  }
  client.loop();

 
}
