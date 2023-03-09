// --------------------------------------------------------------------------
// ConfigParser.h
//    Interprets commandline and config file options.
//
// Author: Christian Steiger <christian.steiger@stud.h-da.de>
// --------------------------------------------------------------------------

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

// Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>

// Klassendefinition
class ConfigParser
{
	public:
		ConfigParser();
		~ConfigParser();

		// the available functions
		char parseConfigFile(const char* const file);
		char parseCommandLine(const int argc, const char* const argv[]);
		char isVisible(const int can_id);
		void printConfiguration ();

		// getter and setter
		const char* getCanDevice();
		char getFrameMode();
		char getPrintFormat();

		void setPrintFormat(const char format);
		void setFrameMode(const char frame_mode);
		void setCanDevice(const char* const can_device);
		void setShowOrHide();
		void setShowOrHide(const char show_or_hide);

	protected:
		char               *m_can_device;
		std::vector<int>    m_can_id_show;
		std::vector<int>    m_can_id_hide;
		char                m_show_or_hide;
		char                m_frame_mode;
		unsigned short int  m_btr0btr1;
		char                m_driver;
		char                m_format;
};

#endif
