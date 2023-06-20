#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Connect to Wi-Fi
  WiFi.begin("Shashon", "uzpv4018");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the IP address
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("API Endpoint: /motor");
}

void loop() {
  // Do nothing
}
