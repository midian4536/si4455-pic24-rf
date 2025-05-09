/*! @file radio_config.h
 * @brief This file contains the automatically generated
 * configurations.
 *
 * @n WDS GUI Version: 3.2.11.0
 * @n Device: Si4455 Rev.: C2
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2017 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_CONFIG_H
#define RADIO_CONFIG_H

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
// Crys_freq(Hz): 30000000    Crys_tol(ppm): 30    IF_mode: 2    High_perf_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 0    ANT_DIV: 0    PM_pattern: 0
// MOD_type: 2    Rsymb(sps): 2400    Fdev(Hz): 30000    RXBW(Hz): 114000    Manchester: 0    AFC_en: 1    Rsymb_error: 0.0    Chip-Version: 2
// RF Freq.(MHz): 433    API_TC: 28    fhst: 250000    inputBW: 0    BERT: 0    RAW_dout: 0    D_source: 0    Hi_pfm_div: 0
// API_ARR_Det_en: 0    Fdev_error: 0    API_ETSI: 0
//
// # WB filter 1 (BW = 114.46 kHz);  NB-filter 1 (BW = 114.46 kHz)

//
// Modulation index: 25
*/

// CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ 30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER 0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH 0x10
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP 0x03
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET 0xF000

#include "si4455_patch.h"

// CONFIGURATION COMMANDS

/*
// Command:                  RF_POWER_UP
// Description:              Command to power-up the device and select the operational mode and functionality.
*/
#define RF_POWER_UP 0x02, 0x81, 0x00, 0x01, 0xC9, 0xC3, 0x80

/*
// Set properties:           RF_INT_CTL_ENABLE_2
// Number of properties:     2
// Group ID:                 0x01
// Start ID:                 0x00
// Default values:           0x04, 0x00, 
// Descriptions:
//   INT_CTL_ENABLE - This property provides for global enabling of the three interrupt groups (Chip, Modem and Packet Handler) in order to generate HW interrupts at the NIRQ pin.
//   INT_CTL_PH_ENABLE - Enable individual interrupt sources within the Packet Handler Interrupt Group to generate a HW interrupt on the NIRQ output pin.
*/
#define RF_INT_CTL_ENABLE_2 0x11, 0x01, 0x02, 0x00, 0x01, 0x20

/*
// Set properties:           RF_FRR_CTL_A_MODE_4
// Number of properties:     4
// Group ID:                 0x02
// Start ID:                 0x00
// Default values:           0x01, 0x02, 0x09, 0x00, 
// Descriptions:
//   FRR_CTL_A_MODE - Fast Response Register A Configuration.
//   FRR_CTL_B_MODE - Fast Response Register B Configuration.
//   FRR_CTL_C_MODE - Fast Response Register C Configuration.
//   FRR_CTL_D_MODE - Fast Response Register D Configuration.
*/
#define RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x08, 0x06, 0x04, 0x0A

/*
// Set properties:           RF_EZCONFIG_XO_TUNE_1
// Number of properties:     1
// Group ID:                 0x24
// Start ID:                 0x03
// Default values:           0x40, 
// Descriptions:
//   EZCONFIG_XO_TUNE - Configure the internal capacitor frequency tuning bank for the crystal oscillator.
*/
#define RF_EZCONFIG_XO_TUNE_1 0x11, 0x24, 0x01, 0x03, 0x40

/*
// Command:                  RF_WRITE_TX_FIFO
// Description:              Writes data byte(s) to the TX FIFO.
*/
#define RF_WRITE_TX_FIFO 0x66, 0xB4, 0xBC, 0x10, 0x7B, 0x82, 0x20, 0xCA, 0x75, 0x70, 0x15, 0x7D, 0x67, 0x8A, 0xD4, 0x13, 0x3A, 0x0D, 0xB1, 0xA0, \
0x8F, 0xDC, 0x2D, 0x4D, 0x29, 0x95, 0x03, 0x5E, 0x71, 0x02, 0x77, 0x1B, 0x48, 0x1F, 0xFC, 0x9D, 0xE6, 0x48, 0xC6, 0xC0, \
0x9A, 0x0F, 0x18, 0x4B, 0x45, 0x36, 0xCB, 0x46, 0x89, 0x6B, 0x4E, 0x72, 0x0B, 0x73, 0xD7, 0xBB, 0x9A, 0x53, 0x40, 0xC8, \
0x70, 0x67, 0x33, 0xCD, 0x58, 0x16, 0x2E, 0xB2, 0x14, 0xDA, 0xAD, 0x33, 0xE4, 0x5B, 0x8C, 0x5E, 0x09, 0xF6, 0xFC, 0x16, \
0xD8, 0xAA, 0x2D, 0x34, 0xFA, 0x26, 0x9C, 0xB5, 0x54, 0xA9, 0x27, 0xFC, 0x3C, 0x75, 0x70, 0xDE, 0x0D, 0x7A, 0x30, 0x36, \
0xA4, 0xC2, 0x13, 0x16, 0x46, 0x03, 0x64, 0x04, 0xED, 0x7D, 0xFC, 0x20, 0xAC, 0x37

/*
// Command:                  RF_NOP
// Description:              No Operation command.
*/
#define RF_NOP 0x00

/*
// Command:                  RF_WRITE_TX_FIFO_1
// Description:              Writes data byte(s) to the TX FIFO.
*/
#define RF_WRITE_TX_FIFO_1 0x66, 0x1E, 0xB3, 0x4D, 0x54, 0x41, 0x0F, 0x44, 0x9F, 0x25, 0xA6, 0x52, 0xB7, 0x5D, 0x4C, 0xAB, 0x3F, 0xC1, 0x20, 0x8B, \
0x0B, 0xED, 0x7B, 0x3D, 0x49, 0x00, 0xF3, 0xAB, 0x0D, 0xCE, 0xB2, 0x15, 0xE3, 0x7A, 0x3B, 0xC3, 0xD0, 0x20, 0x07, 0x6D, \
0x72, 0x8A, 0x39, 0x0F, 0x45, 0xB2, 0x1F, 0x4E, 0x58, 0x49, 0xC7, 0x19, 0x3A, 0xE2, 0xF1, 0x05, 0xC5, 0x8D, 0x97, 0xFC, \
0x8E, 0x22, 0xCC, 0xC1, 0xDC, 0x4F, 0x48, 0xBE, 0xC9, 0x35, 0x25, 0xCA, 0x96, 0xCC, 0xF8, 0x11, 0x8B, 0x51, 0xFD, 0x41, \
0x75, 0xB3, 0xBC, 0x88, 0x46, 0xAC, 0x51, 0xAE, 0xFA, 0xFF, 0x39, 0x2A, 0xB0, 0x04, 0x5F, 0x7E, 0x57, 0xF2, 0xB5, 0x31, \
0xEE, 0xCE, 0x0A, 0x2C, 0x2D, 0x02, 0xCA, 0x5C, 0x82, 0x56, 0xB0, 0xB4

/*
// Command:                  RF_EZCONFIG_CHECK
// Description:              Validates the EZConfig array was written correctly.
*/
#define RF_EZCONFIG_CHECK 0x19, 0x14, 0x4C

/*
// Command:                  RF_GPIO_PIN_CFG
// Description:              Configures the GPIO pins.
*/
#define RF_GPIO_PIN_CFG 0x13, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00


// AUTOMATICALLY GENERATED CODE! 
// DO NOT EDIT/MODIFY BELOW THIS LINE!
// --------------------------------------------

#ifndef FIRMWARE_LOAD_COMPILE
#define RADIO_CONFIGURATION_DATA_ARRAY { \
        SI446X_PATCH_CMDS, \
        0x07, RF_POWER_UP, \
        0x06, RF_INT_CTL_ENABLE_2, \
        0x08, RF_FRR_CTL_A_MODE_4, \
        0x05, RF_EZCONFIG_XO_TUNE_1, \
        0x72, RF_WRITE_TX_FIFO, \
        0x01, RF_NOP, \
        0x70, RF_WRITE_TX_FIFO_1, \
        0x03, RF_EZCONFIG_CHECK, \
        0x08, RF_GPIO_PIN_CFG, \
        0x00 \
 }
#else
#define RADIO_CONFIGURATION_DATA_ARRAY { 0 }
#endif

#define RADIO_CONFIGURATION_DATA {                       \
    radio_configuration_data_array,                      \
    RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,             \
    RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH,        \
    RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP, \
    RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET}

#endif