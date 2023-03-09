// --------------------------------------------------------------------------
// CanLogger.cpp
//    A simple program which receives and prints CAN messages.
//
// Author: Christian Steiger <christian.steiger@stud.h-da.de>
// --------------------------------------------------------------------------
// TODO: Add documentation, fix more stuff

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include "../include/libcanio.h"
#include "../include/canlogger/output.h"
#include "ConfigParser.h"

// Main function
int main(int argc, char *argv[])
{
	// create configparser object
	ConfigParser config;

	// read default config file
	config.parseConfigFile("canlogger.ini");
	errno = 0; // TODO: Not nice practice, test for errors first or throw

	// read command line parameters,
	// parameters here override parameters from the config file
	config.parseCommandLine(argc, argv);

	// copy the driver value
	const char *device = config.getCanDevice();

	// Open a connection to an can interface
	CanIO canio;
	canio.connect(device);

	struct timeval timevalue;
	struct can_frame frame;

	do
	{
		// Read a message from the can bus
		canio.readMsg(&frame, &timevalue);

		// Filter the Can-ID out of the frame
		int const can_id  = frame.can_id & CAN_EFF_MASK;
		int const visible = config.isVisible(can_id);

		// Skip if we don't want to see this can-id
		if (visible == 1)
		{
			// Fetch the output format
			const uint8_t format  = config.getPrintFormat();

			// Print the can frame
			output_canframe(stdout, &frame, format);
		}
	}
	while (1);
}

