/**
 * \file
 *
 * \brief FIFO buffer based on AT45DBX DataFlash component.
 *
 * Copyright (C) 2011 Metormote AB. All rights reserved.
 *
 */
#ifndef AT45DBX_FIFO_H_
#define AT45DBX_FIFO_H_

#include "at45dbx.h"

#ifdef __cplusplus
extern "C" {
#endif


//_____ D E C L A R A T I O N S ____________________________________________


//NOTE buffer size must be power of two
struct at45dbx_fifo_t {
    uint8_t  memidx;
    uint32_t head;        /* first byte of data */
    uint32_t tail;        /* last byte of data */
    uint32_t buffer_len;  /* length of the data */
};
typedef struct at45dbx_fifo_t AT45DBX_FIFO;


/*! \name Control Functions
 */
//! @{

/*! \brief Initializes the data flash controller and the SPI channel by which
 *         the DF is controlled.
 *
 * \retval OK Success.
 * \retval KO Failure.
 */
bool at45dbx_fifo_init(AT45DBX_FIFO *fifo);
uint16_t at45dbx_fifo_read(uint8_t *buf, uint16_t offset, uint16_t len);
uint16_t at45dbx_fifo_write(uint8_t *buf, uint16_t offset, uint16_t len);


//! @}

#ifdef __cplusplus
}
#endif


#endif /* AT45DBX_FIFO_H_ */