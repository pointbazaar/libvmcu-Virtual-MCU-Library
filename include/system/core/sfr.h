/* SFR Header */

#ifndef SFR_H
#define SFR_H

// C Headers
#include <inttypes.h>

// Project Headers
#include "system/mcudef.h"

/*
 * This file belongs to the IO Module but is
 * quite long so I decided to create a seperate
 * file for these functions.
 * 
 * This file will be automatically included by
 * including the IO Module 'io.h'.
 * 
 **/

typedef struct _io io_t;

extern void (*sfr_write[SFR_SIZE]) (io_t *this, const int8_t value);
extern int8_t (*sfr_read[SFR_SIZE]) (io_t *this);

#endif
