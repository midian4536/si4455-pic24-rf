#pragma config OSCIOFNC = ON
#pragma config FNOSC = FRCDIV

#include "xc.h"
#include "p24F16KA102.h"
#include "string.h"

#include "spi.h"
#include "uart.h"
#include "led.h"

#include "utils.h"
#include "debug.h"

#include "radio.h"
#include "radio_config.h"

void handler();

int main(void) {
    AD1PCFG = 0xffff;
    CLKDIVbits.RCDIV = 0;
    led_init();
    spi_init();
    uart_init();
    uart_send_array((unsigned char *) "uart initialized", 16);

    radio_init();
    radio_start_rx(p_radio_configuration->radio_channel_number);
    uart_send_array((unsigned char *) "4455 initialized", 16);
    led_shine(3, 100);

    while (1) {
        handler();
        if (uart_flag) {
            uart_send_array((unsigned char *) "uart buf:", 9);
            uart_send_array(uart_buf, BUF_MAX_LEN);
            memset(uart_buf, 0, BUF_MAX_LEN);
            buf_index = 0;
            uart_flag = 0;
        }
    }
    return 0;
}

void handler() {
    if (1 == radio_check_received_variable_packet()) {
        switch (p_radio_configuration->radio_variable_packet_state) {
            case 0:
                uart_send_array(fix_radio_packet, 16);
                led_shine(1, (fix_radio_packet[9] - '0')*100);
                break;
            case 1:
                uart_send_array(fix_radio_packet, 16);
                led_shine(1, (fix_radio_packet[9] - '0')*100);
                break;
            default:
                break;
        }
    }
}