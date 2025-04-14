#pragma config OSCIOFNC = ON
#pragma config FNOSC = FRCDIV

#include <xc.h>
#include <p24F16KA102.h>
#include <string.h>

#include "spi.h"
#include "uart.h"

#include "delay.h"

#include "led.h"
#include "radio.h"

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
    radio_start_rx(0);
    uart_send_array((uint8_t *)"rx start", 8);
    led_shine(3, 100);

    while (1)
    {
        radio_handler();
    }
    return 0;
}

void radio_handler()
{
    if (radio_check_received_variable_packet())
    {
        uart_send_array(custom_radio_packet + 1, custom_radio_packet[0]);
    }
}