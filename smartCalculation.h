#ifndef SMARTCALCUTATION_H
#define SMARTCALCUTATION_H

#include "commonFunctions.h"
#include "MagneticFieldDependence.h"
#include <deque>
#include <string>

/*
Класс для работы адаптации фильтра
*/

enum parameterType
{
	ELECTRON_MOBILITY=0,
	ELECTRON_DENSITY=1,
	LIGHT_HOLE_MOBILITY=2,
	LIGHT_HOLE_DENSITY=3,
	HEAVY_HOLE_MOBILITY=4,
	HEAVY_HOLE_DENSITY=5
};

struct Results
{
	FilterParams fpHall;
	FilterParams fpRes;
	int polinomPowHall;
	int polinomPowMagnetoresistance;
	TSignal holeConcentration;
    TSignal holeMobility;
    TSignal electronConcentration;
    TSignal electronMobility;
	long double targetFuncValue;
	std::vector <PeaksCriteria> vPC;
    AdditionalData additionalData;
};


class smartCalculation
{
public:
	smartCalculation(MagneticFieldDependence * base);
	~smartCalculation();
	bool processData();
	bool saveResults(std::string filename);


private:
	size_t numberOfParameters;
	size_t numberOfStoredBestResults;
	long double targetFunction (Results & r, parameterType pt);
	bool isBetterResults(Results & r1, Results & r2); // возвращает true если r1 лучше чем r2
	bool processResults();

	std::deque < std::deque<Results> > results;
	MagneticFieldDependence * MFDData;



};

#endif
