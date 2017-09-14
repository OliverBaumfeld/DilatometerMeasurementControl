/*=======================================================================
 *
 *                 Methods of class AH2550A
 * 
 *=======================================================================
 * Here the functions in ni4882.h are used for GPIB communication.
 * They are slightly different to the functions of decl-32.h which
 * is the old version of ni4882.h.
 */

#include "stdafx.h"
#include "ni4882.h"
#include "PPMSV0.1.h"
#include "PPMSV0.1Dlg.h"

char ErrorMnemonic[29][5] = { "EDVR", "ECIC", "ENOL", "EADR", "EARG",
                              "ESAC", "EABO", "ENEB", "EDMA", "",
                              "EOIP", "ECAP", "EFSO", "",     "EBUS",
                              "ESTB", "ESRQ", "",     "",      "",
                              "ETAB", "ELCK", "EARM", "EHDL",  "",
                              "",     "EWIP", "ERST", "EPWR" };

// where to declare thes constants???
// need to be available global
// define that shit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const int ERRID_IBDEV = 666;
const int ERRID_IBCLR = 55;
const int ERRID_IBONL = 56;

const int ERRID_WRITE = 1;
const int ERRID_READ = 2;

int AH2550A::Init( )
/* Sets GPIBdev with GPIB number PRIMARY_ADDR_OF_DMM
 * then clears device GPIBdev (ibclr)
 * Question: Is ibclr really necessary?
 * Return value: 0 on success, ERRID otherwise
 * Pitfalls: Initialization has to be successfull for all other
 *           methods to work because GPIBdev has to be defined/valid.
 */
{
	int nReturn = 0;
	// Open and initialize a device descriptor.
	GPIBdev = ibdev(BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
					TIMEOUT, EOTMODE, EOSMODE);
	if (Ibsta() & ERR)
		nReturn = ERRID_IBDEV;
	else
	{
		ibclr(GPIBdev);			// is this really necessary???
		if (Ibsta() & ERR)
			nReturn = ERRID_IBCLR;
	}
	return nReturn;
}

int AH2550A::Close()
/* Takes GPIBdev offline
 * Return value: 0 on success, ERRID otherwise
 */
{
	ibonl(GPIBdev, 0);
	if (Ibsta() & ERR)
		return ERRID_IBONL;
	else
		return 0;
}

int AH2550A::Write( string strCmd )
/* Writes strCmd to the device GPIBdev
 * Return value: 0 on success, ERRID_WRITE otherwise.
 * Possible Errors: GPIBdev not defined
 */
{
	// TO DO: Is there a maximum number of characters for the AH2550A???
	ibwrt(GPIBdev, strCmd.c_str(), strCmd.length() );
	if ( Ibsta() & ERR )
		return  ERRID_WRITE;
	else
		return 0;
}

string AH2550A::Read( )
/* reads response from GPIBdev
 * Return value: response message on success, empty string otherwise.
 * Possible Errors: GPIBdev not defined, device returns empty string
 */
{
	string strResponse = "";
	ibrd(GPIBdev, ReadBuffer, ARRAYSIZEAH2550);
	if ( !(Ibsta() & ERR) )
		strResponse.append(ReadBuffer, Ibcnt());
	return strResponse;
}

int AH2550A::MakeSingleMeasurement( double* const pdCapacitance, double* const pdLoss )
/* Sends "SI" to GPIBdev (like pressing the Single button on the front panel)
 * and reads the return value.
 * Return value: 0 on success, ErrorID otherwise;
 *               sets dCapacitance and dLoss on success
 *               sets both values to -1 otherwise
 *               
 * Possible Errors: GPIBdev not defined (Write and Read will fail)
 */
{
	if ( pdCapacitance && pdLoss )
	{
		// What happens if Write fails, what is read by Read?
		if ( Write( "SI" ) == ERRID_WRITE )
		{
			//so what???
		}

		string strResponse = Read();
		if ( strResponse != "")
		{
			// DEPENDS ON FORMAT OF RETURN STRING OF THE AH2550A!!!
			// what if the return string is too short?? (smaller than 33)
			string strTemp1 (strResponse, 2, 13);
			string strTemp2 (strResponse, 20, 13);

			*pdCapacitance = atof(strTemp1.c_str());
			*pdLoss = atof(strTemp2.c_str());
		}
		else
		{
			*pdCapacitance = -1;
			*pdLoss = -1;
			//strcpy(ErrorMsg, "Unable to read from device.");
			return 12;
		}
	}
	else
	{
		//strcpy(ErrorMsg, "Internal Error, pointers not set.");
		return 13;
	}

	return 0;
}

int AH2550A::SetAV( int AverageExponent )
/* Sets the Average Exponent to AverageExponent
 * return value: None
 */
{
	//  restrict Average Exponent range from 0 to 15
	if ( AverageExponent >= 0 && AverageExponent <= 15 )
	{
		stringstream ss;
		string strAV;
		ss << AverageExponent;
		ss >> strAV;

		if ( Write( "AV " + strAV ) == 1)
			return 1;
	}
	else
	{
		return 2;
	}
	return 0;
}

void AH2550A::SetRemote()
{
	//TO DO: find a way to get into remote mode
	//       maybe standard ni488.2 command?
	//     also unlock 
}

string AH2550A::GPIBErrorHandler()
/* Generates one error string from all the GPIB error functions
 * Return value: Said error string as CString
 */
{	
	//printf("GPIB Error : %s\nIbsta = 0x%x Iberr = %d (%s)\n", ErrorMsg, Ibsta(), Iberr(), ErrorMnemonic[Iberr()]);
	string ErrorMsg;
	switch (4) {
	case 11:
		ErrorMsg = "";
		break;
	case 12:
		ErrorMsg = "";
		break; 
	case 13:
		ErrorMsg = "";
		break;
	case 1:
		ErrorMsg = "";
		break;
	default:
		ErrorMsg = "Unknown Error.";
	}
	
	stringstream ss;
	ss << "GPIB Error:" << endl;
	ss << "Ibsta = " << Ibsta() << endl;
	ss << "Iberr = " << Iberr() << " " << ErrorMnemonic[Iberr()] << endl;
	ss << "Error Message: " << ErrorMsg;

	return ss.str();
}