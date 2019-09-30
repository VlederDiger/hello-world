/*
 * ExUpdaterTE.cpp
 *
 *  Created on: 29 янв. 2016 г.
 *      Author: aleksandr
 */

#include "ExUpdaterTE.h"
#include "SmartIndex.h"
//   x x x x x
//   o o o o x
//   o o o o x
//   o o o o x
//   x x x x x

__host__ __device__
void ExUpdaterTE::operator() (const int indx) {
	// m и n - индексы в полноценных массивах
	// sizeY - размер полноценнго массива
	int m = indx/(sizeY);
	int n = indx%(sizeY);
	float loss = sigma(m, n)/(2*epsilon(m, n));
	float Cexe = (1-loss)/(1+loss);
	float Cexh =  S * 377.0 / epsilon(m,n) / (1+loss);
	if ( m < (sizeX - 1) && n > 0 && n < (sizeX - 1) ) {
		Ex(m, n) = Cexe*Ex(m, n) + Cexh*(Hz(m, n) - Hz(m, n - 1));
	}
}
