/*****************************************************************************
 *
 * \file
 *
 * \brief Configuration for the ET024006DHU display.
 *
 * Copyright (c) 2009-2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/


 //! Configuration of the et024006dhu LCD display driver

#ifndef _CONF_ET024006DHU_H_
#define _CONF_ET024006DHU_H_

#include "compiler.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(ET024006_IFACE_MODE)      || \
    !defined(ET024006_CMD_ADDR)        || \
    !defined(ET024006_PARAM_ADDR)      || \
    !defined(ET024006DHU_TE_PIN)       || \
    !defined(ET024006DHU_BL_PIN)       || \
    !defined(ET024006DHU_BL_FUNCTION)  || \
    !defined(ET024006DHU_RESET_PIN)


#warning The ET024006DHU configuration does not exist in the board definition file. Using default settings.

#define ET024006_IFACE_MODE       ET024006_IFACE_MODE_EBI

/*! \brief address used to write to the index register
 * (or read device status
 */
#define ET024006_CMD_ADDR     ((volatile unsigned short int *) AVR32_EBI_CS0_ADDRESS)


/*! \brief address used to read/write from/to registers or RAM
 *
 * If the select signal connected to address line 21, 
 * this implies an address offset of 0x200000.
 */
#define ET024006_PARAM_ADDR   ((volatile unsigned short int *) (AVR32_EBI_CS0_ADDRESS + 0x00200000))

//! Tearing sync default configuration
#define ET024006DHU_TE_PIN				AVR32_PIN_PA00

//! Reset pin default configuration
#define ET024006DHU_RESET_PIN			AVR32_PIN_PA02

//! Backlight pin default configuration
#define ET024006DHU_BL_PIN				AVR32_PIN_PA01

// ! Backlight pin default function if PMW is used for managing the brightness
#define ET024006DHU_BL_FUNCTION			0


#endif

#ifdef __cplusplus
}
#endif

#endif // _CONF_ET024006DHU_H_
