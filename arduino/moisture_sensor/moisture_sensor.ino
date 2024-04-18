#define SECRET_SSID "Popovi"
#define SECRET_PASS "1234"
// define led according to pin diagram
int led1 = D10;
int led2 = D9;
int led3 = D8;

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int port = 1883;
const char topic[]  = "real_unique_topic";
const char topic2[]  = "real_unique_topic_2";
const char topic3[]  = "real_unique_topic_3";

void setup() {

  Serial.begin(115200); // open serial port, set the baud rate as 9600 bps
  
  // ADC
  pinMode(A0, INPUT);

  // initialize digital pin led as an output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
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

  mqttClient.beginMessage(percentage);
  mqttClient.print(Rvalue);
  mqttClient.endMessage();
}
