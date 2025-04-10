#ifndef UART_H
#define UART_H

#define TRIS_RX TRISBbits.TRISB2
#define RX PORTBbits.RB2

#define TRIS_TX TRISBbits.TRISB7
#define TX LATBbits.LATB7

#define END_CHAR ';'
#define UART_BUF_MAX_LEN 64

extern uint8_t uart_buf[UART_BUF_MAX_LEN];
extern uint8_t buf_index;
extern uint8_t recv_char;
extern uint8_t uart_flag;

void uart_init(void);

void uart_send_char(uint8_t data);
void uart_send_array(uint8_t *pdata, unsigned int length);

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);

#endif
