#define RXD1 7  // D7 on XIAO (GPIO 7)
#define TXD1 6  // D6 on XIAO (GPIO 6)

#include <Arduino.h>


// UART read buffer
String uartMessage = "";
int eegValue = 0;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000;

void setup() {
  pinMode(RXD1, INPUT);
  pinMode(TXD1, OUTPUT);

  Serial.begin(115200); // USB debug
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // UART from Feather

  delay(1000);
  Serial.println("ESP32-C3 XIAO");
}

void loop() {
  // Read and parse UART input from Feather
  while (Serial1.available()) {
    char c = Serial1.read();

    if (c == '\n') {
      uartMessage.trim(); // remove whitespace
      eegValue = uartMessage.toInt();

      Serial.print("Received EEG Value: ");
      Serial.println(eegValue);

      uartMessage = ""; // reset buffer
    } else if (c != '\r') {
      uartMessage += c;
    }
  }

  // Keep BLE stack alive
  delay(10);
}