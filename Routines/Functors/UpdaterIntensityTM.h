/*
 * UpdaterEz1D.h
 *
 *  Created on: 25 янв. 2016 г.
 *      Author: aleksandr
 */

#ifndef UpdaterIntensityTM_H_
#define UpdaterIntensityTM_H_

#include <thrust/device_vector.h>
#include <thrust/functional.h>

typedef thrust::device_ptr<float> d_ptr;

class UpdaterIntensityTM {
public:
	__host__ __device__
	UpdaterIntensityTM(d_ptr _intensity, d_ptr _Ex,d_ptr _Ey, int _firstX, int _firstY, int _sizeY,
									int _gridSizeY, int _stepX, int _stepY) :
														intensity(_intensity), Ex(_Ex), Ey(_Ey),
														firstX(_firstX), firstY(_firstY),
														sizeY(_sizeY), gridSizeY(_gridSizeY),
														stepX(_stepX), stepY(_stepY)
														{};
	__host__ __device__
	UpdaterIntensityTM() {};

	__host__ __device__
	void setParams(d_ptr _intensity, d_ptr _Ex,d_ptr _Ey, int _firstX, int _firstY, int _sizeY,
									int _gridSizeY, int _stepX, int _stepY) {
		Ex = _Ex;
        Ey = _Ey;
		intensity = _intensity;
		firstX = _firstX;
		firstY = _firstY;
		sizeY = _sizeY;
		gridSizeY = _gridSizeY;
		stepX = _stepX;
		stepY = _stepY;
	}
	__device__
	void operator() (const int indx);

	d_ptr Ex, Ey, intensity;
	int firstX, firstY, sizeY, gridSizeY, stepX, stepY;
};

#endif /* UPDATEREZ1D_H_ */
