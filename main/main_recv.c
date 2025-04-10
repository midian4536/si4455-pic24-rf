#pragma config OSCIOFNC = ON
#pragma config FNOSC = FRCDIV

#include "xc.h"
#include "p24F16KA102.h"
#include "string.h"

#include "led.h"
#include "spi.h"
#include "uart.h"

#include "adc.h"

#include "clock.h"
#include "debug.h"

#include "eeprom.h"

#include "radio.h"

void uart_handler();
void radio_handler();

int main(void) {
    clock_init();
    adc_init();

    led_init();
    spi_init();
    uart_init();
    uart_send_array((uint8_t *) "uart initialized", 16);

    radio_init();
    radio_start_rx(p_radio_configuration->radio_channel_number);
    uart_send_array((uint8_t *) "4455 initialized", 16);
    led_shine(3, 100);

    while (1) {
        uart_handler();
        radio_handler();
    }
    return 0;
}

void uart_handler() {
    if (uart_flag) {
        uart_send_array((uint8_t *) "uart buf:", 9);
        uart_send_array(uart_buf, buf_index);
        memset(uart_buf, 0, buf_index);
        buf_index = 0;
        uart_flag = 0;
    }
}

void radio_handler() {
    if (1 == radio_check_received_variable_packet()) {
        switch (p_radio_configuration->radio_variable_packet_state) {
            case 0:
                uart_send_array(custom_radio_packet, p_radio_configuration->radio_packet_length);
                break;
            case 1:
                uart_send_array(custom_radio_packet + 1, custom_radio_packet[0]);
                break;
            default:
                break;
        }
    }
}