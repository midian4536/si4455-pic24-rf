#ifndef UART_H
#define UART_H

#include <xc.h>
#include <stdbool.h>

#define TRIS_RX _TRISB2
#define RX _RB2

#define TRIS_TX _TRISB7
#define TX _LATB7

#define UART_BAUD_RATE (9600)

#define UART_END_CHAR ';'
#define UART_RX_BUF_LEN (64)
#define UART_TX_BUF_LEN (64)

extern volatile uint8_t uart_rx_buf[UART_RX_BUF_LEN];
extern volatile uint8_t uart_rx_buf_index;
extern volatile uint8_t uart_rx_byte;
extern volatile bool uart_rx_flag;

extern volatile uint8_t uart_tx_buf[UART_TX_BUF_LEN];
extern volatile uint8_t uart_tx_head;
extern volatile uint8_t uart_tx_tail;
extern volatile bool uart_tx_busy;

void uart_init(void);

bool uart_send_byte(uint8_t byte);
void uart_send_bytes(const uint8_t *pdata, uint8_t len);

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);

#endif
