/*
 * UpdaterEz1D.cpp
 *
 *  Created on: 25 янв. 2016 г.
 *      Author: aleksandr
 */

#include "UpdaterIntensityTM.h"

__device__
void UpdaterIntensityTM::operator() (const int indx) {
	#define Ex(M, N) Ex[(M) * (sizeY) + (N)]
    #define Ey(M, N) Ey[(M) * (sizeY-1) + (N)]
	const int n = indx % sizeY;
	const int m = indx / sizeY;
	intensity[indx] = intensity[indx] + sqrt(Ex(firstX + m*stepX, firstY + n*stepX)*Ex(firstX + m*stepX, firstY + n*stepX)+Ey(firstX + m*stepX, firstY + n*stepX)*Ey(firstX + m*stepX, firstY + n*stepX));


}
