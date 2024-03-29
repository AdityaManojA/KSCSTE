#include <ESP8266WiFi.h>
#define RL_VALUE 10.0 // Load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR 9.83 // Clean air resistance factor
#define GAS_AMMONIA 0 // Ammonia gas


const char* ssid     = "FTTH-C928"; 
const char* password = "anitha1090"; 


int gasPin = A0; // Analog pin for gas sensor
float Ro = 10; // Initialize Ro to a default value (will be calibrated)

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; 
  }

  Serial.print("Attempting to connect to SSID");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");

    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  


  Serial.println("MQ137 Ammonia Gas Sensor Test");
  Serial.println("Heating the sensor...");
  delay(10000); // Wait for the sensor to warm up (adjust as needed)
  Serial.println("Sensor ready!");
  Serial.println("Lets test some Meat ;)");
}

void loop() {
  float sensorValue = analogRead(gasPin);
  float sensorVoltage = sensorValue / 1023.0 * 5.0; // Convert analog value to voltage
  float RS_gas = (5.0 - sensorVoltage) / sensorVoltage * RL_VALUE; // Calculate RS in kilo ohms

  float ratio = RS_gas / Ro;
  float ammoniaConcentration = pow(10, ((log10(ratio) - 0.2042) / (-0.3268))); // Calculate ammonia concentration
  
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(", Voltage: ");
  Serial.print(sensorVoltage);
  Serial.print("V, Ammonia Concentration: ");
  Serial.print(ammoniaConcentration);
  Serial.println(" ppm");

  // Check if ammonia concentration indicates spoilage
  if (ammoniaConcentration > 30000000000) {
    Serial.println("Spoiled");
  } else {
    Serial.println("Not Spoiled");
  }

  delay(1000); // Delay between readings
}
