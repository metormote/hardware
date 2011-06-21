/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief TWI Master driver for AVR xmega.
 *
 * This file defines a useful set of functions for the TWI interface on AVR xmega
 * devices.
 *
 * - Compiler:           IAR and GCC for AVR xmega
 * - Supported devices:  All AVR xmega devices with an TWI module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _TWI_MASTER_H_
#define _TWI_MASTER_H_

#include "compiler.h"
#include "sysclk.h"
#include "status_codes.h"
#include "twim.h"

typedef twi_options_t twi_master_options_t;

static inline int twi_master_setup(TWI_t *twi, twi_master_options_t *opt)
{
  int status;   	
  opt->speed_reg = TWI_BAUD(sysclk_get_cpu_hz(),opt->speed);  

#ifdef TWIC
	if((uint16_t)twi == (uint16_t)&TWIC) {
  		sysclk_enable_module(SYSCLK_PORT_C, PR_TWI_bm);
	} 
#endif
#ifdef TWID
	if((uint16_t)twi == (uint16_t)&TWID) {
 		 sysclk_enable_module(SYSCLK_PORT_D, PR_TWI_bm);
	} 
#endif	
	    
  status = twi_master_init(twi, (const twi_options_t *)opt); 
  return(status);
}

extern int twi_master_write(TWI_t *twi, const twi_package_t *package);
extern int twi_master_read(TWI_t *twi, const twi_package_t *package);
static inline void twi_master_enable(TWI_t *twi);
static inline void twi_master_disable(TWI_t *twi);

#endif  // _TWI_MASTER_H_
