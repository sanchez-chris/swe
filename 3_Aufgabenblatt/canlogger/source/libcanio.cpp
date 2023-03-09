//============================================================================
// Name:
//   libcanio.cpp
//
// Summary:
//   A pretty simple interface to the SocketCan layer,
//   which trys to make the communication with the interfaces a bit easier.
//
// Created on:
//   Sep 24, 2010
//
// Author:
//   Christian Steiger
//============================================================================


// Includes
#include "../include/libcanio.h"

// Constructor
CanIO::CanIO()
{
	m_socket = 0;
	m_print_errors = 1;
	m_filter = 0;
	m_filter_size = 0;
}

// Destructor
CanIO::~CanIO()
{
	disconnect();
}

//============================================================================
// Summary: Connects to an socket can interface
// Parameters:
// [in] interface - name of the can interface, e.g. can0, vcan0
// [out] errno if something bad happened
//============================================================================
int CanIO::connect(const char* const interface)
{
	// connect to the adapter
	struct sockaddr_can addr;
	struct ifreq ifr;
	int ret;

	// reset errno
	errno = 0;

	// create a seocket handler
	m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	// invalid socket_handler?
	if (m_socket == -1) {
		if (m_print_errors)
			perror("CanIO::connect, socket");
		return errno;
	}

	// talk with the adapter directly
	strcpy(ifr.ifr_name, interface);
	ioctl(m_socket, SIOCGIFINDEX, &ifr);

	// fill in the info we got from the ioctl call
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	// bind the socket to the device
	ret = bind(m_socket, (struct sockaddr *)&addr, sizeof(addr));

	// where there problems?
	if ( ret == -1 ) {
		if (m_print_errors)
			perror("CanIO::connect, bind");
		return errno;
	}

	return 0;
}

//============================================================================
// Summary: Disconnects from a socket can interface
// Parameters:
// [out] errno if something bad happened
//============================================================================
int CanIO::disconnect(void)
{
	// abort if there isnt a socket
	if (!m_socket)
		return 0;

	// close socket connection
	int ret = close(m_socket);

	// print
	if (ret == -1) {
		if (m_print_errors)
			perror("CanIO::disconnent, close");
		return errno;
	}

	// return the result from close
	return ret;
}

//============================================================================
// Summary:
// Reads a can message from the device. The read ist blocking,
// if there arent new messages it will wait until one is available!
// Parameters:
// [in] frame - pointer to a can_frame where the message data is stored.
//              take a look in /usr/include/linux/can.h for more details.
//              The message type is also in the can_id field, use the masks
//              CAN_EFF_FLAG, CAN_RTR_FLAG and CAN_ERR_FLAG to determine it.
// [in] timestamp - a pointer to a timeval struct, contains sec and usec.
// [out] errno if something bad happened
//============================================================================
int CanIO::readMsg(can_frame* frame, timeval* timestamp)
{
    // detect nullpointer
	if (!frame) {
		if (m_print_errors)
			fprintf(stderr, "CanIO::readMsg: frame is a nullpointer.\n");
		return -1;
	}

	// Set errno to zero
	errno = 0;

    // read messages from the can bus
    ssize_t nbytes;

    // read a can message from the bus
	nbytes = read(m_socket, frame, sizeof(struct can_frame));

	// check for errors
	if (nbytes < 0) {
		if (m_print_errors)
			perror("CanIO::readMsg, read");
		return errno;
	}

	// paraniod check ...
	if (nbytes < (signed int)sizeof(struct can_frame)) {
		if (m_print_errors)
			fprintf(stderr, "CanIO::readMsg, read: incomplete CAN frame.\n");
		return errno;
	}

	// check if a timestamp is wanted
	if (timestamp) {
		ioctl(m_socket, SIOCGSTAMP, timestamp);
	}

	// everything ok, return 0
	return 0;
}


//============================================================================
// Summary:
//   Trys to write a can message to the device.
// Parameters:
// [in] pointer to a can_frame struct which contains the message_data
// [in] frame - pointer to a can_frame struct where the message data is stored.
//              take a look in /usr/include/linux/can.h for more details.
//              The message type is also in the can_id field, use the masks
//              CAN_EFF_FLAG, CAN_RTR_FLAG and CAN_ERR_FLAG to set it.
// [out] int - errno if something bad happened
//============================================================================
int CanIO::writeMsg(const can_frame* const frame)
{
    // detect nullpointer
	if (!frame) {
		if (m_print_errors)
			fprintf(stderr, "CanIO::writeMsg: frame is a nullpointer.\n");
		return -1;
	}

	// set errno to zero
	errno = 0;

	// try to write to the device
	int ret = write(m_socket, frame, sizeof(struct can_frame));

	// return the error, if any happened
	return ret;
}

//============================================================================
// Summary:
//   Dont print error messages on stderr.
//   Useful if you do want to to the errorhandling yourself.
// Parameters:
//   none
//============================================================================
void CanIO::hideErrors(void) {
	m_print_errors = 0;
}

//============================================================================
// Summary:
//   Print error messages on stderr.
//   Useful for debugging purposes, enabled per default.
// Parameters:
//   none
//============================================================================
void CanIO::showErrors(void) {
	m_print_errors = 1;
}


