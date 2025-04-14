#include "uart.h"
#include "delay.h"

volatile uint8_t uart_rx_buf[UART_RX_BUF_LEN];
volatile uint8_t uart_rx_buf_index = 0;
volatile uint8_t uart_rx_byte = 0;
volatile bool uart_rx_flag = 0;

volatile uint8_t uart_tx_buf[UART_TX_BUF_LEN];
volatile uint8_t uart_tx_head = 0;
volatile uint8_t uart_tx_tail = 0;
volatile bool uart_tx_busy = 0;

void uart_init(void)
{
    TRIS_TX = 0;
    TX = 1;
    TRIS_RX = 1;
    RX = 0;

    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;

    U1MODEbits.BRGH = 1;
    U1BRG = FCY / 4 / UART_BAUD_RATE - 1;

    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IPC3bits.U1TXIP = 4;
    IPC2bits.U1RXIP = 4;
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;

    U1MODEbits.UARTEN = 1;

    U1STAbits.UTXEN = 1;
}

void uart_send_byte(uint8_t byte)
{
    while (U1STAbits.UTXBF)
        ;
    U1TXREG = byte;
    while (!U1STAbits.TRMT)
        ;
}

void uart_send_array(uint8_t *pdata, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t next_head = (uart_tx_head + 1) % UART_TX_BUF_LEN;
        while (next_head == uart_tx_tail)
            ;
        uart_tx_buf[uart_tx_head] = pdata[i];
        uart_tx_head = next_head;
    }

    if (!uart_tx_busy)
    {
        uart_tx_busy = 1;
        if (uart_tx_tail != uart_tx_head)
        {
            U1TXREG = uart_tx_buf[uart_tx_tail];
            uart_tx_tail = (uart_tx_tail + 1) % UART_TX_BUF_LEN;
        }
        IEC0bits.U1TXIE = 1;
    }
}

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;

    if (uart_tx_tail != uart_tx_head)
    {
        U1TXREG = uart_tx_buf[uart_tx_tail];
        uart_tx_tail = (uart_tx_tail + 1) % UART_TX_BUF_LEN;
    }
    else
    {
        IEC0bits.U1TXIE = 0;
        uart_tx_busy = 0;
    }
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;

    if (U1STAbits.URXDA)
    {
        uart_rx_byte = U1RXREG;
        if (uart_rx_byte == UART_END_CHAR || uart_rx_buf_index >= UART_RX_BUF_LEN)
        {
            uart_rx_flag = 1;
        }
        else if (uart_rx_buf_index < UART_RX_BUF_LEN)
        {
            uart_rx_buf[uart_rx_buf_index++] = uart_rx_byte;
        }
    }
}
