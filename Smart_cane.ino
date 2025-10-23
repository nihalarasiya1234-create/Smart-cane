#include <SoftwareSerial.h>
#include <TinyGPS++.h>
// Ultrasonic Sensor and Buzzer Pins
const int trigPin1 = 9;
const int echoPin1 = 10;
const int buzzer1 = 11;
const int trigPin2 = 12;
const int echoPin2 = 8;
const int buzzer2 = 7;
const int vibrationMotor = 6;
// GPS Pins
const int GPS_RX = 3;
const int GPS_TX = 4;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;
long duration1, duration2;
int distance1, distance2;
const int minDistance = 10; // Ignore anything less than 10 cm
const int obstacleThreshold = 50;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(vibrationMotor, OUTPUT);
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
   // Measure distance from sensor 1
 digitalWrite(trigPin1, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin1, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH, 30000);
distance1 = duration1 * 0.034 / 2;
 // Measure distance from sensor 2
 digitalWrite(trigPin2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH, 30000);
 distance2 = duration2 * 0.034 / 2;
 bool obstacleDetected = false;
 // Sensor 1 logic
 if (distance1 >= minDistance && distance1 < obstacleThreshold) {
 digitalWrite(buzzer1, HIGH);
 Serial.print("Sensor 1: ");
 Serial.print(distance1);
 Serial.println(" cm -> Obstacle detected");
 obstacleDetected = true;
 } else {
 digitalWrite(buzzer1, LOW);
 }
 // Sensor 2 logic
if (distance2 >= minDistance && distance2 < obstacleThreshold) {
 digitalWrite(buzzer2, HIGH);
 Serial.print("Sensor 2: ");
 Serial.print(distance2);
 Serial.println(" cm -> Obstacle detected");
 obstacleDetected = true;
 } else {
 digitalWrite(buzzer2, LOW);
 }
 // Modified: Vibration motor control
 if (obstacleDetected) {
 digitalWrite(vibrationMotor, HIGH); // Obstacle found -> motor ON
 } else {
 digitalWrite(vibrationMotor, LOW); // No obstacle -> motor OFF
 }
 // GPS data reading
 while (gpsSerial.available() > 0) {
 gps.encode(gpsSerial.read());
 }
 // Display location if available
 if (gps.location.isUpdated()) {
 Serial.print("Current Location -> Latitude: ");
Serial.print(gps.location.lat(), 6);
 Serial.print(", Longitude: ");
 Serial.println(gps.location.lng(), 6);
 } else {
 Serial.println("GPS signal lost or not acquired.");
 }
 delay(100); // Slight delay to reduce sensitivity and processing load
}


