#include "xc.h"
#include "uart.h"

unsigned char uart_buf[BUF_MAX_LEN];
unsigned char buf_index;
unsigned char recv_char;
unsigned char uart_flag;

void uart_init(void) {
    TRIS_RX = 1;
    TRIS_TX = 0;

    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;

    U1MODEbits.BRGH = 1;
    U1BRG = 103;

    IPC3bits.U1TXIP = 4;
    IPC2bits.U1RXIP = 4;
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 1;

    U1MODEbits.UARTEN = 1;

    U1STAbits.UTXEN = 1;

    while (U1STAbits.UTXBF)
        ;
    U1TXREG = 'T';
}

void uart_send_char(unsigned char data) {
    while (U1STAbits.UTXBF)
        ;
    U1TXREG = data;
    while (!U1STAbits.TRMT)
        ;
}

void uart_send_array(unsigned char *pdata, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        while (U1STAbits.UTXBF)
            ;
        U1TXREG = pdata[i];
    }
    while (!U1STAbits.TRMT)
        ;
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;

    if (U1STAbits.URXDA) {
        recv_char = U1RXREG;
        if (recv_char == END_CHAR || buf_index >= BUF_MAX_LEN) {
            uart_flag = 1;
        } else if (buf_index < BUF_MAX_LEN) {
            uart_buf[buf_index++] = recv_char;
        }
    }
}
