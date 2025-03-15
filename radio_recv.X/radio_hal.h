#ifndef RADIO_HAL_H
#define RADIO_HAL_H

void radio_hal_assert_shut_down(void);
void radio_hal_deassert_shut_down(void);

void radio_hal_clear_nsel(void);
void radio_hal_set_nsel(void);

unsigned char radio_hal_nirq_level(void);

void radio_hal_spi_write_byte(unsigned char byte);
unsigned char radio_hal_spi_read_byte(void);

void radio_hal_spi_write_data(unsigned char byte_count, unsigned char *pdata);
void radio_hal_spi_read_data(unsigned char byte_count, unsigned char *pdata);

#endif
