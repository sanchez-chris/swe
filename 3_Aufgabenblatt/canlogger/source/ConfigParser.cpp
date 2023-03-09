// --------------------------------------------------------------------------
// ConfigParser.cpp
//    Interprets commandline and config file options.
//
// Author: Christian Steiger <christian.steiger@stud.h-da.de>
// --------------------------------------------------------------------------

// TODO: Rework or replace this entire file.

#include "ConfigParser.h"

ConfigParser::ConfigParser()
{
	// Set defaults
	m_can_device = 0;
	m_show_or_hide = 0;
	m_driver = 0;
	m_format = 0;

	setCanDevice("default");
}

ConfigParser::~ConfigParser()
{
	// safe delete of this->can_device
	if ( m_can_device ) {
		delete m_can_device;
	}
}

//============================================================================
// Summary: Read and interpret the command line parameters
// Parameters:
// [in] argc - Number of parameters (just copy the main-function parameter)
// [in] *argv[] - The parameters (just copy the main-function parameter)
// [out] 0 = no problems occured
//============================================================================
char ConfigParser::parseCommandLine(const int argc, const char* const argv[])
{
	// declare variables
	int i;
	int clear_canid_vectors = 1;

	// go through the argument, ignore the first one
	for( i = 1; i < argc; i++ )
	{
		// check the arguments first and third char, is it a config parameter?
		if ( argv[i][0] == '-' )
		{
			// check which config parameter it is
			switch( argv[i][1] )
			{
				// read the configuration file
				case 'i':
					this->parseConfigFile(&argv[i][3]);
					break;

				// set can-device
				case 'd':
				case 'f':
					this->setCanDevice(&argv[i][3]);
					break;

				// use extended frames
				case 'e':
					this->setFrameMode(1);
					break;

				// set the print format
				case 'p':
					this->setPrintFormat( atoi(&argv[i][3]) );
					break;

				// show filter
				case 's':
					// kill can_id_vectors?
					if ( clear_canid_vectors ) {
						m_can_id_show.clear();
						m_can_id_hide.clear();
						clear_canid_vectors = 0;
					}
					// hex value or integer?
					if ( argv[i][4] == 'x' )
						m_can_id_show.push_back( strtoul(&argv[i][5], 0, 16) );
					else
						m_can_id_show.push_back( strtoul(&argv[i][3], 0, 10) );
					break;

				// hide filter
				case 'h':
					// kill can_id_vectors?
					if ( clear_canid_vectors ) {
						m_can_id_show.clear();
						m_can_id_hide.clear();
						clear_canid_vectors = 0;
					}
					// hex value or integer?
					if ( argv[i][4] == 'x' )
						m_can_id_hide.push_back( strtoul(&argv[i][5], 0, 16) );
					else
						m_can_id_hide.push_back( strtoul(&argv[i][3], 0, 10) );
					break;

				// print help
				case '?':
					printf(
					"PeakCanLogger - a simple program which receives and prints CAN messages.\n\n"
					"usage:   canlogger [-f=devicenode] [-c] [-n] [-e] [-i=path]\n"
					"                   [-p=format] [-s=canmsgid] [-h=canmsgid] [-?]\n\n"
					"options: -f - sets the can-device to connect to (default is can0). \n"
					"         -e - accept extended frames. (default: standard frames)\n"
					"         -i - path to configuration-file (default ./canlogger.ini). \n"
					"         -p - sets the output format, available choices are: \n"
					"              0 - the same as in the pcan-driver receivetest sample (default).\n"
					"              1 - like 0, with semicolons as separators and pure hex values.\n"
					"              9 - the initial format used in our h_da project group.\n"
					"         -s - shows only messages with the given id. can be used multiple times,\n"
					"              will override settings in the configuration file.\n"
					"         -h - hides messages with the given id. can be used multiple times.  \n"
					"              hide parameters will be ignored if show parameters are present,\n"
					"              will override settings in the configuration file.\n"
					"         -? - this help. \n"
					"\n"
					);
					exit(0);
					break;
			}
		}
	}

	// set show_or_hide for decision making in isVisible.
	if (!clear_canid_vectors)
		this->setShowOrHide();

	// No problems occured
	return 0;
}



//============================================================================
// Summary: Read and interpret a configuration file
// Parameters:
// [in] file - Name and path of the file
// [out] 0 = no problems occured
//============================================================================
char ConfigParser::parseConfigFile(const char* const file)
{
	// Declare variables
	std::ifstream config_file;
	std::string config_line;

	// Open the config file, input only
	config_file.open( file, std::ifstream::in );

	// Check if the filehandler is "good" (no problems occured)
	if ( config_file.good() )
	{
		// Loop until the end of file is reached
		while ( !config_file.eof() )
		{
			// Copy a line into the temporary variable temp
			char temp[256];
			temp[255] = 0;
			config_file.getline(temp, 254);

			// Quick check if the line is a comment,
			// skip the rest of the loop when true
			if ( temp[0] == '#' )
				continue;

			// TODO: the same can be done without strings and stringstream, optimize!

			// Copy temp into a string for easier handling
			config_line.assign(temp);

			// Line begins with "device"?
			if ( config_line.find("device") == 0 )
			{
				// remove "device=" from the string
				config_line.erase(0, 7);

				this->setCanDevice( config_line.c_str() );
			}

			// Line begins with "extended"?
			else if ( config_line.find("extended") == 0 )
			{
				// remove "extended=" from the string
				config_line.erase(0, 9);

				if ( config_line.c_str()[0] == '1' ) {
					this->setFrameMode(1);
				}
				else if ( config_line.c_str()[0] == '0' ) {
					this->setFrameMode(0);
				}

			}

			// Line begins with "format"?
			else if ( config_line.find("format") == 0 )
			{
				// remove "extended=" from the string
				config_line.erase(0, 7);

				this->setPrintFormat( atoi(config_line.c_str()) );

			}

			// Line begins with "show"?
			else if ( config_line.find("show") == 0 )
			{
				// remove "show=" from the string
				config_line.erase(0, 5);

				// temporary variables
				std::istringstream temp;
				int can_id;

				// hex value or not?
				if ( config_line.find("0x") == 0 ) {
					// remove "0x" from the string
					config_line.erase(0, 2);
					// fill the stringstream with the value
					temp.str(config_line);
					// convert it from string to int
					temp >> std::hex >> can_id;
					// add it to the show-vector
					m_can_id_show.push_back(can_id);
				}
				else {
					// fill the stringstream with the value
					temp.str(config_line);
					// convert it from string to int
					temp >> can_id;
					// add it to the show-vector
					m_can_id_show.push_back(can_id);
				}
			}

			// Line begins with "hide"?
			else if ( config_line.find("hide") == 0 )
			{
				// remove "show=" from the string
				config_line.erase(0, 5);

				// temporary variables
				std::istringstream temp;
				int can_id;

				// hex value or not?
				if ( config_line.find("0x") == 0 ) {
					// remove "0x" from the string
					config_line.erase(0, 2);
					// fill the stringstream with the value
					temp.str(config_line);
					// convert it from string to int
					temp >> std::hex >> can_id;
					// add it to the show-vector
					m_can_id_hide.push_back(can_id);
				}
				else {
					// fill the stringstream with the value
					temp.str(config_line);
					// convert it from string to int
					temp >> can_id;
					// add it to the show-vector
					m_can_id_hide.push_back(can_id);
				}
			}



		}

		// close the filehandler
		config_file.close();

		// Automatically set show_or_hide for decision making in isVisible.
		this->setShowOrHide();

		// no problems
		return 0;
	}

	// had some sort of problems while opening the file
	return -1;
}

//============================================================================
// Summary: Automatically set show_or_hide for decision making in isVisible.
// Parameters:
// none
//============================================================================
void ConfigParser::setShowOrHide( void )
{
	int show_size = m_can_id_show.size();
	int hide_size = m_can_id_hide.size();

	// Show everything
	if ( show_size == 0 && hide_size == 0 ) {
		m_show_or_hide =  0;
	}
	// Show only selected IDs
	else if	( show_size >= 1 ) {
		m_show_or_hide =  1;
	}
	// Hide only selected IDs, show everything else
	else {
		m_show_or_hide = -1;
	}
}

//============================================================================
// Summary: Manually set show_or_hide for decision making in isVisible.
// Parameters:
// [in] show_or_hide -
//		0 = Show everything
//		1 = Show only IDs from the can_id_show vector
//		-1 = Hide IDs from the can_id_hide vector, show everything else
//============================================================================
void ConfigParser::setShowOrHide( const char show_or_hide )
{
	m_show_or_hide = show_or_hide;
}

//============================================================================
// Summary: Sets the can device enumerator
// Parameters:
// [in] file - Name of the pcan-device
//============================================================================
void ConfigParser::setCanDevice( const char* const can_device )
{
	// create a new one
	int len = strlen(can_device);
	char * tmp = new char[len + 1];
	strncpy(tmp, can_device, len);
	tmp[len] = 0;

	// delete the old string if necessary
	if ( m_can_device )
		delete m_can_device;

	// copy pointer
	m_can_device = tmp;
}

//============================================================================
// Summary: Gets the can device enumerator
// Parameters:
// [out] file - Name of the pcan-device
//============================================================================
const char* ConfigParser::getCanDevice()
{
	if ( strcmp(m_can_device, "default") == 0 )
	{
		// Possible due to const char* return type
		return "can0";
	}

	return m_can_device;
}

//============================================================================
// Summary: Sets the extended frame attribute
// Parameters:
// [in] extended_frames -
//		0 = use only standard frames
//		1 = use extended frames
//============================================================================
void ConfigParser::setFrameMode( const char frame_mode )
{
	m_frame_mode = frame_mode;
}

//============================================================================
// Summary: Gets the extended frame attribute
// Parameters:
// [out] content of the extended_frames attribute
//============================================================================
char ConfigParser::getFrameMode( void )
{
	return m_frame_mode;
}

//============================================================================
// Summary: Sets the print format
// Parameters:
// [in] format -
//		0 = same format as in the pcan receivetest example
//		1 = like 0, but in pure hex numbers and with semikolon
//		9 = initial data format of the h_da can bus group
//============================================================================
void ConfigParser::setPrintFormat( const char format )
{
	m_format = format;
}

//============================================================================
// Summary: Gets the print format
// Parameters:
// [out] number of the actual print format
//============================================================================
char ConfigParser::getPrintFormat( void )
{
	return m_format;
}

//============================================================================
// Summary: Checks if a can id is visible or not.
// Parameters:
// [in] The can id
// [out] 1 if its visible, 0 when not
//============================================================================
char ConfigParser::isVisible( const int can_id )
{

	// Show everything
	if ( m_show_or_hide == 0 )
	{
		return 1;
	}

	// Show only selected IDs
	else if ( m_show_or_hide == 1 )
	{
		// Get a pointer to the first field in the vector
		std::vector<int>::iterator iter = m_can_id_show.begin();

		// go through every single field
		for( ; iter != m_can_id_show.end(); ++iter )
		{
			// check if the id match, return 1 when true
			if ( *iter == can_id )
				return 1;
		}
	}

	// Hide only selected IDs, show everything else
	else if ( m_show_or_hide == (char)-1 )
	{
		char found = 0;

		// Get a pointer to the first field in the vector
		std::vector<int>::iterator iter = m_can_id_hide.begin();

		// go through every single field
		for( ; iter != m_can_id_hide.end(); ++iter )
		{
			// check if the ids match, set found to 1 when true
			// and stop the for loop
			if ( *iter == can_id ) {
				found = 1;
				break;
			}
		}

		// if the id wasnt found, return 1
		if ( !found )
			return 1;
	}

	return 0;
}

//============================================================================
// Summary: Simply prints the whole configuration parameters
// Parameters:
// none
//============================================================================
void ConfigParser::printConfiguration ()
{
	std::cout << "device: " << this->getCanDevice() << std::endl;
	std::cout << "frame_mode: " << (int)this->getFrameMode() << std::endl;
	std::cout << "format: " << (int)this->getPrintFormat() << std::endl;

	// Get a pointer to the first field in the vector
	std::vector<int>::iterator iter;

	std::cout << "show: ";

	// go through every single field
	for(iter = m_can_id_show.begin(); iter != m_can_id_show.end(); ++iter )
		std::cout << *iter << "; ";

	std::cout << std::endl;

	std::cout << "hide: ";

	// go through every single field
	for(iter = m_can_id_hide.begin(); iter != m_can_id_hide.end(); ++iter )
		std::cout << *iter << "; ";

	std::cout << std::endl;
}






