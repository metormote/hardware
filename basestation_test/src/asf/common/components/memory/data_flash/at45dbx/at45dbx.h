/**
 * \file
 *
 * \brief Management of the AT45DBX DataFlash component.
 *
 * Copyright (C) 2010 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#ifndef _AT45DBX_H_
#define _AT45DBX_H_

#include "compiler.h"
#include "conf_at45dbx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup at45dbx_group AT45dbx DataFlash component driver.
 *
 * This is a driver for the AT45dbX serie DataFlash memories.
 * It provides functions for initialization, read and write operations at 
 * byte or sector level.
 * The DataFlash driver can be used either with SPI Master Sevice or 
 * USART_SPI Service. Depending on AT45DBX_USES_SPI_MASTER_SERVICE or 
 *	AT45DBX_USES_USART_SPI_SERVICE define in conf_at45dbx.h it is possible to 
 *	configure the driver to use the specified SPI service. 
 *
 * \section dependencies Dependencies
 * This driver depends on the following modules:
 * - \ref spi_group for SPI master interface.
 *
 * @{
 */
//_____ D E F I N I T I O N S ______________________________________________

/*! \name Available AT45DBX Sizes
 *
 * Number of address bits of available AT45DBX data flash memories.
 *
 * \note All memories from page sizes of 256 bytes to 1024 bytes are
 *       supported.
 */
//! @{
#define AT45DBX_500KB           19
#define AT45DBX_1MB             20
#define AT45DBX_2MB             21
#define AT45DBX_4MB             22
#define AT45DBX_8MB             23
//! @}

//! Number of bits for addresses within sectors.
#define AT45DBX_SECTOR_BITS     9

//! Sector size in bytes.
#define AT45DBX_SECTOR_SIZE     (1 << AT45DBX_SECTOR_BITS)


#if AT45DBX_MEM_CNT > 4
	#error AT45DBX_MEM_CNT must not exceed 4
#endif


/*! \name AT45DBX Group A Commands
 */
//! @{
#define AT45DBX_CMDA_RD_PAGE              0xD2        //!< Main Memory Page Read (Serial/8-bit Mode).
#define AT45DBX_CMDA_RD_ARRAY_LEG         0xE8        //!< Continuous Array Read, Legacy Command (Serial/8-bit Mode).
#define AT45DBX_CMDA_RD_ARRAY_LF_SM       0x03        //!< Continuous Array Read, Low-Frequency Mode (Serial Mode).
#define AT45DBX_CMDA_RD_ARRAY_AF_SM       0x0B        //!< Continuous Array Read, Any-Frequency Mode (Serial Mode).
#define AT45DBX_CMDA_RD_SECTOR_PROT_REG   0x32        //!< Read Sector Protection Register (Serial/8-bit Mode).
#define AT45DBX_CMDA_RD_SECTOR_LKDN_REG   0x35        //!< Read Sector Lockdown Register (Serial/8-bit Mode).
#define AT45DBX_CMDA_RD_SECURITY_REG      0x77        //!< Read Security Register (Serial/8-bit Mode).
//! @}

/*! \name AT45DBX Group B Commands
 */
//! @{
#define AT45DBX_CMDB_ER_PAGE              0x81        //!< Page Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_ER_BLOCK             0x50        //!< Block Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_ER_SECTOR            0x7C        //!< Sector Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_ER_CHIP              0xC794809A  //!< Chip Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_XFR_PAGE_TO_BUF1     0x53        //!< Main Memory Page to Buffer 1 Transfer (Serial/8-bit Mode).
#define AT45DBX_CMDB_XFR_PAGE_TO_BUF2     0x55        //!< Main Memory Page to Buffer 2 Transfer (Serial/8-bit Mode).
#define AT45DBX_CMDB_CMP_PAGE_TO_BUF1     0x60        //!< Main Memory Page to Buffer 1 Compare (Serial/8-bit Mode).
#define AT45DBX_CMDB_CMP_PAGE_TO_BUF2     0x61        //!< Main Memory Page to Buffer 2 Compare (Serial/8-bit Mode).
#define AT45DBX_CMDB_PR_BUF1_TO_PAGE_ER   0x83        //!< Buffer 1 to Main Memory Page Program with Built-in Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_PR_BUF2_TO_PAGE_ER   0x86        //!< Buffer 2 to Main Memory Page Program with Built-in Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_PR_BUF1_TO_PAGE      0x88        //!< Buffer 1 to Main Memory Page Program without Built-in Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_PR_BUF2_TO_PAGE      0x89        //!< Buffer 2 to Main Memory Page Program without Built-in Erase (Serial/8-bit Mode).
#define AT45DBX_CMDB_PR_PAGE_TH_BUF1      0x82        //!< Main Memory Page Program through Buffer 1 (Serial/8-bit Mode).
#define AT45DBX_CMDB_PR_PAGE_TH_BUF2      0x85        //!< Main Memory Page Program through Buffer 2 (Serial/8-bit Mode).
#define AT45DBX_CMDB_RWR_PAGE_TH_BUF1     0x58        //!< Auto Page Rewrite through Buffer 1 (Serial/8-bit Mode).
#define AT45DBX_CMDB_RWR_PAGE_TH_BUF2     0x59        //!< Auto Page Rewrite through Buffer 2 (Serial/8-bit Mode).
//! @}

/*! \name AT45DBX Group C Commands
 */
//! @{
#define AT45DBX_CMDC_RD_BUF1_LF_SM        0xD1        //!< Buffer 1 Read, Low-Frequency Mode (Serial Mode).
#define AT45DBX_CMDC_RD_BUF2_LF_SM        0xD3        //!< Buffer 2 Read, Low-Frequency Mode (Serial Mode).
#define AT45DBX_CMDC_RD_BUF1_AF_SM        0xD4        //!< Buffer 1 Read, Any-Frequency Mode (Serial Mode).
#define AT45DBX_CMDC_RD_BUF2_AF_SM        0xD6        //!< Buffer 2 Read, Any-Frequency Mode (Serial Mode).
#define AT45DBX_CMDC_RD_BUF1_AF_8M        0x54        //!< Buffer 1 Read, Any-Frequency Mode (8-bit Mode).
#define AT45DBX_CMDC_RD_BUF2_AF_8M        0x56        //!< Buffer 2 Read, Any-Frequency Mode (8-bit Mode).
#define AT45DBX_CMDC_WR_BUF1              0x84        //!< Buffer 1 Write (Serial/8-bit Mode).
#define AT45DBX_CMDC_WR_BUF2              0x87        //!< Buffer 2 Write (Serial/8-bit Mode).
#define AT45DBX_CMDC_RD_STATUS_REG        0xD7        //!< Status Register Read (Serial/8-bit Mode).
#define AT45DBX_CMDC_RD_MNFCT_DEV_ID_SM   0x9F        //!< Manufacturer and Device ID Read (Serial Mode).
//! @}

/*! \name AT45DBX Group D Commands
 */
//! @{
#define AT45DBX_CMDD_EN_SECTOR_PROT       0x3D2A7FA9  //!< Enable Sector Protection (Serial/8-bit Mode).
#define AT45DBX_CMDD_DIS_SECTOR_PROT      0x3D2A7F9A  //!< Disable Sector Protection (Serial/8-bit Mode).
#define AT45DBX_CMDD_ER_SECTOR_PROT_REG   0x3D2A7FCF  //!< Erase Sector Protection Register (Serial/8-bit Mode).
#define AT45DBX_CMDD_PR_SECTOR_PROT_REG   0x3D2A7FFC  //!< Program Sector Protection Register (Serial/8-bit Mode).
#define AT45DBX_CMDD_LKDN_SECTOR          0x3D2A7F30  //!< Sector Lockdown (Serial/8-bit Mode).
#define AT45DBX_CMDD_PR_SECURITY_REG      0x9B000000  //!< Program Security Register (Serial/8-bit Mode).
#define AT45DBX_CMDD_PR_CONF_REG          0x3D2A80A6  //!< Program Configuration Register (Serial/8-bit Mode).
#define AT45DBX_CMDD_DEEP_PWR_DN          0xB9        //!< Deep Power-down (Serial/8-bit Mode).
#define AT45DBX_CMDD_RSM_DEEP_PWR_DN      0xAB        //!< Resume from Deep Power-down (Serial/8-bit Mode).
//! @}


/*! \name Bit-Masks and Values for the Status Register
 */
//! @{
#define AT45DBX_MSK_BUSY                  0x80        //!< Busy status bit-mask.
#define AT45DBX_BUSY                      0x00        //!< Busy status value (0x00 when busy, 0x80 when ready).
#define AT45DBX_MSK_DENSITY               0x3C        //!< Device density bit-mask.
//! @}




#if AT45DBX_MEM_SIZE==AT45DBX_500KB 
/*! \name AT45DB041 Memories
 */
//! @{
#define AT45DBX_DENSITY                   0x1C        //!< Device density value.
#define AT45DBX_BYTE_ADDR_BITS            9           //!< Address bits for byte position within buffer.
//! @}

#elif AT45DBX_MEM_SIZE == AT45DBX_1MB

/*! \name AT45DB081 Memories
 */
//! @{
#define AT45DBX_DENSITY                   0x24        //!< Device density value.
#define AT45DBX_BYTE_ADDR_BITS            9           //!< Address bits for byte position within buffer.
//! @}

#elif AT45DBX_MEM_SIZE == AT45DBX_2MB

/*! \name AT45DB161 Memories
 */
//! @{
#define AT45DBX_DENSITY                   0x2C        //!< Device density value.
#define AT45DBX_BYTE_ADDR_BITS            10          //!< Address bits for byte position within buffer.
//! @}

#elif AT45DBX_MEM_SIZE == AT45DBX_4MB

/*! \name AT45DB321 Memories
 */
//! @{
#define AT45DBX_DENSITY                   0x34        //!< Device density value.
#define AT45DBX_BYTE_ADDR_BITS            10          //!< Address bits for byte position within buffer.
//! @}

#elif AT45DBX_MEM_SIZE == AT45DBX_8MB

/*! \name AT45DB642 Memories
 */
//! @{
#define AT45DBX_DENSITY                   0x3C        //!< Device density value.
#define AT45DBX_BYTE_ADDR_BITS            11          //!< Address bits for byte position within buffer.
//! @}

#else
	#error AT45DBX_MEM_SIZE is not defined to a supported value
#endif

//! Address bits for page selection.
#define AT45DBX_PAGE_ADDR_BITS            (AT45DBX_MEM_SIZE - AT45DBX_PAGE_BITS)

//! Number of bits for addresses within pages.
#define AT45DBX_PAGE_BITS                 (AT45DBX_BYTE_ADDR_BITS - 1)

//! Page size in bytes.
#define AT45DBX_PAGE_SIZE                 (1 << AT45DBX_PAGE_BITS)

//! Bit-mask for byte position within buffer in \ref at45dbx_gl_ptr_mem.
#define AT45DBX_MSK_PTR_BYTE              ((1 << AT45DBX_PAGE_BITS) - 1)

//! Bit-mask for page selection in \ref at45dbx_gl_ptr_mem.
#define AT45DBX_MSK_PTR_PAGE              (((1 << AT45DBX_PAGE_ADDR_BITS) - 1) << AT45DBX_PAGE_BITS)

//! Bit-mask for byte position within sector in \ref at45dbx_gl_ptr_mem.
#define AT45DBX_MSK_PTR_SECTOR            ((1 << AT45DBX_SECTOR_BITS) - 1)


/*! \brief Sends a dummy byte through SPI.
 */
#define spi_write_dummy()                 spi_write(AT45DBX_SPI, 0xFF)



//_____ D E C L A R A T I O N S ____________________________________________

/*! \name Control Functions
 */
//! @{

/*! \brief Initializes the data flash controller and the SPI channel by which
 *         the DF is controlled.
 *
 * \retval OK Success.
 * \retval KO Failure.
 */
extern bool at45dbx_init(void);

/*! \brief Performs a memory check on all DataFlash memories.
 *
 * \retval OK Success.
 * \retval KO Failure.
 */
extern bool at45dbx_mem_check(void);

/*! \brief Opens a DataFlash memory in read mode at a given sector.
 *
 * \param sector  Start sector.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note Sector may be page-unaligned (depending on the DataFlash page size).
 */
extern bool at45dbx_read_sector_open(uint32_t sector);

/*! \brief Opens a DataFlash memory in read mode at a given byte address.
 *
 * \param ad  Start address.
 *
 * \retval OK Success.
 * \retval KO Failure.
 */
extern bool at45dbx_read_byte_open(uint32_t ad);


/*! \brief Unselects the current DataFlash memory.
 */
extern void at45dbx_read_close(void);

/*! \brief This function opens a DataFlash memory in write mode at a given sector.
 *
 * \param sector  Start sector.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note Sector may be page-unaligned (depending on the DF page size).
 *
 * \note If \ref AT45DBX_PAGE_SIZE > \ref AT45DBX_SECTOR_SIZE, page content is
 *       first loaded in buffer to then be partially updated by write byte or
 *       write sector functions.
 */
extern bool at45dbx_write_sector_open(uint32_t sector);

/*! \brief This function opens a DataFlash memory in write mode at a given address.
 *
 * \param ad  Start addres.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 */
extern bool at45dbx_write_byte_open(uint32_t ad);

/*! \brief Fills the end of the current logical sector and launches page programming.
 */
extern void at45dbx_write_close(void);

//! @}


/*! \name Single-Byte Access Functions
 */
//! @{

/*! \brief Performs a single byte read from DataFlash memory.
 *
 * \return The read byte.
 *
 * \note First call must be preceded by a call to the \ref at45dbx_read_byte_open
 *       function.
 */
extern uint8_t at45dbx_read_byte(void);

/*! \brief Performs a single byte write to DataFlash memory.
 *
 * \param b The byte to write.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note First call must be preceded by a call to the \ref at45dbx_write_byte_open
 *       function.
 */
extern bool at45dbx_write_byte(uint8_t b);

//! @}


/*! \name Multiple-Sector Access Functions
 */
//! @{

/*! \brief Reads \a nb_sector sectors from DataFlash memory.
 *
 * Data flow is: DataFlash -> callback.
 *
 * \param nb_sector Number of contiguous sectors to read.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note First call must be preceded by a call to the \ref at45dbx_read_sector_open
 *       function.
 *
 * \note As \ref AT45DBX_PAGE_SIZE is always a multiple of
 *       \ref AT45DBX_SECTOR_SIZE, there is no need to check page end for each
 *       byte.
 */
extern bool at45dbx_read_multiple_sector(uint16_t nb_sector);

/*! \brief Callback function invoked after each sector read during
 *         \ref at45dbx_read_multiple_sector.
 *
 * \param psector Pointer to read sector.
 */
extern void at45dbx_read_multiple_sector_callback(const void *psector);

/*! \brief Writes \a nb_sector sectors to DataFlash memory.
 *
 * Data flow is: callback -> DataFlash.
 *
 * \param nb_sector Number of contiguous sectors to write.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note First call must be preceded by a call to the \ref at45dbx_write_sector_open
 *       function.
 *
 * \note As \ref AT45DBX_PAGE_SIZE is always a multiple of
 *       \ref AT45DBX_SECTOR_SIZE, there is no need to check page end for each
 *       byte.
 */
extern bool at45dbx_write_multiple_sector(uint16_t nb_sector);

/*! \brief Callback function invoked before each sector write during
 *         \ref at45dbx_write_multiple_sector.
 *
 * \param psector Pointer to sector to write.
 */
extern void at45dbx_write_multiple_sector_callback(void *psector);

//! @}


/*! \name Single-Sector Access Functions
 */
//! @{

/*! \brief Reads one DataFlash sector to a RAM buffer.
 *
 * Data flow is: DataFlash -> RAM.
 *
 * \param ram Pointer to RAM buffer.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note First call must be preceded by a call to the \ref at45dbx_read_sector_open
 *       function.
 */
extern bool at45dbx_read_sector_to_ram(void *ram);

/*! \brief Writes one DataFlash sector from a RAM buffer.
 *
 * Data flow is: RAM -> DataFlash.
 *
 * \param ram Pointer to RAM buffer.
 *
 * \retval OK Success.
 * \retval KO Failure.
 *
 * \note First call must be preceded by a call to the \ref at45dbx_write_sector_open
 *       function.
 */
extern bool at45dbx_write_sector_from_ram(const void *ram);

//! @}

/*! \name Functions to connect the DataFlash driver with the SPI Multiple-Sector Access Functions
 */
//! @{	
extern void at45dbx_spi_init(void);
extern void at45dbx_spi_select_device(uint8_t mem_id);
extern void at45dbx_spi_deselect_device(uint8_t mem_id);
extern void at45dbx_spi_write_byte(uint8_t data);
extern void at45dbx_spi_read_byte(uint8_t *data);
extern void at45dbx_spi_read_packet(void const *data, size_t len);
extern void at45dbx_spi_write_packet(void const *data, size_t len);
//! @}

//! @}

#ifdef __cplusplus
}
#endif

#endif  // _AT45DBX_H_
