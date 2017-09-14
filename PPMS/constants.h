/*===================================================================
 *
 *                Declararion of constants, etc
 *
 *===================================================================
 */
long WM_ADVISORY = WM_USER + 0x0101;

const static CString PATH_CERNOX = _T("CernoxCalibration.dat");
const static CString PATH_LOGFILE = _T("DilaLog.txt");

/* Variables for the communication with the PPMS
   via the function GpibSend */
int device=15;             // device number
char command[4096];        // command buffer
long bytesToSend;          // size of command
char response[4096];       // response buffer
long responseSize=4096;    // sizeof response buffer
long bytesRead;            // pointer to actual bytes read
int pPpmsError;            // command error
char errorStr[256];        // error string
long errorSize=256;        // sizeof error string
int swapBytes = 0;         // swap bytes (FALSE)
DWORD mutexTimeout=10000;  // millisecond timeout for communication mutex

/* Variables for the communication with the capacitance bridge AH2550
   via the functions ibdev, ibwrt, ibrd, ... */
int GPIBdev;	// device descriptor

static const int BDINDEX             =  0    ; // Board Index
static const int PRIMARY_ADDR_OF_DMM =  28   ; // Primary address of device
static const int NO_SECONDARY_ADDR   =  0    ; // Secondary address of device
static const int TIMEOUT             =  13	 ; // Timeout value = 13 seconds
static const int EOTMODE             =  1    ; // Enable the END message
static const int EOSMODE             =  0    ; // Disable the EOS mode

static const int ARRAYSIZEAH2550 = 100; // Size of read buffer
char ReadBuffer[ARRAYSIZEAH2550 + 1];   // ibrd read data buffer

/* GPIB Iberr error codes */
char ErrorMnemonic[29][5] = { "EDVR", "ECIC", "ENOL", "EADR", "EARG",
                              "ESAC", "EABO", "ENEB", "EDMA", "",
                              "EOIP", "ECAP", "EFSO", "",     "EBUS",
                              "ESTB", "ESRQ", "",     "",      "",
                              "ETAB", "ELCK", "EARM", "EHDL",  "",
                              "",     "EWIP", "ERST", "EPWR" };

/* Constants for the GPIB error handler */
const int ERRID_WRITE = 1;
const int ERRID_READ = 2;
const int ERRID_PPMSRESPONSE = 3;
const int ERRID_OUTPUTFILE = 4;
const int ERRID_AH2550RESPONSE = 5;
const int ERRID_IBDEV = 666;
const int ERRID_IBCLR = 667;
const int ERRID_IBONL = 668;

/* for the output save file */
const int OUTPUT_APPEND = 100;
const int OUTPUT_OVERWRITE = 200;