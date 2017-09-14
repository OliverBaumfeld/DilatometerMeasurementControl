/*======================================================================
 *               
 *                  PPMSDialog.cpp : implementation file
 *       
 * This is the main file of the application. All the interesting stuff is 
 * going on in here.
 *      
 * Content:
 *     - Help dialog
 *     - FileExistsDialog
 *     - CAboutDlg
 *     - CPPMSDialog dialog, the main dialog of the app.
 *=======================================================================
 */        

#include "stdafx.h"
#include "afxdialogex.h"
#include "PPMSApp.h"
#include "PPMSDialog.h"
#include "CalcGap.h"

#include "constants.h"
#include "ppmsuser.h"

#include <cmath>
#include <iomanip>
#include <iterator>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

/*===========================================================================
 *                             Help dialog
 *===========================================================================
 */

IMPLEMENT_DYNAMIC(CHelpDialog, CDialogEx)

CHelpDialog::CHelpDialog(CString strHelpText, CWnd* pParent /*=NULL*/)
	: CDialogEx(CHelpDialog::IDD, pParent)
	, m_HelpText( strHelpText )
{
}

CHelpDialog::~CHelpDialog()
{
}

void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HELPBOX, m_HelpText);
	DDX_Control(pDX, IDC_HELPBOX, m_HelpCEdit);
}


BEGIN_MESSAGE_MAP(CHelpDialog, CDialogEx)
END_MESSAGE_MAP()

// CHelpDialog message handlers

BOOL CHelpDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_font.CreatePointFont(90, _T("Courier New"));
	m_HelpCEdit.SetFont(&m_font);
	m_HelpCEdit.SetFocus();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*===========================================================================
 *                       FileExistsDialog dialog
 *===========================================================================
 */

IMPLEMENT_DYNAMIC(FileExistsDialog, CDialog)

FileExistsDialog::FileExistsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(FileExistsDialog::IDD, pParent)
{
}

FileExistsDialog::~FileExistsDialog()
{
}

void FileExistsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FileExistsDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &FileExistsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPEND, &FileExistsDialog::OnBnClickedAppend)
	ON_BN_CLICKED(IDC_OVERWRITE, &FileExistsDialog::OnBnClickedOverwrite)
END_MESSAGE_MAP()

// FileExistsDialog message handlers
void FileExistsDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void FileExistsDialog::OnBnClickedAppend()
{
	EndDialog(OUTPUT_APPEND);
}

void FileExistsDialog::OnBnClickedOverwrite()
{
	EndDialog(OUTPUT_OVERWRITE);
}


/*===========================================================================
 *                CAboutDlg dialog  (unused)
 *===========================================================================
 */
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


/*===========================================================================
 *
 *                CPPMSDialog dialog, the main dialog
 *
 *===========================================================================
 */

CPPMSDialog::CPPMSDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPPMSDialog::IDD, pParent)
	, m_SaveFile(_T(""))
	, m_DisplayStatus(_T("Please set output files.\r\n"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPPMSDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SAVEFILE, m_SaveFile);
	DDX_Text(pDX, IDC_DISPLAYSTATUS, m_DisplayStatus);
	DDX_Control(pDX, IDC_CHECK1, m_CernoxSensor);
}

BEGIN_MESSAGE_MAP(CPPMSDialog, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SAVEFILE_BTN, &CPPMSDialog::OnBnClickedSavefileBtn)
	ON_BN_CLICKED(IDOK, &CPPMSDialog::OnOK)
	ON_BN_CLICKED(IDC_HELP_BTN, &CPPMSDialog::OnBnClickedHelpBtn)
	ON_BN_CLICKED(IDC_BTN_CALCULATIONS, &CPPMSDialog::OnBnClickedBtnCalculations)
END_MESSAGE_MAP()


// CPPMSDialog message handlers

BOOL CPPMSDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	nReceivedAdvisories = 0;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	/* Set GPIBdev with GPIB number PRIMARY_ADDR_OF_DMM.
	   Initialization has to be successfull because GPIBdev has to be defined/valid.
	   The Average Exponent is set to 5.
	   */

	GPIBdev = ibdev(BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
					TIMEOUT, EOTMODE, EOSMODE);
	if ( Ibsta() & ERR )     
	{
		m_DisplayStatus = "Unable to open capacitance bridge.\r\n";
		m_DisplayStatus += "Initialisation cancelled.\r\n";
		m_DisplayStatus += "Fix problem and restart program.\r\n";

		AfxMessageBox(_T("Unable to open capacitance bridge. Initialisation cancelled."), MB_OK|MB_ICONSTOP);
		UpdateData(FALSE);
		return false;
	}
	else
	{
		m_DisplayStatus += "Capacitance bridge initialized.\r\n";
		ibclr(GPIBdev);		// Sends clear command to the capacitance bridge. Is this really necessary???
		if ( Ibsta() & ERR )
			m_DisplayStatus += "Unable to clear device, capacitance bridge.";
	}

	Sleep(100);	// wait to give the capacitance bridge some time 

	/* Set the Average Exponent to 5*/
	ibwrt(GPIBdev, "AV 5", 4 );
	if ( Ibsta() & ERR )
	{
		m_DisplayStatus += "Unable to set Average Exponent.\r\n";
		ErrorHandler( ERRID_WRITE );
	}
	else
		m_DisplayStatus += "Average Exponent set to 5.\r\n";

	/* TODO: check if PPMS responds? Is this necessary? */

	m_hAdvisory = RegisterForAdvisories( this->m_hWnd, WM_ADVISORY, 0 );
	m_DisplayStatus += "Program ready to receive advisories.\r\n";
	
	CheckDlgButton(IDC_CHECK1, BST_CHECKED);  // Cernox sensor check box

	// Load Cernox calibration file and calculate the interpolation coefficients.
	// Cubic interpolation after Akima H.
	// The coefficients are stored in the arrays a0, a1, a2 and a3.
	// Set bCernoxLoaded to false - if the file can not be read
	//                            - if temperature and resistance column have not the same size
	//
	// Format of input_file containing the Cernox calibration data: 3 header lines and 2 columns.
	// The temperature in the first column and the resistance of the Cernox sensor in the second.
	bCernoxLoaded = false;
	ifstream fin;
	fin.open(PATH_CERNOX);
	if ( !fin )
		m_DisplayStatus += "Unable to read from Cernox calibration file.\r\n";
	else
	{
		int ii;
		string line;
		for (ii=0; ii<3; ii++) // skip the 3 header files
			getline(fin, line);
		double x;
		vector<double> vT, d, derivT;
		while (fin>>x)
		{
			vT.push_back(x);
			if (fin>>x)
				vR.push_back(x);
		}
		fin.close();

		if ( vT.size() != vR.size() )
			m_DisplayStatus += "Unable to read from Cernox calibration file.\r\n";
		else
		{
			int size = vT.size();
	
			// Calculate the estimate of the slope at each data point: d.
			// d[i-1] = d(i) i.e.: d[0] = d(-1) or d[2] = d(1)
			d.push_back(0);	// will be calculated below
			d.push_back(0); // will be calculated below
			for(ii=0; ii<size-1; ii++)
				d.push_back((vT[ii+1]-vT[ii])/(vR[ii+1]-vR[ii]));
			// Compute the slopes of the first two and the last two segments.
			d[0] = 3*d[2] - 2*d[3];
			d[1] = 2*d[2] - d[3];
			int k = d.size();
			d.push_back(2*d[k-1] - d[k-2]);
			d.push_back(3*d[k-1] - 2*d[k-2]);

			// Calculate the estimates of the derivative of vT: derivT.
			for(ii=0; ii<size; ii++)
				derivT.push_back((abs(d[ii+3]-d[ii+2])*d[ii+1] + abs(d[ii+1]-d[ii])*d[ii+2])/(abs(d[ii+3]-d[ii+2])+abs(d[ii+1]-d[ii])));

			// Compute the Akima coefficients a0, a1, a2 and a3.
			double deltaR;
			for(ii=0; ii<size-1; ii++)
			{
				deltaR = vR[ii+1] - vR[ii];
				a0.push_back(vT[ii]);
				a1.push_back(derivT[ii]);
				a2.push_back((3.*vT[ii+1] - deltaR*derivT[ii+1] - 3.*vT[ii] - 2.*deltaR*derivT[ii])/pow(deltaR,2));
				a3.push_back((-2.*vT[ii+1] + deltaR*derivT[ii+1] + 2.*vT[ii] + deltaR*derivT[ii])/pow(deltaR,3));
			}
			bCernoxLoaded = true;
			m_DisplayStatus += "Cernox calibration table loaded.\r\n";
		}
	}
	

/*	string line;
	ifstream CernoxFile;
	CernoxFile.open(PATH_CERNOX);
	if ( !CernoxFile )
		m_DisplayStatus += "Unable to read from Cernox calibration file.\r\n";
	else
	{
		// Get rid of header lines starting with '#' or '{'
		// Problem of the old solution: The first line with numbers is skipped as well.
		// Ugly solution: Reset to beginning of file and read it again.
		int jj=0;
		getline(CernoxFile, line);
		// count number of header lines
		while (line[0] == '#' || line[0] == '{')
		{	
			jj++;
			getline(CernoxFile, line);
		}
		CernoxFile.seekg(0, CernoxFile.beg);
		for (int ii=0; ii<jj; ii++)
			getline(CernoxFile, line);

		copy(istream_iterator<double>(CernoxFile), istream_iterator<double>(), back_inserter(vCernox));  // this line reads the file
		CernoxFile.close();
		m_DisplayStatus += "Cernox calibration table loaded.\r\n";
	}*/

	UpdateData(FALSE);

	return true;  // return true unless you set the focus to a control
}

void CPPMSDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon. For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPPMSDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPPMSDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CPPMSDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
/* Catches advisories from the PPMS */
{
	if (message == (UINT)WM_ADVISORY) 
	{
		nReceivedAdvisories++;
		ProcessAdvisory(message, wParam, lParam);
	}
	
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CPPMSDialog::ProcessAdvisory(UINT message, WPARAM wParam, LPARAM lParam)
/* This is the main part of the program. It defines how advisories are processed.
 * 'wParam' is the advisory number set in the PPMS MultiVu Sequence.
 */
{
	string strCapacitance = "";
	string strLoss = "";
	string strTimestamp, strTemperature, strField, strResistance, strPressure;
	size_t findPos;

	int ii, nEnd;
	double CernoxTemperature, CernoxResistance;

	// Pause the PPMS sequence until done processing (timeout = 5 min)
	strcpy( command, "HOLDOFF 1 5" ); 
	bytesToSend = strlen( command );
	GpibSend( device, command, bytesToSend, response, responseSize,
		&bytesRead, &pPpmsError, errorStr, errorSize, swapBytes,
		mutexTimeout );

	/* Display the number of advisories received so far and reset the advisory counter */
	if (wParam == 2)
	{
		CString strRA;
		strRA.Format(_T("%i"), nReceivedAdvisories);
		m_DisplayStatus += strRA;
		m_DisplayStatus += " advisories received. \r\n";
		nReceivedAdvisories = 0;
	}

	/* Make a single capacitance measurement and write the result to the output file */
	if ( wParam == 1 )
	{
		/* Request temperature, magnetic field and resisitivity (of the Cernox sensor) from the PPMS.
		   bit 1: temperature 2^1=2
		   bit 2: magnetic field 2^2=4
	       bit 8: user bridge channel 3 (ohm) 2^8=256
		   bit 19: sample space pressure 2^19=524288
		   Examples:
			   - Without sample space pressure:
			     262 = 2+4+256 -> Temperature(K), Magnetic Field(Oe), User Bridge Channel 3(Ohm)
			   - With sample space pressure:
			     524550 = 2+4+256+524288 -> Temperature(K), Magnetic Field(Oe), User Bridge Channel 3(Ohm), Sample Space Pressure (Volts/Unit)
			   - Without Cernox sensor (i.e. without User Bridge Channel 3)
			     524294 = 2+4+524288 -> Temperature(K), Magnetic Field(Oe), Sample Space Pressure */
		if ( m_CernoxSensor.GetCheck() == BST_CHECKED )
			strcpy(command, "GETDAT? 524550");
		else 
			strcpy(command, "GETDAT? 524294");

		bytesToSend = strlen( command );
		
		GpibSend( device, command, bytesToSend, response, responseSize,
			&bytesRead, &pPpmsError, errorStr, errorSize, swapBytes,
			mutexTimeout );

		/* Get Capacitance and Loss from the AH2550A. */
		ibwrt(GPIBdev, "SI", 2 ); // SI ... make a single capacitance measurement
		if ( Ibsta() & ERR )
		{
			ErrorHandler( ERRID_WRITE );

			// resume the M6000 sequence
			strcpy( command, "HOLDOFF -1" ); 
			bytesToSend = strlen( command );

			GpibSend( device, command, bytesToSend, response, responseSize,
				&bytesRead, &pPpmsError, errorStr, errorSize, swapBytes,
				mutexTimeout );
		
			return;
		}

		ibrd(GPIBdev, ReadBuffer, ARRAYSIZEAH2550); // read response
		if ( (Ibsta() & ERR) ) 
		{
			ErrorHandler( ERRID_READ );

			// resume the M6000 sequence
			strcpy( command, "HOLDOFF -1" );
			bytesToSend = strlen( command );

			GpibSend( device, command, bytesToSend, response, responseSize,
				&bytesRead, &pPpmsError, errorStr, errorSize, swapBytes,
				mutexTimeout );

			return;
		}

		/* The processing of the return string of the AH2550A depends on its format.
		   Here I assumed the default format as given in the manual of the AH2550A.
		   I also assume that the Loss is given in nanosiemens. */
		ReadBuffer[Ibcnt()+1] = '\0';
		//if ( ibcnt() >= 33 )
		//{
			string strCapacitance (ReadBuffer, 2, 13); // Gets 13 characters of ReadBuffer starting from position 2
			string strLoss (ReadBuffer, 20, 13); // Gets 13 characters of ReadBuffer starting from position 20
		//}
		//else
		//	ErrorHandler( ERRID_AH2550RESPONSE );
	
		/* Process the response of the PPMS.
		   The format of GETDAT? is: DataFlag, TimeStamp, DataElement1, DataElement2, ... */
		response[ bytesRead ] = 0;
		string strResponse = response;
		findPos = strResponse.find(',');
		if ( findPos != string::npos ) 
		{
			strResponse.erase ( 0 , findPos + 1 );
			findPos = strResponse.find(',');
			if ( findPos != string::npos ) 
			{
				strTimestamp = strResponse.substr(0, findPos);
				strResponse.erase ( 0 , findPos+  1 );
			}
			else
			{
				strTimestamp = "";
				strResponse.clear();
			}
		}
		else
			strResponse.clear();

		if ( strResponse.size() > 0 )
		{
			/* without sample space pressure */
			//findPos = strResponse.find(','); // start counting at 1
			//strTemperature = strResponse.substr( 0 , findPos ); //starts counting at 0
			//strResponse.erase ( 0 , findPos+1 ); // also delete the ,

			//findPos = strResponse.find(',');
			//strField = strResponse.substr(0, findPos );
			//strResponse.erase ( 0 , findPos+1 ); 

			//findPos = strResponse.find(';');
			//strResistance = strResponse.substr(0, findPos );

			/* with sample space pressure */
			findPos = strResponse.find(','); // start counting at 1
			strTemperature = strResponse.substr( 0 , findPos ); //starts counting at 0
			strResponse.erase ( 0 , findPos+1 ); // also delete the ','

			findPos = strResponse.find(',');
			strField = strResponse.substr(0, findPos );
			strResponse.erase ( 0 , findPos+1 ); 

			// Interpolate the temperature of the Cernox sensor using the coefficients a0, a1, a2 and a3.
			// Requires a0, a1, a2, a3 and vR --> bCernoxLoaded needs to be true
			// If the Cernox resitance is out of range CernoxTemperature is set to 0.
			if ( m_CernoxSensor.GetCheck() == BST_CHECKED && bCernoxLoaded)
			{
				findPos = strResponse.find(',');
				strResistance = strResponse.substr(0, findPos );
				strResponse.erase ( 0 , findPos+1 ); 

				CernoxResistance = atof(strResistance.c_str());
				
				bool InRange = true;
				ii=0;
				while( vR[ii] > CernoxResistance )
				{
					ii++;
					if( ii==vR.size() )
						InRange = false;  // resistance out of range
				}
				if (ii == 0)
					InRange = false; // resistance out of range

				if ( InRange )
				{
					ii = ii-1;
					double deltaX = (CernoxResistance-vR[ii]);
					CernoxTemperature = a0[ii] + a1[ii]*deltaX +  a2[ii]*pow(deltaX,2) + a3[ii]*pow(deltaX,3);
				}
				else
					CernoxTemperature = 0;							
			}
			else
			{
				strResistance = "";
				CernoxTemperature = 0;
			}

			findPos = strResponse.find(';');
			strPressure = strResponse.substr(0, findPos );
		}
		else
		{
			ErrorHandler( ERRID_PPMSRESPONSE );
			strTemperature = "";	//whatever
			strField = "";
			strResistance = "";
			CernoxTemperature = 0;
			strPressure = "";
		}

		/* Write the results to the output file. */
		std::ofstream OutputFile( m_SaveFile, ios_base::app ); // maybe faster to create it right away??
		if ( OutputFile.is_open() )
		{	
			//If this is changed make sure to also change the corresponding header in OnBnClickedSavefileBtn()
			OutputFile << strField << ", " << CernoxTemperature << ", " << strCapacitance << ", " << strLoss << ", ";
			OutputFile << strTemperature << ", "  << strResistance  << ", " << strTimestamp <<  ", " << strPressure << endl;
			OutputFile.close();
		}
		else
		{
			m_DisplayStatus += "Unable to write to measurement file.\r\n";
			ErrorHandler( ERRID_OUTPUTFILE );
		}
	}

	/* resume the PPMS sequence */
	strcpy( command, "HOLDOFF -1" );
	bytesToSend = strlen( command );

	GpibSend( device, command, bytesToSend, response, responseSize,
		&bytesRead, &pPpmsError, errorStr, errorSize, swapBytes,
		mutexTimeout );

	UpdateData( FALSE );
}


void CPPMSDialog::OnOK()
/* Called on clicking the Close button in the main dialog */
{
	// TODO: Add your specialized code here and/or call the base class
	ibonl(GPIBdev, 0);
	if (Ibsta() & ERR)
		ErrorHandler( ERRID_IBONL );

	if ( !m_hAdvisory ) 
		UnregisterFromAdvisories( m_hAdvisory );

	CDialog::OnOK();	// Default
}


void CPPMSDialog::OnBnClickedSavefileBtn()
{	
	string strFileHeader;
	// If this file header is changed make shure it is still handled correctly by "calculate gap".
	strFileHeader = "#Field(Oe), CernoxT(K), Capacitance(pF), Loss(nS),  PPMST(K), CernoxR(Ohm), timestamp(s), Pressure(Units/Volts)\r\n";

	CFileDialog SaveDlg(FALSE, _T(".txt"), _T("DilatometerDefault.txt"), OFN_NOCHANGEDIR, _T("Text Files (*.txt)|*.txt|") _T("All Files|*.*||"));
	if ( SaveDlg.DoModal() == IDOK )
	{
		m_SaveFile = SaveDlg.GetPathName();

		fstream OutputFile(m_SaveFile, ios_base::in);	// try to open file

		if ( OutputFile.is_open() ) 
		{
			/* The file already exists. Ask the user if he wants to overwrite or append the file.
				On Cancel replace m_SaveFile with an empty string. */
			OutputFile.close();
			UpdateData(FALSE); // display file name (m_SaveFile) in edit box

			FileExistsDialog dlgFED;
			OutPutMode = dlgFED.DoModal(); // return values: IDCANCEL, OUTPUT_OVERWRITE or OUTPUT_APPEND

			if (OutPutMode == IDCANCEL )
			{
				m_SaveFile = "";
				UpdateData(FALSE);
				return;
			}
			else if ( OutPutMode == OUTPUT_OVERWRITE)
				OutputFile.open(m_SaveFile, ios_base::out | ios_base::trunc );
			else if ( OutPutMode == OUTPUT_APPEND)
			{
				OutputFile.open(m_SaveFile, ios_base::out | ios_base::app );
				strFileHeader = "";
			}
		}
		else	// the file does not exist yet, create it
			OutputFile.open(m_SaveFile, ios_base::out );

		if ( OutputFile.is_open() )
		{
			OutputFile << strFileHeader;
			OutputFile.close();
			m_DisplayStatus += "Measurement file ready.\r\n";
		}
		else
			m_DisplayStatus += "Unable to write to measurement file.\r\n";
	}

	UpdateData(FALSE);
}


void CPPMSDialog::ErrorHandler(int intErrorID)
/* Appends strErrorMessage according to intErrorID to the log file.
 * Return value: None
 */
{
	//fstream OutputLogFile(PATH_LOGFILE, ios_base::out | ios_base::app );
	ofstream OutputLogFile;
	OutputLogFile.open(PATH_LOGFILE, ios::app);
	if ( !OutputLogFile )
	{
		m_DisplayStatus += "Unable to write to log file.\r\n";
		UpdateData(FALSE);
		return;
	}
	string strErrorMessage;

	switch ( intErrorID ) {
	case ERRID_IBDEV:
		strErrorMessage = "Unable to open AH2550A. ";
		strErrorMessage += ReturnGPIBError();
		break;
	case ERRID_IBCLR:
		strErrorMessage = "Unable to clear AH2550A. ";
		strErrorMessage += ReturnGPIBError();
		break;
	case ERRID_IBONL:
		strErrorMessage = "Unable to close AH2550A. ";
		strErrorMessage += ReturnGPIBError();
		break;
	case ERRID_WRITE:
		strErrorMessage = "Unable to write to AH2550A.";
		strErrorMessage += ReturnGPIBError();
		break;
	case ERRID_READ:
		strErrorMessage = "Unable to read from AH2550A.";
		strErrorMessage += ReturnGPIBError();
		break;
	case ERRID_AH2550RESPONSE:
		strErrorMessage = "Wrong format of the return string of the AH2550A.";
		break;
	case ERRID_PPMSRESPONSE:
		strErrorMessage = "Unknown PPMS response.";
		break;
	case ERRID_OUTPUTFILE:
		strErrorMessage = "Unable to write to output file.";
		break;
	default:
		strErrorMessage = "Unknown ErrorID.";
	}

	CTime theTime = CTime::GetCurrentTime();

	OutputLogFile << "Time: " << CStringA(theTime.Format( "%d/%b/%Y %X" )) << ", ";
	OutputLogFile << "ErrID: " << intErrorID << ", ";
	OutputLogFile << "ErrorMsg: " << strErrorMessage << endl;
	OutputLogFile.close();
}


string CPPMSDialog::ReturnGPIBError()
/* Generates one error string from all the GPIB error functions
 * Return value: Said combined error string
 */
{	
	stringstream ss;
	ss << " Ibsta = " << Ibsta() << " Iberr = " << Iberr() << " " << ErrorMnemonic[Iberr()] ;
	return ss.str();
}


void CPPMSDialog::OnBnClickedHelpBtn()
/* Reads ReadMe1.txt and displays its content in a message box */
{
	// TODO: Add your control notification handler code here

	CString HelpText2 = _T(" ");

	ifstream helpfile("ReadMe1.txt");
	if ( helpfile.is_open() )
	{
		string line;
		while (getline(helpfile, line) )
		{
			HelpText2 += line.c_str();
			HelpText2 += "\r\n";
		}
		helpfile.close();
	}
	else
		HelpText2 += "Unable to open 'ReadMe1.txt'";

	CHelpDialog dlgHelp(HelpText2);
	dlgHelp.DoModal();
}


void CPPMSDialog::OnBnClickedBtnCalculations()
{
	// Start CCalcGap
	CCalcGap Dlg2(m_SaveFile);
	Dlg2.DoModal();
}

