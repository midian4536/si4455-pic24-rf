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

#ifndef RADIO_CONFIG_H_
#define RADIO_CONFIG_H_

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
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ                     30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER                    0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH               0x10
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP        0x03
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       0xF000
#define RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD					   {0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5}

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
#define RF_WRITE_TX_FIFO 0x66, 0xEB, 0x7E, 0x9A, 0xC4, 0x3A, 0x3B, 0x03, 0x9A, 0xA1, 0x9B, 0xB5, 0xF5, 0x53, 0x68, 0x42, 0x52, 0x9E, 0xAB, 0x90, \
0x7A, 0x41, 0x58, 0x31, 0x1A, 0x37, 0x69, 0xE7, 0x94, 0xCC, 0x82, 0x81, 0x20, 0x35, 0x7F, 0x6D, 0xCF, 0x9B, 0xDC, 0x73, \
0x60, 0xBA, 0xBF, 0x2B, 0xDE, 0x23, 0xE9, 0xF7, 0xE0, 0xB8, 0xD7, 0xCE, 0xD9, 0x14, 0x6C, 0x73, 0xB6, 0x0E, 0x6A, 0xF5, \
0x04, 0xD8, 0x8C, 0xCB, 0xC7, 0x49, 0xAD, 0x7F, 0xB6, 0xB7, 0x09, 0x80, 0x9A, 0xFC, 0x55, 0x5C, 0xDC, 0x08, 0x03, 0x10, \
0xC4, 0xFA, 0x86, 0x2B, 0x44, 0xE3, 0x8F, 0x4E, 0x22, 0x82, 0xE3, 0x92, 0xCF, 0x31, 0xC2, 0xE0, 0x29, 0x2F, 0x15, 0x19, \
0x94, 0x89, 0x0A, 0xF5, 0x56, 0x2C, 0xEA, 0xCB, 0x62, 0xF2, 0xF0, 0x23, 0x87, 0xCE

/*
// Command:                  RF_NOP
// Description:              No Operation command.
*/
#define RF_NOP 0x00

/*
// Command:                  RF_WRITE_TX_FIFO_1
// Description:              Writes data byte(s) to the TX FIFO.
*/
#define RF_WRITE_TX_FIFO_1 0x66, 0xFB, 0xFE, 0xB7, 0xE6, 0xFB, 0x39, 0x6F, 0xD8, 0x44, 0x7D, 0xC4, 0x19, 0xCB, 0xDF, 0xB6, 0x12, 0x62, 0x0D, 0xAC, \
0x34, 0xB4, 0x26, 0x2A, 0xFF, 0xCA, 0xF2, 0x14, 0x8F, 0x43, 0x93, 0xF4, 0x60, 0x33, 0x8B, 0xD0, 0x16, 0xDD, 0x92, 0x17, \
0x1C, 0xFD, 0x6F, 0xC3, 0x19, 0xD8, 0xA1, 0xB6, 0xB0, 0x2F, 0xC0, 0x7E, 0x76, 0xF3, 0x3F, 0xCB, 0xCA, 0x0B, 0xA7, 0x41, \
0xA9, 0x7D, 0xDF, 0x66, 0x68, 0x26, 0x85, 0x63, 0xA9, 0xB1, 0x77, 0x32, 0x65, 0xFC, 0x78, 0xBD, 0x6A, 0xFC, 0x5A, 0x71, \
0x0E, 0x00, 0x03, 0xA6, 0x29, 0x23, 0x1E, 0x28, 0x49, 0x4D, 0x36, 0xA7, 0xBF, 0x0B, 0x5C, 0x5F, 0x92, 0xA3, 0xC8, 0xD9, \
0x9A, 0x8C, 0xB4, 0x24, 0x95, 0xAB, 0xA5, 0xC1, 0xAB, 0xE8, 0xC1, 0x1F

/*
// Command:                  RF_EZCONFIG_CHECK
// Description:              Validates the EZConfig array was written correctly.
*/
#define RF_EZCONFIG_CHECK 0x19, 0x3E, 0x95

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

// DEFAULT VALUES FOR CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT                     30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT                    0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT               0x10
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT        0x01
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT       0x1000
#define RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD_DEFAULT  		           {0x42, 0x55, 0x54, 0x54, 0x4F, 0x4E, 0x31} // BUTTON1 

#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_INCLUDED                      0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_SIZE                          0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH                               {  }

#ifndef RADIO_CONFIGURATION_DATA_ARRAY
#error "This property must be defined!"
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ          RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER         RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH    RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP   RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET  RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD
#define RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD         RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD_DEFAULT 
#endif

#define RADIO_CONFIGURATION_DATA { \
                            radio_configuration_data_array,                            \
                            RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,                   \
                            RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH,              \
                            RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP,       \
                            RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET,      \
                            RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD                    \
                            }

#endif /* RADIO_CONFIG_H_ */
