#include <xc.h>
#include "spi.h"

#define SPI_DUMMY_BYTE (0xff)

void spi_init(void)
{
    TRIS_SDO = 0;
    TRIS_SDI = 1;
    TRIS_SCK = 0;

    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.SPRE = 6;
    SPI1CON1bits.PPRE = 2;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.MODE16 = 0;

    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;
    IFS0bits.SPI1IF = 0;
}

uint8_t spi_transfer_byte(uint8_t byte)
{
    SPI1BUF = byte;
    while (!IFS0bits.SPI1IF)
        ;
    IFS0bits.SPI1IF = 0;
    return SPI1BUF;
}

void spi_send_byte(uint8_t byte)
{
    spi_transfer_byte(byte);
}

uint8_t spi_recv_byte(void)
{
    return spi_transfer_byte(SPI_DUMMY_BYTE);
}

void spi_send_bytes(const uint8_t *pdata, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        spi_transfer_byte(pdata[i]);
    }
}

void spi_recv_bytes(uint8_t *pdata, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        pdata[i] = spi_transfer_byte(0xff);
    }
}
