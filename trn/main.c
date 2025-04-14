#pragma config OSCIOFNC = ON
#pragma config FNOSC = FRCDIV

#include <xc.h>
#include <stdbool.h>
#include <p24F16KA102.h>
#include <string.h>

#include "spi.h"
#include "uart.h"

#include "delay.h"

#include "led.h"
#include "radio.h"

void uart_handler();
void radio_handler();

int main(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xffff;

    led_init();
    spi_init();
    uart_init();
    uart_send_array((uint8_t *)"uart initialized", 16);

    radio_init();
    uart_send_array((uint8_t *)"4455 initialized", 16);
    led_shine(3, 300);

    const char test_str[] = "Embedded systems require careful memory use.";
    custom_radio_packet[0] = sizeof(test_str);
    memcpy(custom_radio_packet + 1, test_str, sizeof(test_str));

    while (1)
    {
        uart_handler();
        radio_handler();
    }
    return 0;
}

void uart_handler()
{
    if (uart_rx_flag)
    {
        custom_radio_packet[0] = uart_rx_buf_index;
        for (uint8_t i = 0; i < uart_rx_buf_index; i++)
        {
            custom_radio_packet[i + 1] = uart_rx_buf[i];
            uart_rx_buf[i] = 0;
        }

        uart_rx_buf_index = 0;
        uart_rx_flag = 0;
    }
}

void radio_handler()
{
    static bool is_pkt_sending = 0;
    if (radio_check_transmitted())
    {
        is_pkt_sending = 0;
    }
    if (!is_pkt_sending)
    {
        uart_send_array((uint8_t *)"send: ", 6);
        uart_send_array(custom_radio_packet + 1, custom_radio_packet[0]);
        radio_start_tx_variable_packet(0, custom_radio_packet, custom_radio_packet[0] + 1);
        is_pkt_sending = 1;
        __delay_ms(1000);
    }
}