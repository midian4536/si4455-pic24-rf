#ifndef EEPROM_H
#define	EEPROM_H

#define EEPROM_START_ADDR  0x7FFE00
#define EEPROM_SIZE        256     

void eeprom_write_word(uint32_t addr, uint16_t data);
uint16_t eeprom_read_word(uint32_t addr);

void eeprom_write_byte(uint32_t addr, uint8_t data);
uint8_t eeprom_read_byte(uint32_t addr);

void eeprom_write_word_array(uint32_t addr, uint16_t *data, uint16_t length);
void eeprom_read_word_array(uint32_t addr, uint16_t *buffer, uint16_t length);

void eeprom_write_byte_array(uint32_t addr, uint8_t *data, uint16_t length);
void eeprom_read_byte_array(uint32_t addr, uint8_t *buffer, uint16_t length);

void eeprom_erase_all(void);

#endif

