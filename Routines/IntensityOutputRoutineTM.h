/*
 * IntensityOutputRoutine.h
 *
 *  Created on: 11 янв. 2016 г.
 *      Author: aleksandr
 */

#ifndef INTENSITYOUTPUTROUTINE_H_
#define INTENSITYOUTPUTROUTINE_H_

#include <vector>
#include <string>
#include <iostream>
#include "../Structures/HostDeviceVector.h"
#include "../Grids/GridTE.h"
#include "Functors/UpdaterIntensityTM.h"
#include "Routine.h"

/*
 * Класс реализующий вывод интенсивности в файл.
 * Внимание!!! Совершает копирование данных о поле
 * Ez с GPU на CPU
 */

class IntensityOutputRoutineTM: public Routine {
public:
	IntensityOutputRoutineTM(std::string _fileName, GridTE* _grid,
							 int _firstX, int _lastX, int _resolutionX,
							 int _firstY, int _lastY, int _resolutionY,
							 int _startTime, int _endTime, int _period, int _copyEachIteration) :
								 fileName(_fileName), grid(_grid),
								 firstX(_firstX), lastX(_lastX), resolutionX(_resolutionX),
								 firstY(_firstY), lastY(_lastY), resolutionY(_resolutionY),
								 startTime(_startTime), endTime(_endTime), period(_period),
								 currentTime(0), copyEachIteration(_copyEachIteration != 0), stepsCollected(0){
		sizeX=(lastX-firstX)/resolutionX;
		sizeY=(lastY-firstY)/resolutionY;
		std::cout << "Intensity output to files "<< fileName <<"_*.txt : \n";
		std::cout << "\t In region: X(" << firstX << ", " << lastX << ") each " << resolutionX << "px ";
		std::cout << " Y(" << firstY << ", " << lastY << ") each " << resolutionY << "px \n";
		std::cout << "\t Time: start=" << startTime << ", end=" << endTime << " each " << period << "steps \n";
		if (copyEachIteration) {
			intensity.resize(sizeX*sizeY, 0);
			std::cout << "Warning: program will copy Ez vector on each iteration from GPU to CPU, to prevent it specify copyEachIteration = flase. It speed up computation but increase memory consumption." << std::endl;
		} else {
			intensityGPU.resize(sizeX*sizeY, 0);
			updaterIntensity.setParams(intensityGPU.getDevicePtr(),
										grid->Ex.getDevicePtr(),
                                        grid->Ey.getDevicePtr(),
										firstX,
										firstY,
										sizeY,
										grid->sizeY,
										resolutionX,
										resolutionY);
			std::cout << "Warning: To decrease memory consumption specify copyEachIteration = true" << std::endl;
		}
	};
	~IntensityOutputRoutineTM() {};

	void compute(int time);
private:
	void print();
	void printFromGPU();
	void printFromCPU();
	void collectData();
	void collectDataCPU();
	void collectDataGPU();
	void flushData();

	UpdaterIntensityTM updaterIntensity;

	std::vector<float> intensity;
	HostDeviceVector intensityGPU;
	#define intensity(M, N) intensity[(M)*sizeY + (N)]
	#define intensityGPU(M, N) intensityGPU[(M)*sizeY + (N)]

	int currentTime, sizeX, sizeY;

	int firstX, firstY, lastX, lastY, resolutionX, resolutionY, startTime, endTime, period, stepsCollected;
	bool copyEachIteration;
	std::string fileName;
	GridTE* grid;
};

#endif /* INTENSITYOUTPUTROUTINE_H_ */
