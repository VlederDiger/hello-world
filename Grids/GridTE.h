/*
 * GridTE.h
 *
 *  Created on: 28 янв. 2016 г.
 *      Author: aleksandr
 */

#ifndef GRIDTE_H_
#define GRIDTE_H_

#include "../Structures/HostDeviceVector.h"

#include <string>

typedef HostDeviceVector vector;

class GridTE {
public:
	GridTE(std::size_t _sizeX, std::size_t _sizeY, float _S);
	~GridTE() {};

	void readEpsilon(const std::string& fileName);

	void readSigma(const std::string& fileName);
	
	// Геометрические размеры
	std::size_t sizeX, sizeY;
	float S;
	vector Hz, Ex, Ey, epsilon, sigma;
	std::size_t time;
};

#endif /* GRIDTE_H_ */
