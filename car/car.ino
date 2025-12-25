#include <WiFi.h>
#include <esp_now.h>

// Connections
const int motor1pin1 = 16;
const int motor1pin2 = 17;
const int motor2pin1 = 18;
const int motor2pin2 = 19;

// Setting PWM properties
const int pwmChannel = 0;

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
  setMotorsRight(data.pitch/70);
  setMotorsLeft(data.pitch/70);
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

void initializeMotors() {
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
}

void setMotorsRight(float power) {
  if (power < 0.4 && power > -0.4) 
  {
    analogWrite(motor1pin1, 0);
    analogWrite(motor1pin2, 0);
    return;
  }
  if (power > 1) power = 1;
  if (power < -1) power = -1; 

  analogWrite(motor1pin1, power > 0 ? power * 255 : 0);
  analogWrite(motor1pin2, power < 0 ? power * -255 : 0);
}

void setMotorsLeft(float power) {
  if (power < 0.4 && power > -0.4) 
  {
    analogWrite(motor2pin1, 0);
    analogWrite(motor2pin2, 0);
    return;
  }
  if (power > 1) power = 1;
  if (power < -1) power = -1; 

  analogWrite(motor2pin1, power > 0 ? power * 255 : 0);
  analogWrite(motor2pin2, power < 0 ? power * -255 : 0);
}
