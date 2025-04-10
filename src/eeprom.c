#include "xc.h"
#include "eeprom.h"

static void eeprom_unlock_write(void) {
    __builtin_disi(5);
    NVMKEY = 0x55;
    NVMKEY = 0xAA;
    NVMCONbits.WR = 1;
    __builtin_nop();
    __builtin_nop();
    while (NVMCONbits.WR);
}

void eeprom_write_word(uint32_t addr, uint16_t data) {
    TBLPAG = (addr >> 16) & 0xFF;
    uint16_t offset = addr & 0xFFFF;
    __builtin_tblwtl(offset, data);
    NVMCON = 0x4004;
    eeprom_unlock_write();
}

uint16_t eeprom_read_word(uint32_t addr) {
    TBLPAG = (addr >> 16) & 0xFF;
    uint16_t offset = addr & 0xFFFF;
    return __builtin_tblrdl(offset);
}

void eeprom_write_byte(uint32_t addr, uint8_t data) {
    uint32_t word_addr = addr & 0xFFFFFFFE;
    uint16_t word = eeprom_read_word(word_addr);

    if (addr % 2 == 0) {
        word = (word & 0xFF00) | data; 
    } else {
        word = (word & 0x00FF) | (data << 8); 
    }

    eeprom_write_word(word_addr, word);
}

uint8_t eeprom_read_byte(uint32_t addr) {
    uint32_t word_addr = addr & 0xFFFFFFFE; 
    uint16_t word = eeprom_read_word(word_addr);
    
    if (addr % 2 == 0) {
        return (uint8_t)(word & 0x00FF); 
    } else {
        return (uint8_t)((word >> 8) & 0x00FF);
    }
}

void eeprom_write_word_array(uint32_t addr, uint16_t *data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        eeprom_write_word(addr + (i * 2), data[i]);
    }
}

void eeprom_read_word_array(uint32_t addr, uint16_t *buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = eeprom_read_word(addr + (i * 2));
    }
}

void eeprom_write_byte_array(uint32_t addr, uint8_t *data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        eeprom_write_byte(addr + i, data[i]);
    }
}

void eeprom_read_byte_array(uint32_t addr, uint8_t *buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = eeprom_read_byte(addr + i);
    }
}

void eeprom_erase_all(void) {
    for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
        eeprom_write_word(EEPROM_START_ADDR + (i * 2), 0xFFFF);
    }
}

