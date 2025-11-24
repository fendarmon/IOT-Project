#include <WiFi.h>
#include <esp_now.h>


// Packet structure
typedef struct dataPacket {
  float pitch;
  float roll;
} dataPacket;

dataPacket data;

void onDataReceived(const esp_now_recv_info *info, const uint8_t *receivedData, int len) {
  memcpy(&data, receivedData, sizeof(data));

  Serial.print("Data received from: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(info->src_addr[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  Serial.print("Bytes: ");
  Serial.println(len);

  Serial.print("pitch: ");
  Serial.println(data.pitch);
  Serial.print("roll: ");
  Serial.println(data.roll);
}

void setup() {
  Serial.begin(115200);

  initializeCommunication();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void initializeCommunication() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Could not initialize ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_recv_cb(onDataReceived);
}