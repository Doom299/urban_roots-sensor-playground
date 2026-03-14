#include <HardwareSerial.hpp>

// Pin Definitions based on your wiring
#define RE_DE_PIN 4
#define RX_PIN 16
#define TX_PIN 17

// Modbus RTU command to read all 7 parameters (Address 0x01, Function 03, Start 00, Length 07)
const byte readCommand[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08};

void setup() {
  Serial.begin(115200);           // PC Monitor
  Serial2.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN); // Sensor (Most use 4800 or 9600 baud)
  
  pinMode(RE_DE_PIN, OUTPUT);
  digitalWrite(RE_DE_PIN, LOW);   // Start in Receive mode
  
  Serial.println("Starting 7-in-1 Soil Sensor Test...");
}

void loop() {
  // 1. Send the Request
  digitalWrite(RE_DE_PIN, HIGH);  // Switch to Transmit
  delay(10);
  Serial2.write(readCommand, sizeof(readCommand));
  Serial2.flush();
  digitalWrite(RE_DE_PIN, LOW);   // Switch back to Receive
  
  // 2. Wait for Response
  delay(500); 
  
  if (Serial2.available()) {
    Serial.print("Response: ");
    while (Serial2.available()) {
      byte b = Serial2.read();
      if (b < 0x10) Serial.print("0");
      Serial.print(b, HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("No response. Check wiring or Baud rate.");
  }
  
  delay(3000); // Wait 3 seconds before next poll
}