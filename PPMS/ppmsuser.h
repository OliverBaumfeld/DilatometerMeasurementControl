// Ppmscomm.h
// Windows GPIB interface DLL
// Quantum Design 1996-2001


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


//#include "decl-32.h"
#include "ni4882.h"

/////////////////////////////////////
// Global errors
/////////////////////////////////////
typedef enum{
	NEW_EDEV = (ETAB + 1),	// NI GPIB errors = 0-ETAB 
	CMD_ERR,		// start ours after theirs
	MUTEX_TIMEOUT,		// could not acquire mutex
	MUTEX_ABANDONED,	// mutex was abandoned
	MUTEX_RELEASED,		// mutex was not released
	MUTEX_NOEXIST,		// mutex HANDLE = NULL;
	OP_INCOMPLETE,          // operation did not complete
	SEND_ERROR,             // command could not be sent
	DEVICE_CLR_ERROR,       // error caused a device clear
	LAST_SHARED_ERROR	// place holder, insert errors above.
}SHARED_ERRORS;



#define MAX_RESPONSE	4*1024	// maximum Model 6000 read buffer size


/////////////////////////////////////////////////////////////////////////////
//	Exported functions
/////////////////////////////////////////////////////////////////////////////

extern "C" _declspec(dllexport) int GpibSend
(
	int device,                // device number
	LPCSTR command,            // command buffer
	long bytesToSend,          // size of command
	LPSTR response,            // response buffer
	long responseSize,         // sizeof response buffer
	long *bytesRead,           // pointer to actual bytes read
	int *pPpmsError,           // command error
	LPSTR errorStr,            // error string
	long errorSize,            // sizeof error string
	int swap,                  // swap bytes (FALSE)
	DWORD mutexTimeout         // millisecond timeout for communication mutex
);

extern "C" _declspec(dllexport) HANDLE RegisterForAdvisories
(
	HWND hwnd,           // handle of window to receive advisory notification
	UINT message,        // id of return message
	LPARAM lParam        // application specific data 
);


extern "C" _declspec(dllexport) void UnregisterFromAdvisories
(
	HANDLE hTerminateThread		// The thread ID returned by RegisterForAdvisories
);

extern "C" _declspec(dllexport) int  GetCommunicationMutex
(
	DWORD timeout
);

extern "C" _declspec(dllexport) BOOL ReleaseCommunicationMutex( void );

/////////////////////////////////////////////////////////////////////////////

// Visual Basic Exported functions

//extern "C" _declspec(dllexport) 
//         long WINAPI PpmsSendRead(	BSTR bsWriteBuff,
//					BSTR bsReadBuff, 
//					BSTR bsErrBuff,
//					double dTimeout );
//extern "C" _declspec(dllexport) 
//         long WINAPI RegisterAdvisory( long hWindow, long lMsg, long lAppMsg );
//extern "C" _declspec(dllexport) 
//         long WINAPI UnregisterAdvisory( long hAdvisory );
//extern "C" _declspec(dllexport) 
//         long WINAPI GetExclusiveGpib( double dTimeout );
//extern "C" _declspec(dllexport) 
//         long WINAPI ReleaseExclusiveGpib( void );

#ifdef __cplusplus
}
#endif
