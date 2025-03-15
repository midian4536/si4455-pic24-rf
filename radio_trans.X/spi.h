#ifndef SPI_H
#define SPI_H

#define TRIS_SDN TRISAbits.TRISA1
#define SDN LATAbits.LATA1

#define TRIS_SDI TRISBbits.TRISB10
#define SDI PORTBbits.RB10

#define TRIS_SCK TRISBbits.TRISB11
#define SCK LATBbits.LATB11

#define TRIS_SDO TRISBbits.TRISB13
#define SDO LATBbits.LATB13

#define TRIS_NIRQ TRISBbits.TRISB14
#define nIRQ PORTBbits.RB14

#define TRIS_nSel TRISBbits.TRISB15
#define nSel LATBbits.LATB15

void spi_init(void);

unsigned char spi_transfer(unsigned char data);

void spi_send_array(unsigned char *pdata, unsigned char length);
void spi_recv_array(unsigned char *pdata, unsigned char length);

#endif
