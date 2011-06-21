/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Management of the AT45DBX data flash driver through SPI.
 *
 * This file manages the accesses to the AT45DBX data flash components.
 *
 * - Compiler:           IAR EWAVR and GNU GCC for AVR
 * - Supported devices:  All AVR devices with an SPI module can be used.
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

//_____  I N C L U D E S ___________________________________________________


#include "at45dbx.h"
#include "at45dbx_hal_spi.h"

//_____ D E F I N I T I O N S ______________________________________________


//! Boolean indicating whether memory is in busy state.
static bool at45dbx_busy;

//! Memory data pointer.
static uint32_t at45dbx_gl_ptr_mem;

//! Sector buffer.
static uint8_t sector_buf[AT45DBX_SECTOR_SIZE];


/*! \name Control Functions
 */
//! @{
bool at45dbx_init(void)
{
	at45dbx_spi_init();
	// Memory ready by default.
	at45dbx_busy = FALSE;
	return OK;
}


/*! \brief Selects or unselects a DF memory.
 *
 * \param memidx  Memory ID of DF to select or unselect.
 * \param bSelect Boolean indicating whether the DF memory has to be selected.
 */
static void at45dbx_chipselect_df(uint8_t memidx, bool bSelect)
{
	if (bSelect) {
		// Select SPI chip.
		at45dbx_spi_select_device(memidx);
	}
	else {
	 // Unselect SPI chip.
	 	at45dbx_spi_deselect_device(memidx);
	}
}


bool at45dbx_mem_check(void)
{
	uint8_t df;
	uint8_t status = 0;

	// DF memory check.
	for (df = 0; df < AT45DBX_MEM_CNT; df++) {
		// Select the DF memory to check.
		at45dbx_chipselect_df(df, TRUE);
		// Send the Status Register Read command.
		at45dbx_spi_write_byte(AT45DBX_CMDC_RD_STATUS_REG);
		// Send a dummy byte to read the status register.
		at45dbx_spi_read_byte(&status);
		// Unselect the checked DF memory.
		at45dbx_chipselect_df(df, FALSE);
		// Unexpected device density value.
		if ((status & AT45DBX_MSK_DENSITY) < AT45DBX_DENSITY) return KO;
	}
	return OK;
}


static void at45dbx_wait_ready(void)
{
	uint8_t status;

	// Select the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, TRUE);

	// Send the Status Register Read command.
	at45dbx_spi_write_byte(AT45DBX_CMDC_RD_STATUS_REG);
	// Read the status register until the DF is ready.
	do {
		// Send a dummy byte to read the status register.
		at45dbx_spi_read_byte(&status);	
	} while ((status & AT45DBX_MSK_BUSY) == AT45DBX_BUSY);

	// Unselect the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);
}

bool at45dbx_read_sector_open(uint32_t sector)
{
	return at45dbx_read_byte_open(sector << AT45DBX_SECTOR_BITS);   //sector * AT45DBX_SECTOR_SIZE.
}

bool at45dbx_read_byte_open(uint32_t ad)
{
	uint32_t addr;

	// Set the global memory pointer to a byte address.
	at45dbx_gl_ptr_mem = ad;

	// If the DF memory is busy, wait until it's ready.
	if (at45dbx_busy) at45dbx_wait_ready();
	at45dbx_busy = FALSE;

	// Select the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, TRUE);

	// Initiate a page read at a given sector.

	// Send the Main Memory Page Read command.
	at45dbx_spi_write_byte(AT45DBX_CMDA_RD_PAGE);
	
	// Send the three address bytes, which comprise:
	//  - (24 - (AT45DBX_PAGE_ADDR_BITS + AT45DBX_BYTE_ADDR_BITS)) reserved bits;
	//  - then AT45DBX_PAGE_ADDR_BITS bits specifying the page in main memory to be read;
	//  - then AT45DBX_BYTE_ADDR_BITS bits specifying the starting byte address within that page.
	// NOTE: The bits of at45dbx_gl_ptr_mem above the AT45DBX_MEM_SIZE bits are useless for the local
	// DF addressing. They are used for DF discrimination when there are several DFs.
	addr = (Rd_bitfield(at45dbx_gl_ptr_mem, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS) |
		Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE);

	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));

	// Send 32 don't care clock cycles to initialize the read operation.
	at45dbx_spi_write_byte(0x55);
	at45dbx_spi_write_byte(0x55);
	at45dbx_spi_write_byte(0x55);
	at45dbx_spi_write_byte(0x55);
	return OK;
}


void at45dbx_read_close(void)
{
	// Unselect the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);
	// Memory ready.
	at45dbx_busy = FALSE;
}

bool at45dbx_write_sector_open(uint32_t sector)
{
	// Set the global memory pointer to a byte address.
	return at45dbx_write_byte_open(sector << AT45DBX_SECTOR_BITS); // at45dbx_gl_ptr_mem = sector * AT45DBX_SECTOR_SIZE.
}

bool at45dbx_write_byte_open(uint32_t ad)
{
	volatile uint32_t addr;
   at45dbx_gl_ptr_mem = ad;
	
	// If the DF memory is busy, wait until it's ready.
	if (at45dbx_busy) {
		at45dbx_wait_ready();
	}
	at45dbx_busy = FALSE;

#if AT45DBX_PAGE_SIZE > AT45DBX_SECTOR_SIZE
	// Select the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, TRUE);

	// Transfer the content of the current page to buffer 1.

	// Send the Main Memory Page to Buffer 1 Transfer command.
	at45dbx_spi_write_byte(AT45DBX_CMDB_XFR_PAGE_TO_BUF1);

	// Send the three address bytes, including:
	//  - (24 - (AT45DBX_PAGE_ADDR_BITS + AT45DBX_BYTE_ADDR_BITS)) reserved bits;
	//  - then AT45DBX_PAGE_ADDR_BITS bits specifying the page in main memory to be read;
	//  - then AT45DBX_BYTE_ADDR_BITS don't care bits.
	// NOTE: The bits of at45dbx_gl_ptr_mem above the AT45DBX_MEM_SIZE bits are useless for the local
	// DF addressing. They are used for DF discrimination when there are several DFs.
	addr = Rd_bitfield(at45dbx_gl_ptr_mem, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS;
	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));

	// Unselect the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);

	// Wait for end of page transfer.
	at45dbx_wait_ready();
#endif

	// Select the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, TRUE);

	// Initiate a page write at a given sector.

	// Send the Main Memory Page Program through Buffer 1 command.
	at45dbx_spi_write_byte(AT45DBX_CMDB_PR_PAGE_TH_BUF1);

	// Send the three address bytes, which comprise:
	//  - (24 - (AT45DBX_PAGE_ADDR_BITS + AT45DBX_BYTE_ADDR_BITS)) reserved bits;
	//  - then AT45DBX_PAGE_ADDR_BITS bits specifying the page in main memory to be written;
	//  - then AT45DBX_BYTE_ADDR_BITS bits specifying the starting byte address within that page.
	// NOTE: The bits of at45dbx_gl_ptr_mem above the AT45DBX_MEM_SIZE bits are useless for the local
	// DF addressing. They are used for DF discrimination when there are several DFs.
	addr = ((uint32_t)(Rd_bitfield(at45dbx_gl_ptr_mem, (uint32_t)AT45DBX_MSK_PTR_PAGE)) << AT45DBX_BYTE_ADDR_BITS) |
			Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE);	
	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));

	return OK;
}


void at45dbx_write_close(void)
{
	// While end of logical sector not reached, zero-fill remaining memory bytes.
	while (Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_SECTOR)) {
		at45dbx_spi_write_byte(0x00);
		at45dbx_gl_ptr_mem++;
	}

	// Unselect the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);

	// Memory busy.
	at45dbx_busy = TRUE;
}


//! @}


/*! \name Single-Byte Access Functions
 */
//! @{


uint8_t at45dbx_read_byte(void)
{
	uint8_t data;

	// Memory busy.
	if (at45dbx_busy) {
		// Being here, we know that we previously finished a page read.
		// => We have to access the next page.

		// Memory ready.
		at45dbx_busy = FALSE;

	// Eventually select the next DF and open the next page.
	// NOTE: at45dbx_read_open input parameter is a sector.
		at45dbx_read_sector_open(at45dbx_gl_ptr_mem >> AT45DBX_SECTOR_BITS); // at45dbx_gl_ptr_mem / AT45DBX_SECTOR_SIZE.
	}

	at45dbx_spi_read_byte(&data);
	at45dbx_gl_ptr_mem++;

	// If end of page reached,
	if (!Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE)) {
		// unselect the DF memory at45dbx_gl_ptr_mem points to.
		at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);
		// Memory busy.
		at45dbx_busy = TRUE;
	}
	return data;
}


bool at45dbx_write_byte(uint8_t b)
{
	// Memory busy.
	if (at45dbx_busy) {
		// Being here, we know that we previously launched a page programming.
		// => We have to access the next page.
		// Eventually select the next DF and open the next page.
	// NOTE: at45dbx_write_open input parameter is a sector.
		at45dbx_write_byte_open(at45dbx_gl_ptr_mem); 
	}

	// Write the next data byte.
	at45dbx_spi_write_byte(b);
	at45dbx_gl_ptr_mem++;

	// If end of page reached,
	if (!Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE)) {
		// unselect the DF memory at45dbx_gl_ptr_mem points to in order to program the page.
		at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);
		// Memory busy.
		at45dbx_busy = TRUE;
	}
	return OK;
}


//! @}


/*! \name Multiple-Sector Access Functions
 */
//! @{


bool at45dbx_read_multiple_sector(uint16_t nb_sector)
{
	while (nb_sector--) {
		// Read the next sector.
		at45dbx_read_sector_to_ram(sector_buf);
		at45dbx_read_multiple_sector_callback(sector_buf);
	}
	return OK;
}


bool at45dbx_write_multiple_sector(uint16_t nb_sector)
{
	while (nb_sector--) {
		// Write the next sector.
		at45dbx_write_multiple_sector_callback(sector_buf);
		at45dbx_write_sector_from_ram(sector_buf);
	}
	return OK;
}


//! @}


/*! \name Single-Sector Access Functions
 */
 
//! @{
bool at45dbx_read_sector_to_ram(void *ram)
{
	uint32_t end_addr=at45dbx_gl_ptr_mem+AT45DBX_SECTOR_SIZE;
	
	if (at45dbx_busy) {
		// Being here, we know that we previously finished a page read.
		// => We have to access the next page.
		at45dbx_busy = FALSE;
		// Eventually select the next DF and open the next page.
		at45dbx_read_byte_open(at45dbx_gl_ptr_mem);
	}
	
	while(at45dbx_gl_ptr_mem<end_addr) {
#if (AT45DBX_PAGE_SIZE<AT45DBX_SECTOR_SIZE)
		at45dbx_spi_read_packet(ram,AT45DBX_PAGE_SIZE);
		at45dbx_gl_ptr_mem+=AT45DBX_PAGE_SIZE;
		ram=(void*)((uint8_t*)ram+(uint32_t)AT45DBX_PAGE_SIZE);
		at45dbx_read_close();
		at45dbx_read_byte_open(at45dbx_gl_ptr_mem);
#else
		at45dbx_spi_read_packet(ram,AT45DBX_SECTOR_SIZE);
		at45dbx_gl_ptr_mem+=AT45DBX_SECTOR_SIZE;
		 // If end of page reached,
		if (!Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE)) {
			at45dbx_read_close();
			at45dbx_read_byte_open(at45dbx_gl_ptr_mem);
		}
		
#endif		
	}
	return OK;
}


bool at45dbx_write_sector_from_ram(const void *ram)
{
	uint32_t end_addr=at45dbx_gl_ptr_mem+AT45DBX_SECTOR_SIZE;
	// Memory busy.
	if (at45dbx_busy) {
		//Being here, we know that we previously launched a page programming.
		//=> We have to access the next page.

		//Eventually select the next DF and open the next page.
		// NOTE: at45dbx_write_open input parameter is a sector.
		at45dbx_write_byte_open(at45dbx_gl_ptr_mem); // at45dbx_gl_ptr_mem / AT45DBX_SECTOR_SIZE.
	}
	while(at45dbx_gl_ptr_mem<end_addr) {
#if (AT45DBX_PAGE_SIZE<AT45DBX_SECTOR_SIZE)
		at45dbx_spi_write_packet(ram,AT45DBX_PAGE_SIZE);
		at45dbx_gl_ptr_mem+=AT45DBX_PAGE_SIZE;
		ram=(void*)((uint8_t*)ram+(uint32_t)AT45DBX_PAGE_SIZE);
		 // If end of page reached,
		if (!Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE)) {
			// unselect the DF memory at45dbx_gl_ptr_mem points to in order to program the page.
			at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);
			// Memory busy.
			at45dbx_busy = TRUE;
		}
		if(at45dbx_gl_ptr_mem<end_addr) {
			at45dbx_write_byte_open(at45dbx_gl_ptr_mem);
		}
#else
		at45dbx_spi_write_packet(ram,AT45DBX_SECTOR_SIZE);
		at45dbx_gl_ptr_mem+=AT45DBX_SECTOR_SIZE;
		ram=(void*)((uint8_t*)ram+(uint32_t)AT45DBX_SECTOR_SIZE);
		 // If end of page reached,
		if (!Rd_bitfield(at45dbx_gl_ptr_mem, AT45DBX_MSK_PTR_BYTE)) {
			// unselect the DF memory at45dbx_gl_ptr_mem points to in order to program the page.
			at45dbx_chipselect_df(at45dbx_gl_ptr_mem >> AT45DBX_MEM_SIZE, FALSE);
			// Memory busy.
			at45dbx_busy = TRUE;
		}

#endif		
	}
	return OK;
}
//! @}
