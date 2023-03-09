//============================================================================
// Name:
//   libcanio.h
//
// Summary:
//   A pretty simple interface to the SocketCan layer,
//   which trys to make the communication with the can interfaces easier.
//
// Created on:
//   Sep 24, 2010
//
// Author:
//   Christian Steiger
//============================================================================

#ifndef LIBCANIO_H_
#define LIBCANIO_H_

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <fcntl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/time.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
#include <linux/sockios.h>

class CanIO
{
	public:
		CanIO();
		virtual ~CanIO();

		int connect(const char* const interface = "can0");
		int disconnect(void);
		int readMsg(can_frame* frame, timeval* timestamp = 0);
		int writeMsg(const can_frame* const frame);
		//int addFilter(const can_filter* const filter, const int size);
		//int clearFilter(void);

		void hideErrors(void);
		void showErrors(void);

	protected:
		int		m_socket;
		char	m_print_errors;
		can_filter*	m_filter;
		unsigned short int m_filter_size;
};

#endif
