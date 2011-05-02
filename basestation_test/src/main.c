/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * AVR Software Framework (ASF).
 */
#include <asf.h>
#include "at45dbx.h"
#include "conf_board.h"

int main (void)
{
	sysclk_init();
	
	board_init();
	
	at45dbx_init();
	
	// Insert application code here, after the board has been initialized.
}
