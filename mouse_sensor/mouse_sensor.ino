/**
 * @file ADNS-5020-EN_3Wire_SPI.ino
 * @brief Arduino implementation of 3-wire SPI communication with ADNS-5020-EN optical sensor.
 * 
 * This code communicates with the ADNS-5020-EN optical sensor using 3-wire SPI (SDIO, SCLK, NCS).
 * It reads motion data (Delta X/Y) and outputs it to the Serial Monitor.
 * 
 * Future plans:
 * - Port this project to ESP32 to interface with USB mice or Bluetooth for wireless operation.
 * 
 * @author Your Name
 * @date 2025-2-16
 */

#include <Arduino.h>

#define CS_PIN 10       // Chip Select (NCS)
#define SCK_PIN 13      // Clock (SCLK)
#define SDIO_PIN 11     // Serial Data I/O (MOSI/MISO combined)
#define RESET_PIN 12    // Reset pin for ADNS-5020-EN

/**
 * @brief Initializes the ADNS-5020-EN sensor.
 * 
 * This function sets up SPI communication and verifies the sensor by reading the Product ID.
 */
void setup() {
    Serial.begin(115200);

    // Configure pins
    pinMode(CS_PIN, OUTPUT);
    pinMode(SCK_PIN, OUTPUT);
    pinMode(RESET_PIN, OUTPUT);
    
    digitalWrite(CS_PIN, HIGH);  // Default HIGH (inactive)
    digitalWrite(SCK_PIN, HIGH); // SPI Mode 3: SCLK idle HIGH
    digitalWrite(RESET_PIN, HIGH);

    // Reset ADNS-5020-EN
    digitalWrite(RESET_PIN, LOW);
    delay(10);
    digitalWrite(RESET_PIN, HIGH);
    delay(50);

    // Read Product ID to verify communication
    uint8_t product_id = readRegister(0x00);
    Serial.print("Product ID: 0x");
    Serial.println(product_id, HEX);

    if (product_id != 0x12) {
        Serial.println("ERROR: Unable to recognize ADNS-5020-EN");
        while (1);  // Halt execution
    }

    Serial.println("ADNS-5020-EN initialized successfully");
}

/**
 * @brief Reads motion data from the sensor and prints it to Serial Monitor.
 */
void loop() {
    int8_t motion = readRegister(0x02);  // Check if motion is detected
    if (motion != 0) {
        int8_t deltaX = readRegister(0x03);  // Read X displacement
        int8_t deltaY = readRegister(0x04);  // Read Y displacement

        Serial.print("Delta X: ");
        Serial.print(deltaX);
        Serial.print(", Delta Y: ");
        Serial.println(deltaY);
    } else {
        delay(10);  // Short delay when no motion is detected
    }
}

/**
 * @brief Reads a register from ADNS-5020-EN via 3-wire SPI.
 * 
 * @param reg The register address to read.
 * @return The data read from the register.
 */
uint8_t readRegister(uint8_t reg) {
    uint8_t data = 0;

    digitalWrite(CS_PIN, LOW);
    delayMicroseconds(1);

    // Send register address (7-bit, MSB first)
    pinMode(SDIO_PIN, OUTPUT);
    for (int i = 7; i >= 0; i--) {
        digitalWrite(SCK_PIN, LOW);  
        digitalWrite(SDIO_PIN, ((reg & 0x7F) >> i) & 0x01); // Bit-wise transmission
        delayMicroseconds(1);
        digitalWrite(SCK_PIN, HIGH);
        delayMicroseconds(1);
    }

    delayMicroseconds(1);

    // Switch SDIO to input mode and read the response
    pinMode(SDIO_PIN, INPUT);
    for (int i = 7; i >= 0; i--) {
        digitalWrite(SCK_PIN, LOW);
        delayMicroseconds(1);
        digitalWrite(SCK_PIN, HIGH);
        data |= (digitalRead(SDIO_PIN) << i);  // Bit-wise reception
        delayMicroseconds(1);
    }

    digitalWrite(CS_PIN, HIGH);
    return data;
}

/**
 * @brief Writes a value to a register in ADNS-5020-EN via 3-wire SPI.
 * 
 * @param reg The register address to write to.
 * @param value The data to be written.
 */
void writeRegister(uint8_t reg, uint8_t value) {
    digitalWrite(CS_PIN, LOW);
    delayMicroseconds(1);

    pinMode(SDIO_PIN, OUTPUT);

    // Send register address (write operation: MSB = 1)
    for (int i = 7; i >= 0; i--) {
        digitalWrite(SCK_PIN, LOW);
        digitalWrite(SDIO_PIN, ((reg | 0x80) >> i) & 0x01);
        delayMicroseconds(1);
        digitalWrite(SCK_PIN, HIGH);
        delayMicroseconds(1);
    }

    // Send data
    for (int i = 7; i >= 0; i--) {
        digitalWrite(SCK_PIN, LOW);
        digitalWrite(SDIO_PIN, (value >> i) & 0x01);
        delayMicroseconds(1);
        digitalWrite(SCK_PIN, HIGH);
        delayMicroseconds(1);
    }

    digitalWrite(CS_PIN, HIGH);
}
