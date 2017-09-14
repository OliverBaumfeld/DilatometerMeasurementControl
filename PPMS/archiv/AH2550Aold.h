/*=============================================================
 *   Header for communicating with hte AH2550
 *=============================================================
 */

//#include "ni4882.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class AH2550A 
{
private:
	int GPIBnumber;
	int Write( string strCommand );
	string Read();
	void GPIBCleanup(int Dev, const char * ErrorMsg);

	// parameters of the AH2550A
	static const int BDINDEX             =  0    ; // Board Index
	static const int PRIMARY_ADDR_OF_DMM =  28   ; // Primary address of device
	static const int NO_SECONDARY_ADDR   =  0    ; // Secondary address of device
	static const int TIMEOUT             =  13	; // Timeout value = 10 seconds
	static const int EOTMODE             =  1    ; // Enable the END message
	static const int EOSMODE             =  0    ; // Disable the EOS mode

	static const int ARRAYSIZEAH2550 = 100; // Size of read buffer
	char ReadBuffer[ARRAYSIZEAH2550 + 1];   // Read data buffer
	//static const int WRITEBUFFERSIZE = 30;  //
	//char WriteBuffer[WRITEBUFFERSIZE+1];    //
	
public:
	void SetAV( int AverageExponent );
	void SendGPIBCommand( string strCommand );
	void SetRemote();
	void MakeSingleMeasurement( double* const Capacitance,  double* const dLoss );
	int Init();
	int Close();

};