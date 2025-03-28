# Si4455 RF Communication Demo

This demo establishes a radio communication link between a **transmit board** and a **receive board** using the **Si4455 RF transceiver**. It enables wireless command transmission for applications such as **robot control**.

## Features

- Supports **both fixed and variable-length** packet transmission.
- Functions for checking the status of received and transmitted packets.
- Flexible radio configuration via structured settings.
- Uses **CRC-16-IBM for error detection**.

## Requirements

- **Hardware**
  - **PIC24F16KA102** *(or any compatible PIC24 microcontroller)* - development platform.
  - **Si4455 RF Transceiver** - wireless communication module.
- **Software**
  - **MPLAB X** - Integrated development environment (IDE) for PIC microcontrollers.
  - **XC16 Compiler** - C compiler for PIC24.
  - **Wireless Development Suite 3** *(Optional)* - radio configuration.

## IO Ports

| PIC24 Pin | Function | Connected Device | Description                     |
| --------- | -------- | ---------------- | ------------------------------- |
| RA6       | GPIO     | LED              | Controls an indicator LED       |
| RB7       | UART TX  | RS232            | Transmit data to serial device  |
| RB2       | UART RX  | RS232            | Receive data from serial device |
| RB11      | SPI SCLK | Si4455           | SPI Clock for RF module         |
| RB13      | SPI SDI  | Si4455           | SPI Data In from RF module      |
| RB10      | SPI SDO  | Si4455           | SPI Data Out to RF module       |
| RB15      | SPI nSel | Si4455           | Chip Select for RF module       |
| RB14      | GPIO     | Si4455           | IRQ pin from RF module          |
| RA1       | GPIO     | Si4455           | SND pin to RF module            |

## Project Structure

- `radio_recv.X/` - Contains firmware for the **receive board**, responsible for receiving and processing commands.
- `radio_trans.X/` - Contains firmware for the **transmit board**, responsible for sending commands wirelessly.

## Source Files

- **Debugging & LED Control**
  - `debug.h` & `debug.c` - Debug utilities for displaying Si4455 information and status.
  - `led.h` & `led.c` - LED control, useful for debugging.
- **Main Application**
  - `main.c` - The primary application logic.
- **Radio Communication**
  - `radio.h` & `radio.c` - Core functions for using the **Si4455**.
  - `radio_comm.h` & `radio_comm.c` - Simplifies **Si4455 library** communication.
  - `radio_config.h` - Configuration settings for **Si4455** (generated by **WDS**).
  - `radio_hal.h` & `radio_hal.c` - Hardware abstraction layer (HAL) for interfacing with different microcontrollers.
  - `si4455_api_lib.h` & `si4455_api_lib.c` - API functions for **Si4455** commands.
  - `si4455_defs.h` - Definitions for **Si4455 communication parameters** (Do not modify).
  - `si4455_patch.h` - Patch for initializing **Si4455** (used by `radio_config.h`, Do not modify).
- **Communication Interfaces**
  - `spi.h` & `spi.c` - **SPI driver** for radio module communication.
  - `uart.h` & `uart.c` - **UART driver** for PC communication.
- **Others**
  - `adc.h` & `adc.c` - ADC initialization.
  - `clock.h` & `clock.c` - Clock initialization and time delay.
  - `eeprom.h` & `eeprom.c` - EEPROM related functions.

## Packet Structure

Each transmitted packet consists of the following parts:

| Field         | Size       | Description                                                   |
| ------------- | ---------- | ------------------------------------------------------------- |
| **Preamble**  | 8 bytes    | Helps the receiver detect signal presence (101010... pattern) |
| **Sync Word** | 2 bytes    | Synchronization marker for correct packet alignment           |
| **Length**    | 1 byte     | Number of bytes in the payload (0-47 bytes)                   |
| **Payload**   | 0-47 bytes | Actual data being transmitted (max 47 bytes)                  |
| **CRC**       | 2 bytes    | CRC-16-IBM (initial value: 0xFFFF, high byte first)           |

The **FIFO threshold is set to 48 bytes** for both TX and RX. This means that the length byte allows up to **47 bytes** of payload data in each packet. The FIFO size is **64 bytes**, so the threshold can be adjusted up to 64 bytes if needed.

## Q&A

### **Q1: How do I modify the radio frequency settings?**

The radio settings, including frequency and modulation, are configured in `radio_config.h`. You can use **Wireless Development Suite (WDS)** to generate new configurations and update this file, which is recommended. Alternatively, you can modify the settings manually if needed.

### **Q2: Why is the signal transmission unstable?**

Ensure that the **PIC24 and Si4455 can communicate properly** and that both Si4455 modules use the **same transmit or receive circuitry**. Additionally, check the following:

- Ensure the **antenna is properly connected**.
- Check for **interference from nearby electronic devices**.
- Adjust **transmission power settings** to optimize signal strength.
- If using a breadboard, try shortening the RF module's wiring to reduce interference.

### **Q3: Why doesn't the generated radio config file work?**

Modify the **include path** and the **definition of `RADIO_CONFIGURATION_DATA`** to match your project setup.

- If the issue persists, verify that `radio_config.h` includes all necessary parameters and that no additional modifications are required after the WDS export.

## Update History

- **2025-03-25**
  - Added eeprom functions.

- **2025-03-17**
  - Fixed structure of variable-length packets.

- **2025-03-15**
  - Optimized code structure.
  - Improved Si4455 driver.

- **2025-03-12**
  - Improved **communication stability**.
  - Rejected the option of **half-duplex communication**.
  
- **2025-03-10**
  - Added **CRC** for error detection.
  - Enabled **variable-length** packet transmission.
  
- **2025-03-07**
  - Documented **packet structure**.
  - Fixed a bug in `radio.c`.
  
- **2025-03-06**
  - Implemented **FIFO buffer clearing** before starting TX and RX.
  - Added `si4455_defs.h` for better readability.
  
- **2025-03-05**
  - Extended **packet length** from **7 bytes** to **16 bytes**.
  - Added explanation for `si4455_patch.h`.

## To-Do List

  - ~~Implement **half-duplex communication**.~~ After testing, we confirmed that **Si4455 Rev C2A** (used in this project) does not support half-duplex communication. However, **Si4455 Rev B1A** may have this capability.
  - Investigate **UART communication errors** after initialization. The issue might be caused by **fast transmission rates**—adding a small delay between transmissions could improve stability.

## Author Information

  - **Author**: Yiqi Wei
  - **Affiliation**: Shanghai Jiao Tong University
  - **Contact**: yiqiwei4536@gmail.com

## References

- [PIC24F16KA102 Family Data Sheet](https://ww1.microchip.com/downloads/en/DeviceDoc/39927c.pdf)
- [Si4455 Data Sheet](https://www.silabs.com/documents/public/data-sheets/Si4455.pdf)
- [EZRadio API Rev C2A](https://www.silabs.com/documents/public/application-notes/EZRadio_REVC2_API.zip)
- [AN685: Layout Design Guide for the Si4455/435x RF ICS](https://www.silabs.com/documents/public/application-notes/AN685.pdf)
- [AN692: Si4355/4455 Programming Guide and Sample Codes](https://www.silabs.com/documents/public/application-notes/AN692.pdf)
- [AN796: Wireless Development Suite General Description](https://www.silabs.com/documents/public/application-notes/AN796.pdf)
- [AN797: WDS User's Guide for EZRadio Devices](https://www.silabs.com/documents/public/application-notes/AN797.pdf)