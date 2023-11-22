#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "WiFi.h"
#include "MQ135.h"
#include "DHT.h"
#define DHT11PIN 33
#define MQ135PIN 19
#define LED_RED 32
#define LED_GREEN 27
#define LED_YELLOW 26
#define BUZZER 34 


float humidity;
float temperature;

const char* ssid = "CAMILO";
const char* password = "9XKUQ8THKZHYFZJ";

// const int buzzer = 26;

AsyncWebServer server(80);

DHT dht(DHT11PIN, DHT11);
MQ135 mq135_sensor(MQ135PIN);


void wifi_setup() {
  WiFi.begin(ssid, password);

  Serial.println(WiFi.localIP());
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());

}

void mq_135() {
  float local_temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
  float local_humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(local_temperature, local_humidity );
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(local_temperature, local_humidity );

  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");

  delay(300);
}

void dht_sensor() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) { humidity=0; temperature=0; }
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print(" %");
  delay(1000);
}

 String read_dht_temperature(){
  temperature = dht.readTemperature();
  return String(humidity);
 }

 String read_mq_ppm(){
  float ppm = mq135_sensor.getPPM();
  return String(ppm);
 }

  String read_dht_humidity(){
  humidity = dht.readHumidity();
  return String(humidity);
 }

void server_setup(){
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read_dht_temperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read_dht_humidity().c_str());
  });
  server.on("/ppm", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read_mq_ppm().c_str());
  });

  server.begin();
}

void setup() {
  // Serial.begin(9600);
  // Serial port for debugging purposes
  Serial.begin(115200);
  // put your instructions here, to run once:
  wifi_setup();
  server_setup();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
    dht_sensor();
    mq_135();
    
  digitalWrite(LED_RED, HIGH); // turn the LED on
  delay(500);             // wait for 500 milliseconds
  digitalWrite(LED_RED, LOW);  // turn the LED off
  delay(500);
  delay(1000);

   digitalWrite(LED_GREEN, HIGH); // turn the LED on
  delay(500);             // wait for 500 milliseconds
  digitalWrite(LED_GREEN, LOW);  // turn the LED off
  delay(500);
  //delay(1000);

  digitalWrite(LED_YELLOW, HIGH); // turn the LED on
  delay(500);             // wait for 500 milliseconds
  digitalWrite(LED_YELLOW, LOW);  // turn the LED off
  delay(500);
  //delay(1000);
  
  tone(BUZZER, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(BUZZER);     // Stop sound...
  //delay(1000);  
}
