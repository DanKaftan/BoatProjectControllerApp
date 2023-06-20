#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <stdio.h>
#include <stdlib.h>
#include <ESP32Servo.h>

Servo myServo;

// Replace with your network credentials
const char* ssid = "Shashon";
const char* password = "uzpv4018";

// Create an instance of the web server
AsyncWebServer server(80);

// Motor pins
const int motorPin1 = 27;
const int motorPin2 = 26;
int servoDegree;
int servoPin = 25;

// API endpoint path
const char* apiEndpoint = "/motor";
//Servo myservo;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set motor pins as output
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  myServo.attach(18);
  // Route for motor control
  server.on(apiEndpoint, HTTP_POST, [](AsyncWebServerRequest *request){
    String motorState = request->getParam("state")->value();
    if (motorState == "forward") {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      Serial.println("MOTOR FORWARD");
    } else if (motorState == "stop") {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      Serial.println("MOTOR STOP");
    } else if (motorState == "backward") {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      Serial.println("MOTOR BACKWARD");
    } else {
      //int num = atoi(motorState);
      int degree = motorState.toInt();
      myServo.write(degree);  
      Serial.println("write to motor " + motorState + " degrees");
    }
    request->send(200, "text/plain", "Motor state: " + motorState);
  });

  // servo control
  server.on("/boat", HTTP_GET, [](AsyncWebServerRequest* request) {
    String servoParam = request->getParam("servo_degree")->value();
    Serial.println("Servo degree: " + servoParam);

    // Parse the servo parameter
    int newServoDegree = servoParam.toInt();

    // Update the servo degree only if a valid value is received
    if (newServoDegree >= 0 && newServoDegree <= 180) {
      servoDegree = newServoDegree;

      // Control the servo
      int servoValue = map(servoDegree, 0, 180, 0, 180);  // Map the servo degree to servo values
      analogWrite(servoPin, servoValue);

      // Send a response back to the client
      request->send(200, "text/plain", "Servo degree updated");
    } else {
      // Send an error response back to the client
      request->send(400, "text/plain", "Invalid servo degree");
    }
  });


  // Start the server
  server.begin();
}

void loop() {
  // Nothing to do here
}

