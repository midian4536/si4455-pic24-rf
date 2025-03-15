#include "debug.h"
#include "uart.h"
#include "si4455_api_lib.h"
#include "utils.h"

unsigned char hex1, hex2;

void char_to_hex(unsigned char value, unsigned char *hex1, unsigned char *hex2) {
    *hex1 = (value >> 4) + ((value >> 4) < 10 ? '0' : 'A' - 10);
    *hex2 = (value & 0x0F) + ((value & 0x0F) < 10 ? '0' : 'A' - 10);
}

void show_si4455_properties() {
    si4455_get_property(0x01, 4, 0);
    delay_ms(100);
    char_to_hex(radio_cmd[0], &hex1, &hex2);
    uart_send_array((unsigned char *) "INT_CTL_ENABLE: ", 16);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[1], &hex1, &hex2);
    uart_send_array((unsigned char *) "INT_CTL_PH_ENABLE: ", 19);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[2], &hex1, &hex2);
    uart_send_array((unsigned char *) "INT_CTL_MODEM_ENABLE: ", 22);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[3], &hex1, &hex2);
    uart_send_array((unsigned char *) "INT_CTL_CHIP_ENABLE: ", 21);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');

    si4455_get_property(0x02, 4, 0);
    delay_ms(100);
    char_to_hex(radio_cmd[0], &hex1, &hex2);
    uart_send_array((unsigned char *) "FRR_CTL_A_MODE: ", 16);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[1], &hex1, &hex2);
    uart_send_array((unsigned char *) "FRR_CTL_B_MODE: ", 16);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[2], &hex1, &hex2);
    uart_send_array((unsigned char *) "FRR_CTL_C_MODE: ", 16);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[3], &hex1, &hex2);
    uart_send_array((unsigned char *) "FRR_CTL_D_MODE: ", 16);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');

    si4455_get_property(0x24, 1, 3);
    delay_ms(100);
    char_to_hex(radio_cmd[0], &hex1, &hex2);
    uart_send_array((unsigned char *) "EZCONFIG_XO_TUNE: ", 18);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
}

void show_si4455_fifo() {
    si4455_fifo_info(0);
    delay_ms(100);
    char_to_hex(radio_cmd[0], &hex1, &hex2);
    uart_send_array((unsigned char *) "RX_FIFO_COUNT: ", 15);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[1], &hex1, &hex2);
    uart_send_array((unsigned char *) "TX_FIFO_SPACE: ", 15);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
}

void show_si4455_state() {
    si4455_device_state();
    delay_ms(100);
    char_to_hex(radio_cmd[0], &hex1, &hex2);
    uart_send_array((unsigned char *) "CURR_STATE: ", 12);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
    delay_ms(100);
    char_to_hex(radio_cmd[1], &hex1, &hex2);
    uart_send_array((unsigned char *) "CURRENT_CHANNEL: ", 17);
    uart_send_char(hex1);
    uart_send_char(hex2);
    uart_send_char('H');
}