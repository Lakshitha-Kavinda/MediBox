#include <PubSubClient.h>
#include <WiFi.h>

//initiates WiFi client and pubsub client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupWifi();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(mqttClient.connected()) {
    connectToBroker();
  }

}

void setupWifi() {
  WiFi.begin("Wokwi-GUEST", "");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void setupMqtt() {
  //setting up mqtt server
  mqttClient.setServer("test.mosquitto.org", 1883);
}

void connectToBroker(){
  while (!mqttClient.connected()){
    Serial.print("Attempting MQTT connection");
    mqttClient.connect();
    connectToBroker();
  }

}

