// --------------------------------------------------------------------------
// canlogger/output.h
//    Contains functions to pretty print the can frames onto the screen.
//
// Author: Christian Steiger <christian.steiger@stud.h-da.de>
// --------------------------------------------------------------------------

#ifndef __CANLOGGER_OUTPUT_H__
#define __CANLOGGER_OUTPUT_H__

// TODO: Add documentation

#include <stdio.h>
#include <stdint.h>
#include <linux/can.h>

extern void output_canframe
(
	                  FILE *file,
	const struct can_frame *frame,
	const          uint8_t  format
);

#endif
