#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "DHT.h"

// --- SENSOR CONFIG ---
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

// --- NETWORK AND THINGSPEAK DATA ---
const String ssid = "WIFI SSID";
const String password = "WIFI PASSWORD";
const String apiKey = "API KEY";
const String host = "api.thingspeak.com";

// --- 7-SEGMENT DISPLAY PIN LAYOUT ---
const int segA = 5;
const int segB = 6;
const int segC = 7;
const int segD = 8;
const int segE = 9;
const int segF = 10;
const int segG = 11;

// --- DISPLAY FUNCTION ---
void showstate(char state) {
  // ALL SEGMENTS OFF
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segC, LOW);
  digitalWrite(segD, LOW); digitalWrite(segE, LOW); digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);

  // Launching designated patterns
  if(state == '0') {
    digitalWrite(segA, HIGH); digitalWrite(segB, HIGH); digitalWrite(segC, HIGH);
    digitalWrite(segD, HIGH); digitalWrite(segE, HIGH); digitalWrite(segF, HIGH);
  }
  else if(state == '1') {
    digitalWrite(segB, HIGH); digitalWrite(segC, HIGH);
  }
  else if(state == '2') {
    digitalWrite(segA, HIGH); digitalWrite(segB, HIGH); digitalWrite(segD, HIGH);
    digitalWrite(segE, HIGH); digitalWrite(segG, HIGH);
  }
  else if(state == '3') {
    digitalWrite(segA, HIGH); digitalWrite(segB, HIGH); digitalWrite(segC, HIGH);
    digitalWrite(segD, HIGH); digitalWrite(segG, HIGH);
  }
  else if(state == 'E') { // General Error pattern
    digitalWrite(segA, HIGH); digitalWrite(segD, HIGH); digitalWrite(segE, HIGH);
    digitalWrite(segF, HIGH); digitalWrite(segG, HIGH);
  }
  else if(state == 'b') { // BMP Error (small 'b')
    digitalWrite(segC, HIGH); digitalWrite(segD, HIGH); digitalWrite(segE, HIGH);
    digitalWrite(segF, HIGH); digitalWrite(segG, HIGH);
  }
  else if(state == 'd') { // DHT Errod (small 'd')
    digitalWrite(segB, HIGH); digitalWrite(segC, HIGH); digitalWrite(segD, HIGH);
    digitalWrite(segE, HIGH); digitalWrite(segG, HIGH);
  }
}

void setup() {
  pinMode(segA, OUTPUT); pinMode(segB, OUTPUT); pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT); pinMode(segE, OUTPUT); pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  // STATE 0: Startup
  showstate('0');
  
  Serial.begin(115200);
  Serial.println("Sensor startup");
  dht.begin();
  
  Serial.println("BMP180 check");
  // BMP180 check on startup
  // This check is important if you have a non-soldered BMP180 sensor as ensuring proper connection with is tends to be problematic.
  if (!bmp.begin()) { 
    showstate('b'); // display 'b' (BMP180 error)
    while (1) {
      delay(10); // Hang the program - BMP sensor not responding
    }
  }
  Serial.println("BMP180 check successful");
  
  delay(2000); // Warmup

  // STATE 1: Connecting to Wi-Fi
  Serial.println("AT+CWMODE=1");
  delay(1000);
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  
  for(int i = 0; i < 14; i++) {
    showstate('1');
    delay(250);
    showstate('X');
    delay(250);
  }
  
  // STATE 2: Connected / Idle
  showstate('2');
}

void loop() {
  // Data readout
  float t = (bmp.readTemperature() + dht.readTemperature())/2; 
  float h = dht.readHumidity();
  int air = analogRead(A0);
  int press = bmp.readPressure() / 100;

  // --- Additional Error analysis ---
  
  // 1. DHT22 Check
  if (isnan(t) || isnan(h)) {
    showstate('d'); // display 'd' (DHT Error)
    delay(5000);
    showstate('2');
    return; // Interrupt and try again
  }

  // 2. BMP180 Check
  if (press <= 0) {
    showstate('b'); // display 'b' (BMP Error)
    delay(5000);
    showstate('2');
    return; // Interrupt and try again
  }

  // STATE 3: Sending data to ThingSpeak
  showstate('3');
  
  String url = "/update?api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(air) + "&field4=" + String(press);
  String httpRequest = "GET " + url + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";

  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\",80");
  delay(2000);

  Serial.print("AT+CIPSEND=");
  Serial.println(httpRequest.length());
  delay(1000);
  Serial.print(httpRequest);

  delay(2000);

  // Return to state 2
  showstate('2');
  
  delay(15000); 
}