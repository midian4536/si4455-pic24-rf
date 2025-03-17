#ifndef UART_H
#define UART_H

#define TRIS_RX TRISBbits.TRISB2
#define RX PORTBbits.RB2

#define TRIS_TX TRISBbits.TRISB7
#define TX LATBbits.LATB7

#define END_CHAR ';'
#define UART_BUF_MAX_LEN 64

extern unsigned char uart_buf[UART_BUF_MAX_LEN];
extern unsigned char buf_index;
extern unsigned char recv_char;
extern unsigned char uart_flag;

void uart_init(void);

void uart_send_char(unsigned char data);
void uart_send_array(unsigned char *pdata, unsigned int length);

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);

#endif
