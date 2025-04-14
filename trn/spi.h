#ifndef SPI_H
#define SPI_H

#define TRIS_SDI _TRISB10
#define SDI _RB10

#define TRIS_SCK _TRISB11
#define SCK _LATB11

#define TRIS_SDO _TRISB13
#define SDO _LATB13

void spi_init(void);

uint8_t spi_transfer_byte(uint8_t byte);
void spi_send_byte(uint8_t byte);
uint8_t spi_recv_byte(void);

void spi_send_bytes(const uint8_t *pdata, uint8_t len);
void spi_recv_bytes(uint8_t *pdata, uint8_t len);

#endif
