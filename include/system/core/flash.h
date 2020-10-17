/* AVR Flash Header */

#ifndef FLASH_H
#define FLASH_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _table table_t;
typedef struct _entry entry_t;
typedef struct _plain plain_t;
typedef struct _array array_t;

typedef struct _flash {
    
    int pc;
    
    int16_t *memory;
    unsigned int mem_usage;

    array_t *plain;
    table_t *table;
    
} flash_t;

/* FLASH Constructor + Destructor */

extern struct _flash* flash_ctor(const char *file);
extern void flash_dtor(struct _flash *this);

/* General FLASH Functions */

extern plain_t* flash_fetch(const struct _flash *this);

extern plain_t* flash_read_instr(const struct _flash *this, const int addr);
extern uint16_t flash_read(const struct _flash *this, const int addr);

extern void flash_move_pc(struct _flash *this, const int inc);
extern void flash_set_pc(struct _flash *this, const int addr);
extern int flash_get_pc(const struct _flash *this);

extern void flash_reboot(struct _flash *this);

/* Table Operations */

extern int flash_add_breakp(const struct _flash *this, const char *point);
extern int flash_del_breakp(const struct _flash *this, const char *point);
extern bool flash_on_breakp(const struct _flash *this);
extern int flash_table_size(const struct _flash *this);
extern entry_t* flash_dump_table(const struct _flash *this);

#endif
