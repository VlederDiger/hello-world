/*
 * Parser.cpp
 *
 *  Created on: 11 янв. 2016 г.
 *      Author: aleksandr
 */

#include "Parser.h"

#include <cmath>
#include <iostream>

Parser::Parser(): gridTM(NULL), gridTE(NULL), tfsfTM(NULL), tfsfTE(NULL), num(0) {

}

Parser::~Parser() {
	if (gridTM != NULL) {
		delete gridTM;
	}
	if (gridTE != NULL) {
		delete gridTE;
	}
	for (int i = 0; i < hSources.size(); i++){
		delete hSources[i];
	}
	for (int i = 0; i < rSources.size(); i++){
		delete rSources[i];
	}
	for (int i = 0; i < EzOut.size(); i++){
		delete EzOut[i];
	}
	for (int i = 0; i < HzOut.size(); i++){
		delete HzOut[i];
	}
	for (int i = 0; i < IntenOut.size(); i++){
		delete IntenOut[i];
	}
	for (int i = 0; i < ExOut.size(); i++){
		delete ExOut[i];
	}
	for (int i = 0; i < EyOut.size(); i++){
		delete EyOut[i];
	}
	for (int i = 0; i < HxOut.size(); i++){
		delete HxOut[i];
	}
	for (int i = 0; i < HyOut.size(); i++){
		delete HyOut[i];
	}
	for (int i = 0; i < ABCTMs.size(); i++){
		delete ABCTMs[i];
	}
	for (int i = 0; i < periodicsTM.size(); i++){
		delete periodicsTM[i];
	}
	for (int i = 0; i < periodicsTE.size(); i++){
		delete periodicsTE[i];
	}
	for (int i = 0; i < ABCTEs.size(); i++){
		delete ABCTEs[i];
	}

	if (tfsfTM != NULL) {
		delete tfsfTM;
	}
	if (tfsfTE != NULL) {
		delete tfsfTE;
	}
}

void Parser::runFile(std::string fileName) {
	fileStream.open(fileName);
	if (!fileStream.is_open()) {
		return;
	}
	std::string simulationType;
	fileStream >> simulationType; // Skip prefix
	fileStream >> simulationType; // Read type
	if (simulationType == "TE") {
		runTE();
	}
	if (simulationType == "TM") {
		runTM();
	}
	fileStream.close();
}

void Parser::runTM() {
	
}

void Parser::runTE() {
	std::string command = "";
	while (fileStream >> command) {
		if (command == "GridTE:") {
			createGridTE();
		}
		if (command == "Epsilon:") {
			readEpsilonTE();
		}
		
		if (command == "Sigma:") {
			readSigmaTE();
		}
		
		if (command == "Source:") {
			addSourceTE();
		}
		if (command == "BoundaryCond:") {
			addBoundaryCondTE();
		}
		if (command == "Output:") {
			addRoutineTE();
		}
		if (command == "Run:") {
			fileStream >> num;
		}
	}
	for (int i = 0; i < hSources.size(); i++){
		updaterTE.addSource(hSources[i]);
	}
	for (int i = 0; i < HzOut.size(); i++){
		updaterTE.addRoutine(HzOut[i]);
	}
	for (int i = 0; i < ExOut.size(); i++){
		updaterTE.addRoutine(ExOut[i]);
	}
	for (int i = 0; i < EyOut.size(); i++){
		updaterTE.addRoutine(EyOut[i]);
	}
	
	for (int i = 0; i < IntenOut.size(); i++){
		updaterTE.addRoutine(IntenOut[i]);
	}
	for (int i = 0; i < ABCTEs.size(); i++){
		updaterTE.addBoundaryCond(ABCTEs[i]);
	}
	for (int i = 0; i < periodicsTE.size(); i++){
		updaterTE.addBoundaryCond(periodicsTE[i]);
	}
	if (tfsfTE != NULL) {
		updaterTE.addTFSF(tfsfTE);
	}
	updaterTE.run(num);
}

void Parser::readEpsilonTM() {
	std::string epsilonFileName = "";
	fileStream >> epsilonFileName;
	gridTM->readEpsilon(epsilonFileName);
}

void Parser::readSigmaTM() {
	std::string sigmaFileName = "";
	fileStream >> sigmaFileName;
	gridTM->readSigma(sigmaFileName);
}

void Parser::readExcludedTM() {
	std::string excludedFileName = "";
	fileStream >> excludedFileName;
	gridTM->readExcluded(excludedFileName);
}

void Parser::readEpsilonTE() {
	std::string epsilonFileName = "";
	fileStream >> epsilonFileName;
	gridTE->readEpsilon(epsilonFileName);
}

void Parser::readSigmaTE() {
	std::string sigmaFileName = "";
	fileStream >> sigmaFileName;
	gridTE->readSigma(sigmaFileName);
}

void Parser::createGridTM() {
	int sizeX(0), sizeY(0);
	fileStream >> sizeX;
	fileStream >> sizeY;
	gridTM = new GridTM(sizeX, sizeY, 1/sqrt(2));
	updaterTM.addGrid(gridTM);
}

void Parser::createGridTE() {
	int sizeX(0), sizeY(0);
	fileStream >> sizeX;
	fileStream >> sizeY;
	gridTE = new GridTE(sizeX, sizeY, 1/sqrt(2));
	updaterTE.addGrid(gridTE);
}

inline void Parser::addSourceTM() {
	std::string sourceType;
	fileStream >> sourceType;
	if (sourceType == "Harmonic") {
		float amp = 0;
		fileStream >> amp;
		float freq = 0;
		fileStream >> freq;
		freq = 4.44063/freq;
		int posX(0), posY(0);
		fileStream >> posX >> posY;
		HarmonicSource* hSource = new HarmonicSource(amp, freq, posX, posY);
		hSources.push_back(hSource);
	}
	if (sourceType == "Ricker") {
		float amp = 0;
		fileStream >> amp;
		float freq = 0;
		fileStream >> freq;
		freq = 4.44063/freq;
		int M = 0;
		fileStream >> M;
		int posX(0), posY(0);
		fileStream >> posX >> posY;
		RickerSource* rSource = new RickerSource(amp, freq, M, posX, posY);
		rSources.push_back(rSource);
	}
}

inline void Parser::addSourceTE() {
	std::string sourceType;
	fileStream >> sourceType;
	if (sourceType == "Harmonic") {
		float amp = 0;
		fileStream >> amp;
		float freq = 0;
		fileStream >> freq;
		freq = 4.44063/freq;
		int posX(0), posY(0);
		fileStream >> posX >> posY;
		HarmonicSource* hSource = new HarmonicSource(amp, freq, posX, posY);
		hSources.push_back(hSource);
	}
	if (sourceType == "Ricker") {
		float amp = 0;
		fileStream >> amp;
		float freq = 0;
		fileStream >> freq;
		freq = 4.44063/freq;
		int M = 0;
		fileStream >> M;
		int posX(0), posY(0);
		fileStream >> posX >> posY;
		RickerSource* rSource = new RickerSource(amp, freq, M, posX, posY);
		rSources.push_back(rSource);
	}
}

inline void Parser::addBoundaryCondTM() {
	std::string condType;
	fileStream >> condType;
	if (condType == "ABC") {
		ABCTM* abc = new ABCTM(gridTM);
		ABCTMs.push_back(abc);
	}
	if (condType == "Periodic") {
		PeriodicCondTM* periodic = new PeriodicCondTM(gridTM);
		periodicsTM.push_back(periodic);
	}
	if (condType == "TFSF") {
		int firstX, lastX, firstY, lastY;
		fileStream >> firstX >> lastX >> firstY >> lastY;
		std::cout << firstX << " " << lastX << " " << firstY << " " << lastY << " ";
		tfsfTM = new TFSFCondTM(gridTM, firstX, lastX, firstY, lastY);
		std::string sourceType;
		fileStream >> sourceType;
		if (sourceType == "Harmonic") {
				float amp = 0;
				fileStream >> amp;
				float freq = 0;
				fileStream >> freq;
				freq = 4.44063/freq;
				hTFSFSource = new HarmonicSource(amp, freq, 0, 0);
				tfsfTM->addSource(hTFSFSource);
		}
		if (sourceType == "Ricker") {
			float amp = 0;
			fileStream >> amp;
			float freq = 0;
			fileStream >> freq;
			freq = 4.44063/freq;
			int M = 0;
			fileStream >> M;
			rTFSFSource = new RickerSource(amp, freq, M, 0, 0);
			tfsfTM->addSource(rTFSFSource);
		}
	}
}

inline void Parser::addBoundaryCondTE() {
	std::string condType;
	fileStream >> condType;
	if (condType == "ABC") {
		ABCTE* abc = new ABCTE(gridTE);
		ABCTEs.push_back(abc);
	}
	if (condType == "Periodic") {
		PeriodicCondTE* periodic = new PeriodicCondTE(gridTE);
		periodicsTE.push_back(periodic);
	}
	if (condType == "TFSF") {
		int firstX, lastX, firstY, lastY;
		fileStream >> firstX >> lastX >> firstY >> lastY;
		tfsfTE = new TFSFCondTE(gridTE, firstX, lastX, firstY, lastY);
		std::string sourceType;
		fileStream >> sourceType;
		if (sourceType == "Harmonic") {
				float amp = 0;
				fileStream >> amp;
				float freq = 0;
				fileStream >> freq;
				freq = 4.44063/freq;
				hTFSFSource = new HarmonicSource(amp, freq, 0, 0);
				tfsfTE->addSource(hTFSFSource);
		}
		if (sourceType == "Ricker") {
				float amp = 0;
				fileStream >> amp;
				float freq = 0;
				fileStream >> freq;
				freq = 4.44063/freq;
				int M = 0;
				fileStream >> M;
				rTFSFSource = new RickerSource(amp, freq, M, 0, 0);
				tfsfTE->addSource(rTFSFSource);
			}
	}
}

inline void Parser::addRoutineTM() {
	
	
}

inline void Parser::addRoutineTE() {
	std::string routineType;
	fileStream >> routineType;
	if (routineType == "Hz") {
		std::string fileName;
		int firstX, lastX, stepX, firstY, lastY, stepY, startTime, endTime, period;
		fileStream >> fileName >> firstX >> lastX >> stepX >> firstY >> lastY >> stepY >> startTime >> endTime >> period;
		HzOutputRoutineTE* routine = new HzOutputRoutineTE(fileName, gridTE, firstX, lastX,
															stepX, firstY, lastY, stepY,
															startTime, endTime,
															period);
		HzOut.push_back(routine);
	}
	if (routineType == "Ex") {
		std::string fileName;
		int firstX, lastX, stepX, firstY, lastY, stepY, startTime, endTime, period;
		fileStream >> fileName >> firstX >> lastX >> stepX >> firstY >> lastY >> stepY >> startTime >> endTime >> period;
		ExOutputRoutineTE* routine = new ExOutputRoutineTE(fileName, gridTE, firstX, lastX,
															stepX, firstY, lastY, stepY,
															startTime, endTime,
															period);
		ExOut.push_back(routine);
	}
	if (routineType == "Ey") {
		std::string fileName;
		int firstX, lastX, stepX, firstY, lastY, stepY, startTime, endTime, period;
		fileStream >> fileName >> firstX >> lastX >> stepX >> firstY >> lastY >> stepY >> startTime >> endTime >> period;
		EyOutputRoutineTE* routine = new EyOutputRoutineTE(fileName, gridTE, firstX, lastX,
															stepX, firstY, lastY, stepY,
															startTime, endTime,
															period);
		EyOut.push_back(routine);
	}
	
	if (routineType == "Intensity") {
		std::string fileName;
		int firstX, lastX, stepX, firstY, lastY, stepY, startTime, endTime, period, copyEachItearation;
		fileStream >> fileName >> firstX >> lastX >> stepX >> firstY >> lastY >> stepY >> startTime >> endTime >> period >> copyEachItearation;
		IntensityOutputRoutineTM* routine = new IntensityOutputRoutineTM(fileName, gridTE, firstX, lastX,
															stepX, firstY, lastY, stepY,
															startTime, endTime,
															period, copyEachItearation);
		IntenOut.push_back(routine);
	}
}

