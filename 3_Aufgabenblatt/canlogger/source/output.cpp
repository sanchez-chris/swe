// --------------------------------------------------------------------------
// output.c
//    Contains functions to pretty print the can frames onto the screen.
//
// TODO: Still not too happy with this file, further rework needed.
//
// Author: Christian Steiger <christian.steiger@stud.h-da.de>
// --------------------------------------------------------------------------

#include <stdio.h>             // printf
#include <stdint.h>            // uint8_t and consorts
#include <linux/can.h>         // struct can_frame

// TODO: Get rid of this Header
#include <sys/time.h>          // DEPRECATED: gettimeofday, timeval

#include "../include/canlogger/output.h"  // ...

// Prints the data bytes of a can frame
static void output_databytes
(
	                  FILE *file,
	const struct can_frame *frame,
	const             char *format
)
{
	// Don't print any frame contents for remote frames,
	if (frame->can_id & CAN_RTR_FLAG) { return; }

	// Limit the possible data values to 8 to avoid bugs
	const uint8_t count = (frame->can_dlc > 8) ? 8 : frame->can_dlc;

	// Loop through the data bytes
	uint8_t i;
	for (i = 0; i < count; i++)
	{
		fprintf(file, format, frame->data[i]);
	}
}


// prints a can message
void output_canframe
(
	                  FILE *file,
	const struct can_frame *frame,
	const          uint8_t  format
)
{
	// TODO: Replace this with something thats not deprecated
	struct timeval timevalue;
	gettimeofday(&timevalue, 0);

	// --------------------------------------------------------
	// The initial data format of the h_da can bus group,
	// don't use it, it's just there for compatibility
	// --------------------------------------------------------
	if (format == 9)
	{
		// Get the can frame flags out of the id field
		const uint8_t message_flags = (frame->can_id >> 28);

		// print timestamp, usec, messagetype, can-id and datalength
		fprintf(
			file,
			"%ld;%ld;%x;%x;%x;",
			timevalue.tv_sec,
			timevalue.tv_usec,
			message_flags,
			frame->can_id & CAN_EFF_MASK,
			frame->can_dlc
		);

		// Print the data-bytes
		output_databytes(file, frame, "%x; ");
	}
	// --------------------------------------------------------
	// Structured like the default print format,
	// but with semicolons and pure hex numbers
	// --------------------------------------------------------
	else if (format == 1)
	{
		// Get the can frame flags out of the id field
		const uint8_t message_flags = (frame->can_id >> 28);

		// print RTR, 11 or 29, CAN-Id and data-length
		fprintf(
			file,
			"%08lx; %05lx; %1x; %08x; %1x; ",
			timevalue.tv_sec,
			timevalue.tv_usec,
			message_flags,
			frame->can_id & CAN_EFF_MASK,
			frame->can_dlc
		);

		// Print the data-bytes
		output_databytes(file, frame, "%02x; ");
	}
	// --------------------------------------------------------
	// Structured like the default print format,
	// but with decimal values and no spaces.
	// Should be easy to interpret by other software
	// --------------------------------------------------------
	else if (format == 2)
	{
		// Get the can frame flags out of the id field
		const uint8_t message_flags = (frame->can_id >> 28);

		// print RTR, 11 or 29, CAN-Id and data-length
		fprintf(
			file,
			"%lu;%lu;%u;%u;%u;",
			timevalue.tv_sec,
			timevalue.tv_usec,
			message_flags,
			frame->can_id & CAN_EFF_MASK,
			frame->can_dlc
		);

		// Print the data-bytes
		output_databytes(file, frame, "%u;");
	}
	// --------------------------------------------------------
	// The default print format, human readable,
	// like in the pcan "receivetest" example
	// --------------------------------------------------------
	else
	{
		// Default types
		char frame_type = 's';
		char message_type = 'm';

		// Set the frame and message type if necessary
		if      (frame->can_id & CAN_EFF_FLAG) {   frame_type = 'e'; }
		if      (frame->can_id & CAN_ERR_FLAG) { message_type = 'x'; }
		else if (frame->can_id & CAN_RTR_FLAG) { message_type = 'r'; }

		// print RTR, 11 or 29, CAN-Id and data-length
		fprintf(
			file,
			"%010ld %06ld  %c %c 0x%08x %1d  ",
			timevalue.tv_sec,
			timevalue.tv_usec,
			message_type,
			frame_type,
			frame->can_id & CAN_EFF_MASK,
			frame->can_dlc
		);

		// Print the data-bytes
		output_databytes(file, frame, "0x%02x ");
	}

	// Finish the output with a newline
	puts("");
}
