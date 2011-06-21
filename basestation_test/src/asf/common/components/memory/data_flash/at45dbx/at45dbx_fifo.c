/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Use the AT45DBX data flash driver as a FIFO buffer through SPI.
 *
 * This file manages the accesses to the AT45DBX data flash components.
 *
 *
 ******************************************************************************/

/* Copyright (c) 2011 Metormote AB. All rights reserved.
 *
 */

//_____  I N C L U D E S ___________________________________________________

#include "at45dbx_fifo.h"


//! Boolean indicating whether memory is in busy state.
static bool at45dbx_busy_read;
static bool at45dbx_busy_write;

//! Memory data pointer.
static AT45DBX_FIFO *at45dbx_fifo;



/*! \brief Returns the number of elements in the ring buffer
 *
 * \retval Number of elements in the ring buffer.
 */
static volatile uint32_t at45dbx_fifo_count (void)
{	
    return (at45dbx_fifo ? (at45dbx_fifo->head - at45dbx_fifo->tail) : 0);
}


/*! \brief Returns the empty/full status of the ring buffer.
 *
 * \retval true full.
 * \retval false not full.
 */
static bool at45dbx_fifo_full (void)
{
    return (at45dbx_fifo ? (at45dbx_fifo_count() == at45dbx_fifo->buffer_len) : true);
}


/*! \brief Returns the empty/full status of the ring buffer.
 *
 * \retval true empty.
 * \retval false not empty.
 */
static bool at45dbx_fifo_empty (void)
{
    return (at45dbx_fifo ? (at45dbx_fifo_count() == 0) : true);
}


/*! \brief Selects or unselects a DF memory.
 *
 * \param memidx  Memory ID of DF to select or unselect.
 * \param bSelect Boolean indicating whether the DF memory has to be selected.
 */
static void at45dbx_fifo_chipselect_df(uint8_t memidx, bool bSelect)
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



static void at45dbx_fifo_wait_ready(void)
{
	uint8_t status;

	// Select the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);

	// Send the Status Register Read command.
	at45dbx_spi_write_byte(AT45DBX_CMDC_RD_STATUS_REG);
	// Read the status register until the DF is ready.
	do {
		// Send a dummy byte to read the status register.
		at45dbx_spi_read_byte(&status);	
	} while ((status & AT45DBX_MSK_BUSY) == AT45DBX_BUSY);

	// Unselect the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);
}



static void at45dbx_fifo_load_read_buffer(void) 
{
    volatile uint32_t addr;
  
    at45dbx_busy_read = TRUE;
  
    at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);
  
  // Send the Main Memory Page Read command.
	at45dbx_spi_write_byte(AT45DBX_CMDB_XFR_PAGE_TO_BUF1);
	
	addr = (Rd_bitfield(at45dbx_fifo->head, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS);
  
	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));
  
  // Unselect the DF memory at45dbx_gl_ptr_mem points to.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);
  
}

static void at45dbx_fifo_save_write_buffer(void) 
{
	volatile uint32_t addr;
  
    at45dbx_busy_write = TRUE;
  
	// Select the DF memory.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);
  
    at45dbx_spi_write_byte(AT45DBX_CMDB_PR_BUF2_TO_PAGE_ER);
	
	addr = (Rd_bitfield((uint32_t)at45dbx_fifo->tail, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS);

	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));
  
  // Unselect the DF memory.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);

}



/*! \name Control Functions
 */
//! @{
bool at45dbx_fifo_init(AT45DBX_FIFO *fifo)
{
    at45dbx_fifo=fifo;
	at45dbx_spi_init();
	// Memory ready by default.
	at45dbx_busy_read = FALSE;
	at45dbx_busy_write = FALSE;
	return OK;
}


uint16_t at45dbx_fifo_read(uint8_t *buf, uint16_t offset, uint16_t len) 
{
    volatile uint32_t addr;
    uint16_t i;
	uint8_t *p	= buf+offset;
	
	if (at45dbx_fifo_empty()) {
		return 0;
	}
		
  // If the DF memory is busy, wait until it's ready.
	if (at45dbx_busy_read) {
		at45dbx_fifo_wait_ready();
	}
	at45dbx_busy_read = FALSE;
  
	// Select the DF memory.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);
  
    at45dbx_spi_write_byte(AT45DBX_CMDC_RD_BUF1_LF_SM);
	
	//addr = (Rd_bitfield(at45dbx_fifo->head, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS |
	//		Rd_bitfield(at45dbx_fifo->head, AT45DBX_MSK_PTR_BYTE));
	addr = Rd_bitfield(at45dbx_fifo->head, AT45DBX_MSK_PTR_BYTE);
	
	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));
  
    // Send a dont care clock cycle to initialize the read operation.
	at45dbx_spi_write_byte(0x55);
	
    for(i=0;i<len;i++) {
		if (at45dbx_fifo_empty()) {
			break;
		}
		// Read the next data byte.
		at45dbx_spi_read_byte(p++);
		at45dbx_fifo->head++;

		// If end of page reached
		if(at45dbx_fifo->head & AT45DBX_MSK_PTR_BYTE) {
			// Unselect the DF memory.
			at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);
      
			//read a new page to buffer
			at45dbx_fifo_load_read_buffer();
      
			// Wait for end of page transfer.
			at45dbx_fifo_wait_ready();
			at45dbx_busy_read = FALSE;
      
			// Select the DF memory.
			at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);
  
			at45dbx_spi_write_byte(AT45DBX_CMDC_RD_BUF1_LF_SM);
	
			//addr = (Rd_bitfield(at45dbx_fifo->head, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS |
			//	Rd_bitfield(at45dbx_fifo->head, AT45DBX_MSK_PTR_BYTE));
			addr = Rd_bitfield(at45dbx_fifo->head, AT45DBX_MSK_PTR_BYTE);
			
			at45dbx_spi_write_byte(LSB2W(addr));
			at45dbx_spi_write_byte(LSB1W(addr));
			at45dbx_spi_write_byte(LSB0W(addr));
    }
  }
  
  // Unselect the DF memory.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);
  
  return i;
}



uint16_t at45dbx_fifo_write(uint8_t *buf, uint16_t offset, uint16_t len) 
{
	volatile uint32_t addr;
    uint16_t i;
	uint8_t *p=buf+offset;
	
	if (at45dbx_fifo_full()) {
		return 0;
	}
	
  // If the DF memory is busy, wait until it's ready.
	if (at45dbx_busy_write) {
		at45dbx_fifo_wait_ready();
	}
	at45dbx_busy_write = FALSE;
  
	// Select the DF memory.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);
  
    at45dbx_spi_write_byte(AT45DBX_CMDC_WR_BUF2);
	
	//addr = (Rd_bitfield(at45dbx_fifo->tail, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS |
	//	Rd_bitfield(at45dbx_fifo->tail, AT45DBX_MSK_PTR_BYTE));
	addr = Rd_bitfield(at45dbx_fifo->tail, AT45DBX_MSK_PTR_BYTE);
	
	at45dbx_spi_write_byte(LSB2W(addr));
	at45dbx_spi_write_byte(LSB1W(addr));
	at45dbx_spi_write_byte(LSB0W(addr));
  
    for(i=0;i<len;i++) {
		
		if (at45dbx_fifo_full()) {
			break;
		}
		// Write the next data byte.
		at45dbx_spi_write_byte(*p);
		p++;
		at45dbx_fifo->tail++;

	  // If end of page reached
		if(at45dbx_fifo->tail & AT45DBX_MSK_PTR_BYTE) {
			// Unselect the DF memory.
			at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);
      
			//save the page to main memory
			at45dbx_fifo_save_write_buffer();
      
			// Wait for end of page transfer.
			at45dbx_fifo_wait_ready();
			at45dbx_busy_write = FALSE;
      
			// Select the DF memory.
			at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, TRUE);
  
			at45dbx_spi_write_byte(AT45DBX_CMDC_WR_BUF2);
	
			//addr = (Rd_bitfield(at45dbx_fifo->tail, (uint32_t)AT45DBX_MSK_PTR_PAGE) << AT45DBX_BYTE_ADDR_BITS |
			//Rd_bitfield(at45dbx_fifo->tail, AT45DBX_MSK_PTR_BYTE));
			addr = Rd_bitfield(at45dbx_fifo->tail, AT45DBX_MSK_PTR_BYTE);
			
			at45dbx_spi_write_byte(LSB2W(addr));
			at45dbx_spi_write_byte(LSB1W(addr));
			at45dbx_spi_write_byte(LSB0W(addr));
		}
  }
  
  // Unselect the DF memory.
	at45dbx_fifo_chipselect_df(at45dbx_fifo->memidx, FALSE);
  
  return i;
}

//! @}
