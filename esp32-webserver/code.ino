#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "******************";
const char* password = "******************";

WebServer server(80);

void getIndex() {
  /* Create index page */
  String page = 
  "<!DOCTYPE html><html><head><title>Home page of ESP32 CAM</title></head><body>";
  page += "<h1>ESP32</h1>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void captureImage() {
  /* Create route to capture image */
  return;
}

void receiveMessage() {
  /* Create route to get message */
  String message;
}

void setupRouting() {
  /* Create routes of webserver */
  server.on("/", HTTP_GET, getIndex);
  server.on("/capture", HTTP_GET, captureImage);
  server.on("/receive", HTTP_POST, receiveMessage);
  return;
}

void setup() {
  // Start serial
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  // Keep retrying until the WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // Show IP Address in serial monitor
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebServer
  setupRouting();
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
