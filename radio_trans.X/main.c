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

#include "radio.h"

void uart_handler();
void radio_handler();

int main(void) {
    clock_init();
    adc_init();

    led_init();
    spi_init();
    uart_init();
    uart_send_array((unsigned char *) "uart initialized", 16);

    radio_init();
    uart_send_array((unsigned char *) "4455 initialized", 16);
    led_shine(3, 300);

    custom_radio_packet[0] = 16;
    memcpy(custom_radio_packet + 1, "0123456789abcdef", custom_radio_packet[0]);

    while (1) {
        uart_handler();
        radio_handler();
    }
    return 0;
}

void uart_handler() {
    if (uart_flag) {
        uart_send_array((unsigned char *) "uart buf:", 9);
        uart_send_array(uart_buf, buf_index);

        custom_radio_packet[0] = buf_index;
        memcpy(custom_radio_packet + 1, uart_buf, custom_radio_packet[0]);

        memset(uart_buf, 0, buf_index);
        buf_index = 0;
        uart_flag = 0;
    }
}

void radio_handler() {
    static unsigned char is_pkt_sending = 0;
    if (1 == radio_check_transmitted()) {
        is_pkt_sending = 0;
    }
    if (0 == is_pkt_sending) {
        uart_send_array((unsigned char *) "send: ", 6);
        uart_send_array(custom_radio_packet + 1, custom_radio_packet[0]);
        radio_start_tx_variable_packet(p_radio_configuration->radio_channel_number, custom_radio_packet, custom_radio_packet[0] + 1);
        is_pkt_sending = 1;
        delay_ms(1000);
    }
}