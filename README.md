# ADNS-5020-EN 3-Wire SPI Interface with Arduino Zero

This project interfaces an **ADNS-5020-EN optical sensor** with an **Arduino Zero** using **3-wire SPI** (SDIO, SCLK, NCS). 
It reads motion data (Delta X, Delta Y) and prints it to the serial monitor.

## Features
- Reads motion data from the **ADNS-5020-EN sensor**
- Implements **3-wire SPI bit-banging** for SDIO communication
- Compatible with **Arduino Zero**
- Plans to expand to **ESP32 for USB/Bluetooth mouse support**

## Hardware Requirements
- **Arduino Zero**
- **ADNS-5020-EN sensor**
- Jumper wires

## Wiring Guide
| ADNS-5020-EN | Arduino Zero |
|--------------|--------------|
| NCS          | **PIN10** |
| SCLK         | **PIN13** |
| SDIO         | **PIN11** |
| RESET        | **PIN12** |

| GND          | **GND** |
| VDD (5V)     | **5V** |

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/adns5020-arduino.git
   cd adns5020-arduino
