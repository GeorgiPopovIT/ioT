#include <ArduinoMqttClient.h>
#include <WiFi.h>
// define led according to pin diagram
int led1 = D10;
int led2 = D9;
int led3 = D8;

const char* ssid = "Popovi-5GHz";
const char* password = "42b4632c";    // your network password (use for WPA, or use as key for WEP)


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//MQTT broker address
const char  mqtt_broker[]   = "mqtt3.thingspeak.com";
int         mqtt_port       = 1883;

//MQTT credentials
const char  mqtt_id[]       = "<INSERT_YOUR_ID>";
const char  mqtt_username[] = "<INSERT_YOUR_USERNAME>";
const char  mqtt_pwd[]      = "<INSERT_YOUR_PASSWORD>";

//MQTT topics
const char  topic_temp[]    = "channels/<INSERT_YOUR_CHANNEL_ID>/publish/fields/field1";
const char  topic_hum[]     = "channels/<INSERT_YOUR_CHANNEL_ID>/publish/fields/field2";


void setup() {

  Serial.begin(115200); // open serial port, set the baud rate as 9600 bps
  
  // ADC
  pinMode(A0, INPUT);

  // initialize digital pin led as an output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

  Serial.println("You're connected to the network");
  Serial.println(WiFi.localIP());
 
 if (!mqttClient.connect(mqtt_broker, mqtt_port)) {
  Serial.print("MQTT connection failed! Error code = ");
  Serial.println(mqttClient.connectError());
  }
 Serial.println("You're connected to the MQTT broker!");
}

int calculatePercentage(int start, int end, int current) {
  // Return 0% if the value is out of range
  if (end > current) {
    return 0;
  }

  // Calculate the total range
  double totalRange = end - start;
  
  // Calculate the current position within the range
  double currentPosition = current - start;
  
  // Calculate the percentage and round it to the nearest integer
  int percentage = static_cast<int>((currentPosition / totalRange) * 100.0 + 0.5);
  
  // Ensure the percentage is within [0, 100]
  if (percentage < 0) {
      return 0;
  } else if (percentage > 100) {
      return 100;
  } else {
      return percentage;
  }
}

int printSoilMoisture(uint8_t id, uint8_t pin) {
  uint32_t soil = analogReadMilliVolts(pin);
  int percentage = calculatePercentage(2250, 1050, soil);
  Serial.print("Soil moisture ");
  Serial.print(id);
  Serial.print(": ");
  Serial.print(percentage);
  Serial.println("%");

  return percentage;

}

void loop() {

  int percentage = printSoilMoisture(1,A0);
  if(percentage <= 10){
    digitalWrite(led1, HIGH);
       digitalWrite(led2, LOW);
  }
  else if( percentage > 10 && percentage <= 70){
   digitalWrite(led2, HIGH);
   digitalWrite(led1, LOW);
  }
  else{
    digitalWrite(led3, HIGH);
    digitalWrite(led1, LOW);
   digitalWrite(led2, LOW);

  }
delay(500); 

  mqttClient.poll();

 //Send temperature data to the respective topic
 mqttClient.beginMessage(topic_temp);
 mqttClient.print(temp_hum_val[1]);
 mqttClient.endMessage();
 //Send humidity data to the respective topic
 mqttClient.beginMessage(topic_hum);
 mqttClient.print(temp_hum_val[0]);
 mqttClient.endMessage();
}
