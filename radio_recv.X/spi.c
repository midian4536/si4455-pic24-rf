#include "xc.h"
#include "spi.h"

void spi_init(void) {
    TRIS_nSel = 0;
    nSel = 1;

    TRIS_SDN = 0;
    SDN = 1;

    TRIS_SDO = 0;
    TRIS_SDI = 1;
    TRIS_SCK = 0;
    TRIS_NIRQ = 1;

    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.SPRE = 6;
    SPI1CON1bits.PPRE = 2;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.MODE16 = 0;
    SPI1STATbits.SPIROV = 0;
    IFS0bits.SPI1IF = 0;
    SPI1STATbits.SPIEN = 1;
}

uint8_t spi_transfer(uint8_t data) {
    SPI1BUF = data;
    while (!IFS0bits.SPI1IF)
        ;
    IFS0bits.SPI1IF = 0;
    return SPI1BUF;
}

void spi_send_array(uint8_t *pdata, uint8_t length) {
    nSel = 0;
    for (uint8_t i = 0; i < length; i++) {
        spi_transfer(pdata[i]);
    }
    nSel = 1;
}

void spi_recv_array(uint8_t *pdata, uint8_t length) {
    nSel = 0;
    for (uint8_t i = 0; i < length; i++) {
        pdata[i] = spi_transfer(0xFF);
    }
    nSel = 1;
}
