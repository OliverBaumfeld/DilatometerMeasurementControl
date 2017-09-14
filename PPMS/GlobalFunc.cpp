/* ====================================
 *    Global functions 
 *=====================================
 */
#include "StdAfx.h"
#include <vector>

bool interpolate(std::vector<double> & vList, double dSeek, double & dResult)
/* The vector has to include x and y data stored as [ x1 y1 x2 y2 ... ]
   Note that vector is a one dimensional array.
   This function searches for x(n) <= dSeek < x(n+1) and calculates an
   interpolated y value corresponding to dSeek. Y(dSeek)
   The y value is stored in the variable pointed at by dResult.
   
   Returns false if datapoint is out of range.
*/
{
	unsigned ii=0;
	unsigned nEnd = vList.size()/2*2;

	while ( ii<nEnd && vList[ii]<dSeek )
		ii+=2;

	if (ii == 0)
		return false;
	else if (ii >= nEnd)
		return false;
	else
		dResult = vList[ii-1] +  (vList[ii-1]-vList[ii+1])/(vList[ii-2]-vList[ii]) * (dSeek - vList[ii-2]);

	return true;
}