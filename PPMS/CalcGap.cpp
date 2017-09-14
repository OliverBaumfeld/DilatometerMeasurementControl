// CalcGap.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "CalcGap.h"
#include "PPMSDialog.h"

#include <string>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

bool interpolate(vector<double> & vList, double dSeek, double & dResult);

/*constants for calculating the gap */
const static double PI =  3.14159265359;
const static double EPSILON0 = 8.854187817; // in pF/m

const static CString PATH_AGLIT = _T("AgOB400.dat");
const static CString PATH_CONFIG = _T("DilConf.txt");

// CCalcGap dialog

IMPLEMENT_DYNAMIC(CCalcGap, CDialog)

CCalcGap::CCalcGap(CString FilePath, CWnd* pParent /*=NULL*/)
	: CDialog(CCalcGap::IDD, pParent)
	, m_MeasFile(FilePath)
	, m_AgLitFile(PATH_AGLIT)
	, m_TCOL(_T("2"))
	, m_CCOL(_T("3"))
	, m_Output(_T(""))
	, m_OutputFile(_T(""))
	, m_AgZero(_T(""))
{

}

CCalcGap::~CCalcGap()
{
}

void CCalcGap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_MEASFILE, m_MeasFile);
	DDX_Text(pDX, IDC_EB_AGLIT, m_AgLitFile);
	DDX_Text(pDX, IDC_EB_C0, m_C0);
	DDX_Text(pDX, IDC_EB_B, m_b);
	DDX_Text(pDX, IDC_EB_RO, m_ro);
	DDX_Text(pDX, IDC_EB_RI, m_ri);
	DDX_Text(pDX, IDC_EB_TCOL, m_TCOL);
	DDX_Text(pDX, IDC_EB_CCOL, m_CCOL);
	DDX_Text(pDX, IDC_OUTPUT, m_Output);
	DDX_Text(pDX, IDC_EB_OUTPUTFILE, m_OutputFile);
	DDX_Text(pDX, IDC_EB_AGZERO, m_AgZero);
}


BEGIN_MESSAGE_MAP(CCalcGap, CDialog)
	ON_BN_CLICKED(IDC_BTN_MEASFILE, &CCalcGap::OnBnClickedBtnMeasfile)
	ON_BN_CLICKED(IDC_BTN_AGLIT, &CCalcGap::OnBnClickedBtnAglit)
	ON_BN_CLICKED(IDC_BTN_CALCULATEGAP, &CCalcGap::OnBnClickedBtnCalculategap)
	ON_BN_CLICKED(IDC_BTN_OUTPUTFILE, &CCalcGap::OnBnClickedBtnOutputfile)
	ON_BN_CLICKED(IDC_BTN_HELP2, &CCalcGap::OnBnClickedBtnHelp2)
	ON_BN_CLICKED(IDC_BTN_AGZERO, &CCalcGap::OnBnClickedBtnAgzero)
END_MESSAGE_MAP()


// CCalcGap message handlers

BOOL CCalcGap::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	/* Read dilatometer cell parameters from config file */
	string line, strLabel, strValue;
	ifstream ConfigFile(PATH_CONFIG);
	if ( ConfigFile.is_open() )
	{
		while(getline(ConfigFile, line))
		{
			if(line[0] != '#' && line[0] != '{')
			{
				line.erase( remove( line.begin(), line.end(), ' ' ), line.end() ); //erase white spaces
				unsigned pos =  line.find("=");
				strLabel= line.substr( 0, pos);
				if(strLabel == "C0") 
					m_C0 = line.substr(pos+1, line.find('#')-pos-1).c_str();
				else if(strLabel == "b") 
					m_b = line.substr(pos+1, line.find('#')-pos-1).c_str();
				else if(strLabel == "ro") 
					m_ro = line.substr(pos+1, line.find('#')-pos-1).c_str();

				else if(strLabel == "ri") 
					m_ri = line.substr(pos+1, line.find('#')-pos-1).c_str();
			}
		}
		ConfigFile.close();
	}
	else
		AfxMessageBox( _T("Unable to read cell parameters from config file."), MB_OK|MB_ICONEXCLAMATION );

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCalcGap::OnBnClickedBtnMeasfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog MeasDlg(FALSE, _T(".txt"), FALSE , OFN_NOCHANGEDIR, _T("Text Files (*.txt)|*.txt|") _T("All Files|*.*||"));
	if ( MeasDlg.DoModal() == IDOK )
		m_MeasFile = MeasDlg.GetPathName();

	UpdateData(FALSE);
}

void CCalcGap::OnBnClickedBtnOutputfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog MeasDlg(FALSE, _T(".txt"), FALSE , OFN_NOCHANGEDIR, _T("Text Files (*.txt)|*.txt|") _T("Data Files (*.dat)|*.dat|") _T("All Files|*.*||"));
	if ( MeasDlg.DoModal() == IDOK )
		m_OutputFile = MeasDlg.GetPathName();

	// open file
	ifstream OutputFile(m_OutputFile);
	if ( OutputFile.is_open() )
	{
		/* The file already exists. */ 
		CString strMessage = _T("The file ") + m_OutputFile + _T(" already exists.");
		if ( AfxMessageBox(strMessage, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
			m_OutputFile = "";
	}

	UpdateData(FALSE);
}


void CCalcGap::OnBnClickedBtnAglit()
{
	// TODO: Add your control notification handler code here
	CFileDialog MeasDlg(FALSE, _T(".dat"), _T("AgOB400.dat"), OFN_NOCHANGEDIR, _T("Data Files (*.dat)|*.dat|") _T("Text Files (*.txt)|*.txt|")  _T("All Files|*.*||"));
	if ( MeasDlg.DoModal() == IDOK )
		m_AgLitFile = MeasDlg.GetPathName();

	UpdateData(FALSE);
}


void CCalcGap::OnBnClickedBtnAgzero()
{
	// TODO: Add your control notification handler code here
	CFileDialog MeasDlg(FALSE, _T(".dat"), _T(""), OFN_NOCHANGEDIR, _T("Data Files (*.dat)|*.dat|") _T("Text Files (*.txt)|*.txt|") _T("All Files|*.*||"));
	if ( MeasDlg.DoModal() == IDOK )
		m_AgZero = MeasDlg.GetPathName();

	UpdateData(FALSE);
}


void CCalcGap::OnBnClickedBtnHelp2()
{
	// TODO: Add your control notification handler code here
	CString HelpText2 = _T(" ");

	ifstream helpfile("ReadMe2.txt");
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
	{
		HelpText2 += "Unable to open 'ReadMe2.txt'";
	}
	CHelpDialog dlgHelp(HelpText2);
	dlgHelp.DoModal();
}


template <typename T> int sgn(T val)
/* calulates signum */
{
    return (T(0) < val) - (val < T(0));
}


void CCalcGap::OnBnClickedBtnCalculategap()
/* Main Function 
 * Calculates the gap from a measurement file (with capacitance and temperature columns)*/
{
	double C, T, dllAg, kT, AoT, AiT, C2, dstep, d, gamma2_o, gamma2_i, gamma_o, gamma_i;
	int oldsign;
	int ii, nsize;
	string line;
	stringstream ssMessage;

	/* load dialog entries */
	UpdateData(TRUE); //important to update dialog variables (m_AgLitFile, m_TCOL, ...)
	int nTcolumn = _tstoi(m_TCOL);
	int nCcolumn = _tstoi(m_CCOL);
	int nGAPcolumn = nCcolumn -1;
	if (nGAPcolumn < 1 )
		nGAPcolumn = 1; 

	//double dll, dllAgZero;
	//double samplelength = atof(m_L);

	/* load cell parameteres */
	double C0 = _tstof(m_C0);
	double b  = 0.001 * _tstof(m_b);  // [mm] -> [m]
	double ro = 0.001 * _tstof(m_ro); // [mm] -> [m]
	double ri = 0.001 * _tstof(m_ri); // [mm] -> [m]
	double k0 = EPSILON0/C0 * (ro*ro*PI - ri*ri*PI); // [pF/m]/[pF] * [m^2] = [m]

	/* load silver literature file into vector*/
	vector<double> vAgLit;
	ifstream SilverLitFile(m_AgLitFile);
	if ( SilverLitFile.is_open() )
	{
		//Problem of the old solution: skips the first line with numbers as well!
		// ugly(?) solution: reset to beginning of file and read again
		// get rid of header lines starting with # or {
		int jj=0;
		getline(SilverLitFile, line);
		// count number of header lines
		while (line[0] == '#' || line[0] == '{')
		{	
			jj++;
			getline(SilverLitFile, line);
		}
		SilverLitFile.seekg(0, SilverLitFile.beg);
		for (ii=0; ii<jj; ii++)
			getline(SilverLitFile, line);
		
		copy(istream_iterator<double>(SilverLitFile), istream_iterator<double>(), back_inserter(vAgLit));
		SilverLitFile.close();

		/* normalise data to dll(T=300K)=0 */
		double dllAg300 = 0;
		if ( interpolate(vAgLit, 300, dllAg300) )
		{
			for (std::vector<double>::size_type nn = 1; nn < vAgLit.size(); nn+=2)
				vAgLit[nn]= vAgLit[nn] - dllAg300;
		}
		else
		{
			// silver.lit file is not going up to 300.
			AfxMessageBox( _T("Silver literature file out of range (value for T=300K needed) or inapropiate header lines (see help)."), MB_OK|MB_ICONEXCLAMATION );
			return;
		}
	}
	else
	{
		AfxMessageBox( _T("Unable to open silver literature file."), MB_OK|MB_ICONEXCLAMATION );
		return;
	}

	///* load silver zero file into vector*/
	//vector<double> vAgZero;
	//ifstream SilverZeroFile(m_AgZero);
	//if ( SilverZeroFile.is_open() )
	//{
	//	do {	// get rid of header lines starting with # or {
	//		std::getline(SilverZeroFile, line);
	//	}while (line[0] == '#' || line[0] == '{');
	//	// Problem! This skips the first line with numbers as well!
	//	copy(istream_iterator<double>(SilverZeroFile), istream_iterator<double>(), back_inserter(vAgZero));
	//	SilverZeroFile.close();

	//	/* normalise data to dll(T=300K)=0 */
	//	double dllAgZero300 = 0;
	//	if ( interpolate(vAgZero, 300, dllAgZero300) )
	//	{
	//		// Dangerous! If vAgZero.size() is odd this will produce an error
	//		// Solution: < instead of !=, nn= 1 instead of 0, nn instead of nn+1
	//		for (std::vector<double>::size_type nn = 1; nn < vAgZero.size(); nn+=2) 
	//			vAgZero[nn]= vAgZero[nn] - dllAgZero300;
	//	}
	//	else
	//	{
	//		// silver.lit file is not going up to 300.
	//		AfxMessageBox( "Silver zero file out of range. Value for T = 300K needed.", MB_OK|MB_ICONEXCLAMATION );
	//		return;
	//	}
	//}
	//else
	//{
	//	AfxMessageBox( "Unable to open silver zero file.", MB_OK|MB_ICONEXCLAMATION );
	//	return;
	//}
	
	/* handle the measurement file */
    //copyto << copyfrom.rdbuf();
	if (m_MeasFile == m_OutputFile)
	{
		AfxMessageBox( _T("Measurement and output file are the same."), MB_OK|MB_ICONEXCLAMATION );
		return;
	}
	ifstream ReadFile(m_MeasFile);
	if ( !ReadFile.is_open() )
	{
		AfxMessageBox( _T("Unable to open measurement file."), MB_OK|MB_ICONEXCLAMATION );
		return;
	}
	ofstream WriteFile(m_OutputFile);
	if ( !WriteFile.is_open() )
	{
		AfxMessageBox( _T("Unable to open output file."), MB_OK|MB_ICONEXCLAMATION );
		return;
	} 
	/* handle headers */
	int nHeaderLines = 1;
	for (ii=0; ii<nHeaderLines; ii++)
	{
		getline(ReadFile, line);
		stringstream ssline( line );
		string field;
		for (ii=0; ii<nGAPcolumn; ii++)
		{
			getline(ssline, field, ',');
			WriteFile << field << ",";
		}
		WriteFile << " gap(mm),";
		WriteFile << ssline.rdbuf() << endl;
	}
	WriteFile << "#The gap has been calculated with the following cell parameters: C0="<<C0<<" b="<<1000*b<<" ro="<<1000*ro<<" ri="<<1000*ri<<endl;
	nHeaderLines++;
	//WriteFile << "k0=" << setprecision(19) << k0<< endl;

	/* main loop */
	vector <double> vLine;
	int nLineCounter = nHeaderLines;
	bool bSkipLine;
	int SkippedPoints = 0;
	while(getline(ReadFile, line))
	{
		nLineCounter++;
		bSkipLine = false;
		/* convert line into array of doubles */
		vLine.clear();
		stringstream ssline(line);
		string field;
		while(getline( ssline, field, ',' ))
		{
			stringstream fs( field );
			double f = 0.0;  // (default value is 0.0)
			fs >> f;
			vLine.push_back( f );
		}

		/* get T and C */
		T = vLine[nTcolumn-1];
		C = vLine[nCcolumn-1];

		/* interpolate dllAG */
		if ( interpolate(vAgLit, T, dllAg) &&  C > 0.0 )
		{
			/* calculate set of temperature dependent parameters */
			kT = k0;// * (1- dllAg);
			AoT = ro*ro*PI*(1+dllAg);  // explain why no square
			AiT = ri*ri*PI*(1+dllAg);
	
			C2 = 1000; //delete
			dstep = 0.000001;
			d = (AoT - AiT) * EPSILON0/C;
			if ( d < kT*ro/(b+ro) )
				d = 1.1 *  kT*ro/(b+ro);
			oldsign = 0;
			ii=0;
			while (abs(C - C2) > 1E-9 && dstep/d > 1E-15 && ii < 1000000)
			{
				ii++;
				gamma_o  = ro/b * (kT/d-1.);
				gamma2_o = gamma_o*gamma_o;
				gamma_i  = ri/b * (kT/d-1.);
				gamma2_i = gamma_i*gamma_i;
				C2 = AoT*EPSILON0/d *2./gamma2_o * (1-sqrt(1-gamma2_o));
				C2 = C2 - AiT*EPSILON0/d *2./gamma2_i * (1.-sqrt(1.-gamma2_i));
				d = d + sgn(C2-C)* dstep;

				if (oldsign != sgn(C2-C))
				{
					oldsign = sgn(C2-C);
					dstep = dstep/10;
				}
			}

			if (ii == 1000000 || abs(C-C2) > 0.0001 )
			{
				ssMessage << "Iteration failed at line: " << nLineCounter << "\r\n";
				bSkipLine = true;
				SkippedPoints++;
			}
		}
		else
		{
			if ( C <= 0.0 )
				ssMessage << "Negative capacitance in line: " << nLineCounter << "\r\n";
			else
				ssMessage << "Temperature out of range. line: " << nLineCounter << "\r\n";
			bSkipLine = true;
			SkippedPoints++;
		}

		/* calculate (Delta L)/L */
		//if ( !bSkipLine )
		//{
		//	/* interpolate AgZero */
		//	if ( interpolate(vAgZero, T, dllAgZero)) 
		//		dll = d/samplelength - dllAgZero + dllAg; // I am not sure if dllAg is defined properly
		//}

		/* write values to output file */
		ii=0;
		nsize = vLine.size();
		if ( nGAPcolumn > nsize )
			nGAPcolumn = nsize+1;
		for ( ii = 0; ii<nGAPcolumn; ii++)
			WriteFile << setw(11) << setprecision(9) << vLine[ii] << ", ";

		if ( bSkipLine  == true )
			WriteFile << setw(14) << " ";
		else
			WriteFile << setw(14) << setprecision(12) << 1000*d;

		for (; ii<nsize; ii++)
			WriteFile << ", " << setw(11) << setprecision(9) << vLine[ii];
		WriteFile << endl;
	}
		
	ReadFile.close();
	WriteFile.close();

	ssMessage << "Calculation finished." << "\r\n";
	ssMessage << "Skipped Datapoints: " << SkippedPoints << "\r\n";
	//string strMessage;
	//ssMessage >> strMessage;
	m_Output += ssMessage.str().c_str();
	UpdateData(false);
}

