#include <WiFi.h>
#include <WebServer.h>
#include "Base64.h"
#include "esp_camera.h"

// Camera configuration
#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

// 4 for flash led or 33 for normal led
#define LED_GPIO_NUM   4

// Webserver configuration
const char* ssid = "Bezerra";
const char* password = "juliaamordaminhavida";
WebServer server(80);

void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_RGB565;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // 1600x1200
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA; // 800x600
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void getIndex() {
  /* Create index page */
  String page = 
  "<!DOCTYPE html><html><head><title>Home page of ESP32 CAM</title></head><body>";
  page += "<h1>ESP32</h1>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}


void captureImage() {
  camera_fb_t *fb = NULL;

  // Capturar a imagem da ESP32 CAM
  fb = esp_camera_fb_get(); 
  if (!fb) {
    server.send(500, "text/plain", "Image capture failed");
    return;
  }

  // Enviar os cabeçalhos da resposta
  server.sendHeader("Content-Type", "application/octet-stream");
  server.sendHeader("Content-Disposition", "attachment; filename=capture.jpg");
  server.setContentLength(fb->len);

  // Enviar o código HTTP 200 (OK) sem corpo
  server.send(200);

  // Enviar o conteúdo em partes
  WiFiClient client = server.client();
  client.write(fb->buf, fb->len);

  // Retornar o frame buffer de volta para o driver
  esp_camera_fb_return(fb);
}

void receiveMessage() {
  /* Create route to get message */
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "No message received");
    return;
  }
  // Get POST response
  String message = server.arg("plain");  
  Serial.println(message);

  // Send HTTP response to the client
  server.send(200, "text/plain", "Message received and sent via Serial");
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

  // Camera start
  startCamera();

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
