#include <WiFi.h>
#include <esp_now.h>

// Receiver ESP32 MAC address
const uint8_t broadcastAddress[] = {0x30, 0xC9, 0x22, 0x28, 0x56, 0x24};
esp_now_peer_info_t peerInfo;

// Packet structure
typedef struct dataPacket {
  float pitch;
  float roll;
} dataPacket;

dataPacket data;

// Callback when data is sent
void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("Last packet status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivered successfully" : "Failed to deliver");
}

void initializeCommunication() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Could not initialize ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(onDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void setup() {
  Serial.begin(115200);
  initializeCommunication();
}

void loop() {
  getRotation();

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
  if (result == ESP_OK) {
    Serial.println("Sending verified");
  }

  delay(500);
}

void getRotation() {
  data.pitch = 0;
  data.roll = 0;
}