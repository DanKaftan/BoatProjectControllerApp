#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <stdio.h>
#include <stdlib.h>
#include <ESP32Servo.h>
#include <math.h>


Servo myServo;

// Replace with your network credentials
const char* ssid = "note 17";
const char* password = "david171717";

// Create an instance of the web server
AsyncWebServer server(80);

// Motor pins
// propeller motor pins
const int propellerMotorPin1 = 27;
const int propellerMotorPin2 = 26;
const int propellerMotorEnablePinL = 35;
const int propellerMotorEnablePinR = 34;
int propellorPower = 40;



// diver motor pins
const int diverMotorPin1 = 33;
const int diverMotorPin2 = 32;
int diverPower = 100;


// anchor motor pins
const int anchorMotorPin1 = 18;
const int anchorMotorPin2 = 19;


int servoDegree;
const int servoPin = 18;


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
  pinMode(propellerMotorPin1, OUTPUT);
  pinMode(propellerMotorPin2, OUTPUT);
  pinMode(diverMotorPin1, OUTPUT);
  pinMode(diverMotorPin2, OUTPUT);
  pinMode(anchorMotorPin1, OUTPUT);
  pinMode(anchorMotorPin2, OUTPUT);
  pinMode(propellerMotorEnablePinL, OUTPUT);
  pinMode(propellerMotorEnablePinR, OUTPUT);



  myServo.attach(servoPin);

  // Route for motor control
  server.on(apiEndpoint, HTTP_POST, [](AsyncWebServerRequest *request){
    String motorState = request->getParam("state")->value();
    if (motorState == "forward") {
      digitalWrite(propellerMotorPin1, HIGH);
      digitalWrite(propellerMotorPin2, 50);
      Serial.println("MOTOR FORWARD");
    } else if (motorState == "stop") {
      digitalWrite(propellerMotorPin1, LOW);
      digitalWrite(propellerMotorPin2, LOW);
      Serial.println("MOTOR STOP");
    } else if (motorState == "backward") {
      digitalWrite(propellerMotorPin1, LOW);
      digitalWrite(propellerMotorPin2, HIGH);
      Serial.println("MOTOR BACKWARD");
    } else if (motorState == "AnchorUpPressed") { //Anchor Up
      digitalWrite(anchorMotorPin1, LOW);
      digitalWrite(anchorMotorPin2, HIGH);
      Serial.println("Anchor Motor Up");
    } else if (motorState == "AnchorDownPressed") { //Anchor Up
      digitalWrite(anchorMotorPin1, LOW);
      digitalWrite(anchorMotorPin2, HIGH);
      Serial.println("Anchor Motor Down");
    }else if (motorState == "AnchorReleased") { //Anchor Up
      digitalWrite(anchorMotorPin1, LOW);
      digitalWrite(anchorMotorPin2, LOW);
      Serial.println("Anchor Motor Down");
    }  else if (motorState == "DiveUpPressed") { //Anchor Up
      digitalWrite(diverMotorPin1, LOW);
      digitalWrite(diverMotorPin2, diverPower);
      Serial.println("Dive Motor Up");
    } else if (motorState == "DiveDownPressed") { //Anchor Up
      digitalWrite(diverMotorPin1, diverPower);
      digitalWrite(diverMotorPin2, LOW);
      Serial.println("Dive Motor Down");
    }else if (motorState == "DiveReleased") { //Anchor Up
      digitalWrite(diverMotorPin1, LOW);
      digitalWrite(diverMotorPin2, LOW);
      Serial.println("Dive Motor Down");
    } else {

     
      //int num = atoi(motorState);
      //int degree = motorState.toInt();
      float degree = motorState.toFloat();

       if (getDecimal(degree) == 3){
        propellorPower = floor(degree);
        Serial.println("propellor power: " + propellorPower);


      } else if (getDecimal(degree) == 4){
          diverPower = floor(degree);
          Serial.println("diver power: " + diverPower);

      } else {
        myServo.write(degree);  
        Serial.println("write to motor " + motorState + " degrees");
      }
      
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

float getDecimal(float num){
  return (num - floor(num)) * 10;
}

void loop() {
  // Nothing to do here
}

