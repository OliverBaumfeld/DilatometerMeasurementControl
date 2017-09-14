/*=============================================================
 *   Class definition AH2550A
 *   Header file
 *=============================================================
 */



#include <string>
#include <sstream>

//#include <stdlib.h>
//#include <stdio.h>
//#include <conio.h>
//#include <iostream>
//#include <fstream>

using namespace std;

class AH2550A 
{
private:
	// private variables :
	int GPIBdev;	// device descriptor
	// parameters of the AH2550A
	static const int BDINDEX             =  0    ; // Board Index
	static const int PRIMARY_ADDR_OF_DMM =  28   ; // Primary address of device
	static const int NO_SECONDARY_ADDR   =  0    ; // Secondary address of device
	static const int TIMEOUT             =  13	 ; // Timeout value = 10 seconds
	static const int EOTMODE             =  1    ; // Enable the END message
	static const int EOSMODE             =  0    ; // Disable the EOS mode

	static const int ARRAYSIZEAH2550 = 100; // Size of read buffer
	char ReadBuffer[ARRAYSIZEAH2550 + 1];   // Read data buffer

	// private functions :
	int Write( string strCommand );
	string Read();
	
	
public:
	int Init();
	void SetRemote();
	int Close();
	int SetAV( int AverageExponent );
	int MakeSingleMeasurement( double* const Capacitance,  double* const dLoss );
	string GPIBErrorHandler();
};