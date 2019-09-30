/*
 * HzOutputRoutineTE.h
 *
 *  Created on: 29 янв. 2016 г.
 *      Author: aleksandr
 */

#ifndef HZOUTPUTROUTINETE_H_
#define HZOUTPUTROUTINETE_H_

#include <vector>
#include <string>
#include <iostream>
#include "../Grids/GridTE.h"
#include "Routine.h"

/*
 * Класс выводящий данные о поле Hz в файл.
 * Внимание!! Совершает копирование с GPU на CPU
 */

class HzOutputRoutineTE: public Routine {
public:
	HzOutputRoutineTE(std::string _fileName, GridTE* _grid,
					  int _firstX, int _lastX, int _resolutionX,
				   	  int _firstY, int _lastY, int _resolutionY,
					  int _startTime, int _endTime, int _period) :
								 fileName(_fileName), grid(_grid),
								 firstX(_firstX), lastX(_lastX), resolutionX(_resolutionX),
								 firstY(_firstY), lastY(_lastY), resolutionY(_resolutionY),
								 startTime(_startTime), endTime(_endTime), period(_period),
								 currentTime(0){
		sizeX=(lastX-firstX)/resolutionX;
		sizeY=(lastY-firstY)/resolutionY;
		std::cout << "Hz output to files "<< fileName <<"_*.txt : \n";
		std::cout << "\t In region: X(" << firstX << ", " << lastX << ") each " << resolutionX << "px ";
		std::cout << " Y(" << firstY << ", " << lastY << ") each " << resolutionY << "px \n";
		std::cout << "\t Time: start=" << startTime << ", end=" << endTime << " each " << period << "steps \n";
	};
	~HzOutputRoutineTE() {};

	void compute(int time);
private:
	void print();

	int currentTime, sizeX, sizeY;

	int firstX, firstY, lastX, lastY, resolutionX, resolutionY, startTime, endTime, period;
	std::string fileName;
	GridTE* grid;
};
#endif /* HZOUTPUTROUTINETE_H_ */
