
#include "stdafx.h"
#include "ni4882.h"
#include "AH2550A.h"

char ErrorMnemonic[29][5] = { "EDVR", "ECIC", "ENOL", "EADR", "EARG",
                              "ESAC", "EABO", "ENEB", "EDMA", "",
                              "EOIP", "ECAP", "EFSO", "",     "EBUS",
                              "ESTB", "ESRQ", "",     "",      "",
                              "ETAB", "ELCK", "EARM", "EHDL",  "",
                              "",     "EWIP", "ERST", "EPWR" };


int AH2550A::Init( )
{

	GPIBnumber = ibdev(BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
					TIMEOUT, EOTMODE, EOSMODE);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIBnumber, "Unable to open device");
		return 1;
	}
	/* Clear the internal or device functions of the device.  If the error
	 * bit ERR is set in ibsta, call GPIBCleanup with an error message. */
	else
	{
		ibclr(GPIBnumber);
		if (Ibsta() & ERR)
		{
			GPIBCleanup(GPIBnumber, "Unable to clear device");
			return 1;
		}
	}
	return 0;
}

int AH2550A::Close()
{
	ibonl(GPIBnumber, 0);
	cout << "Device closed\n";
	return 0;
}

int AH2550A::Write( string strCmd )
{
	// TO DO: Is there a maximum number of characters for the AH2550A???
	int iReturn = 0;

	ibwrt(GPIBnumber, strCmd.c_str(), strCmd.length() );
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIBnumber, "Unable to write to device");
		iReturn = 1;
	}
	return iReturn;
}

string AH2550A::Read( )
{
	// read response of the AH2550
	string strResponse = "";
	ibrd(GPIBnumber, ReadBuffer, ARRAYSIZEAH2550);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIBnumber, "Unable to read data from device");
	}
	else
	{
		strResponse.append(ReadBuffer, ibcnt);
	}

	return strResponse;
}

void AH2550A::MakeSingleMeasurement( double* const dCapacitance, double* const dLoss )
{
	if ( dCapacitance && dLoss )
	{
		string strResponse;

		Write( "SI" );
		strResponse = Read();
	
		// DEPENDS ON FORMAT OF RETURN STRING OF THE AH2550A!!!
		string strTemp1 (strResponse, 2, 13);
		string strTemp2 (strResponse, 20, 13);

		*dCapacitance = atof(strTemp1.c_str());
		*dLoss = atof(strTemp2.c_str());

		//cout << "Return value: " << strResponse << endl;
	}
}

void AH2550A::GPIBCleanup(int ud, const char * ErrorMsg)
{
	// where is it printed to ???
   printf("Error : %s\nibsta = 0x%x iberr = %d (%s)\n",
          ErrorMsg, Ibsta(), Iberr(), ErrorMnemonic[Iberr()]);
   if (ud != -1)
   {
      printf("Cleanup: Taking device offline\n");
      ibonl(ud, 0);
   }
}

void AH2550A::SetAV( int AverageExponent )
{
	//  restrict Average Exponent range from 0 to 15
	if ( AverageExponent >= 0 && AverageExponent <= 15 )
	{
		stringstream ss;
		string strAV;
		ss << AverageExponent;
		ss >> strAV;

		Write( "AV " + strAV );
	}
	else
	{
		// TO DO: raise some error message ?
	}
}

void AH2550A::SendGPIBCommand( string strCommand )
{
	//Write( strCommand );
	//error handling ???
	// or just make write public
}

void AH2550A::SetRemote()
{
	//TO DO: find a way to get into remote mode
	//       maybe standard ni488.2 command?
	//     also unlock 
}

