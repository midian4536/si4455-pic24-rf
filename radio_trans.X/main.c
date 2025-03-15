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
    uart_send_array((unsigned char *) "4455 initialized", 16);
    led_shine(3, 300);

    memcpy(fix_radio_packet, "0123456789abcdef", 16);
    while (1) {
        handler();
        if (uart_flag) {
            uart_send_array((unsigned char *) "uart buf:", 9);
            uart_send_array(uart_buf, BUF_MAX_LEN);
            memcpy(fix_radio_packet, uart_buf, 16);
            memset(uart_buf, 0, BUF_MAX_LEN);
            buf_index = 0;
            uart_flag = 0;
        }
    }
    return 0;
}

void handler() {
    static unsigned char is_pkt_sending = 0;
    if (1 == radio_check_transmitted()) {
        is_pkt_sending = 0;
    }
    if (0 == is_pkt_sending) {
        uart_send_array((unsigned char *) "send: ", 6);
        uart_send_array(fix_radio_packet, 16);
        radio_start_tx(p_radio_configuration->radio_channel_number, fix_radio_packet);
        is_pkt_sending = 1;
        delay_ms(1000);
    }
}