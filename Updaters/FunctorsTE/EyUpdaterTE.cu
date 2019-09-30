/*
 * EyUpdaterTE.cpp
 *
 *  Created on: 29 янв. 2016 г.
 *      Author: aleksandr
 */

#include "EyUpdaterTE.h"
#include "SmartIndex.h"
//   x o o o x
//   x o o o x
//   x o o o x
//   x o o o x
//   x x x x x

__host__ __device__
void EyUpdaterTE::operator() (const int indx) {
	// m и n - индексы в полноценных массивах
	// sizeY - размер полноценнго массива
	int m = indx/(sizeY);
	int n = indx%(sizeY);
	float loss = sigma(m, n)/(2*epsilon(m, n));
	float Ceye = (1-loss)/(1+loss);
	float Ceyh =  S * 377.0 / epsilon(m,n) / (1+loss);
	
	if ( (m>0 && m<sizeX-1) && (n<sizeY-1) ) {
		Ey(m, n) = Ceye*Ey(m, n) - Ceyh*(Hz(m, n) - Hz(m-1, n));
	}
}
