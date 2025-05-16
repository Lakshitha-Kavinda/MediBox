#include <PubSubClient.h>
#include <WiFi.h>

//initiates WiFi client and pubsub client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setupWifi();
void setupMqtt();
void connectToBroker();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupWifi();
  setupMqtt();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!mqttClient.connected()) {
    connectToBroker();
  }

//to check the available msgs
  mqttClient.loop();

//pusblishing temperature values to the topic
  mqttClient.publish("ENTC-TEMP", "25.24");
  delay(1000);

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
  mqttClient.setServer("broker.hivemq.com", 1883);
}

void connectToBroker(){
  while (!mqttClient.connected()){
    Serial.print("Attempting MQTT connection ");
    if(mqttClient.connect("ESP32")){
      Serial.println("connected");
    }else{
      Serial.print("failed");
      Serial.print(mqttClient.state());
      delay(5000);
    }
  }
}

