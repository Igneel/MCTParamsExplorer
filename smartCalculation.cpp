#include "smartCalculation.h"
#include <sstream>

smartCalculation::smartCalculation(MagneticFieldDependence * base)
{
	/*
	Всё делается на основе данных, которые хранятся и рассчитываются в классе MagneticFieldDependence.
	*/
	numberOfParameters=6;
	numberOfStoredBestResults=3;
	MFDData=base;
	results.resize(numberOfParameters);
}

smartCalculation::~smartCalculation()
{

}

bool isBetterResult(Results & r1, Results & r2)
// возвращает true если r1 лучше чем r2
{
	return r1.targetFuncValue<r2.targetFuncValue;
}

bool smartCalculation::isBetterResults(Results & r1, Results & r2)
// возвращает true если r1 лучше чем r2
{
	return r1.targetFuncValue<r2.targetFuncValue;
}

//bool compareResult (int i,int j) { return (i<j); }

bool smartCalculation::processResults()
{
	// Провести обработку

	Results nr; // наши новые результаты.
	nr.fpHall=*(MFDData->getFilterParamsHall());
	nr.fpRes=*(MFDData->getFilterParamsResistance());
	MFDData->getExtrapolateParams(nr.polinomPowHall,nr.polinomPowMagnetoresistance);
	nr.holeConcentration=*(MFDData->getHoleConcentration());
    nr.holeMobility=*(MFDData->getHoleMobility());
    nr.electronConcentration=*(MFDData->getElectronConcentration());
    nr.electronMobility=*(MFDData->getElectronMobility());
    nr.vPC=MFDData->MobilitySpectrumObj->getPeaksCriteria();
    nr.additionalData=MFDData->MobilitySpectrumObj->getAdditionalData();

	// Рассчитать целевые функции для всех видов носителей заряда
	for (int i = 0; i < numberOfParameters; ++i)
	{
		nr.targetFuncValue= targetFunction(nr,i);

		if (results[i].size()==0)
		{
			/* (если пусто - добавить так) */
			results[i].push_back(nr);
			sort(results[i].begin(), results[i].end(), isBetterResult); // Тут нужно определить свою функцию для сортировки!
		}
		else
		{
			// Сравнить с имеющимися данными 
			if (isBetterResults(nr,results[i].back()))
			{
				// Если целевая функция лучше - сохранить
				results[i].push_back(nr);
				sort(results[i].begin(), results[i].end(), isBetterResult); // Тут нужно определить свою функцию для сортировки!
			}

			// Хранить будем numberOfStoredBestResults лучших значений целевой функции для каждого параметра

			if (results[i].size()>numberOfStoredBestResults)
			{
				results[i].pop_back();
			}
		}
	}
	return true;
}

bool smartCalculation::processData()
{
	/*
	Итак, тут обработка данных ведется.
	*/

	// Для каждого значения частоты среза фильтра
	FilterParams fpHall=*(MFDData->getFilterParamsHall());
	FilterParams fpRes=*(MFDData->getFilterParamsResistance());
	int polinomPowHall;
	int polinomPowMagnetoresistance;
	MFDData->getExtrapolateParams(polinomPowHall,polinomPowMagnetoresistance);

	long double step=0.1;
	for (long double BandWidthFHall=0.01, AttenuationFHall=0.1; BandWidthFHall < 10; BandWidthFHall+=step, AttenuationFHall+=step)
	{
		if(step==0.1 && BandWidthFHall>1.0)
		{
			step=1.0;
		}
		long double step=0.1;
		for (long double BandWidthFRes=0.01, AttenuationFRes=0.1; BandWidthFRes < 10; BandWidthFRes+=step, AttenuationFRes+=step)
		{
			if(step==0.1 && BandWidthFRes>1.0)
			{
				step=1.0;
			}

        	fpHall.BandwidthFrequency=BandWidthFHall;
        	fpHall.AttenuationFrequency=AttenuationFHall;
        	fpRes.BandwidthFrequency=BandWidthFRes;
        	fpRes.AttenuationFrequency=AttenuationFRes;

        	MFDData->setFilterParamsResistance(fpRes.SamplingFrequecy, fpRes.BandwidthFrequency, 
        		fpRes.AttenuationFrequency, fpRes.filterLength);
			MFDData->setFilterParamsHall(fpHall.SamplingFrequecy, fpHall.BandwidthFrequency, 
        		fpHall.AttenuationFrequency, fpHall.filterLength);
			MFDData->filterData();
        	MFDData->extrapolateData(FILTERED_DATA,polinomPowMagnetoresistance,polinomPowHall);
    		MFDData->calculateTenzor(FILTERED_DATA);
			MFDData->calculateMobilitySpectrum();
			MFDData->MobilitySpectrumObj->calculatePeaksWeigth();
			if (0/* Нужна подгонка */)
			{
				/* сначала перепиши её в класс MagneticFieldDependence! */
			}
			processResults();
		}
	}
	return true;
}

std::string convertToString(AdditionalData & ad)
{
	std::ostringstream strs;
	strs << "leftPointElectronConductivity " << ad.leftPointElectronConductivity << "\n";
	strs << "leftPointHoleConductivity " << ad.leftPointHoleConductivity << "\n";
	strs << "rightPointElectronConductivity " << ad.rightPointElectronConductivity << "\n";
	strs << "rightPointHoleConductivity " << ad.rightPointHoleConductivity << "\n";
	strs << "leftPointElectronConductivityLog " << ad.leftPointElectronConductivityLog << "\n";
	strs << "leftPointHoleConductivityLog " << ad.leftPointHoleConductivityLog << "\n";
	strs << "rightPointElectronConductivityLog " << ad.rightPointElectronConductivityLog << "\n";
	strs << "rightPointHoleConductivityLog " << ad.rightPointHoleConductivityLog << "\n";

	return strs.str();
}

std::string convertToString(PeaksCriteria & pc)
{
	std::ostringstream strs;
	strs << "peakHeigh " << pc.peakHeigh << "\n";
	strs << "peakWidth " << pc.peakWidth << "\n";
	strs << "peakWidthOrd " << pc.peakWidthOrd << "\n";
	strs << "peakVelocityR " << pc.peakVelocityR << "\n";
	strs << "peakVelocity2R " << pc.peakVelocity2R << "\n";
	strs << "symmetri " << pc.symmetri << "\n";
	strs << "peakVelocityL " << pc.peakVelocityL << "\n";
	strs << "peakVelocity2L " << pc.peakVelocity2L << "\n";
	strs << "peakLeftBorderFirstDerivative " << pc.peakLeftBorderFirstDerivative << "\n";
	strs << "peakFirstDerivative " << pc.peakFirstDerivative << "\n";
	strs << "peakRightBorderFirstDerivative " << pc.peakRightBorderFirstDerivative << "\n";
	strs << "peakLeftMiddleFirstDerivative " << pc.peakLeftMiddleFirstDerivative << "\n";
	strs << "peakRightMiddleFirstDerivative " << pc.peakRightMiddleFirstDerivative << "\n";
	strs << "peakLeftBorderSecondDerivative " << pc.peakLeftBorderSecondDerivative << "\n";
	strs << "peakSecondDerivative " << pc.peakSecondDerivative << "\n";
	strs << "peakRightBorderSecondDerivative " << pc.peakRightBorderSecondDerivative << "\n";
	strs << "peakLeftMiddleSecondDerivative " << pc.peakLeftMiddleSecondDerivative << "\n";
	strs << "peakRightMiddleSecondDerivative " << pc.peakRightMiddleSecondDerivative << "\n";

	return strs.str();
}

bool smartCalculation::saveResults(std::string filename)
{
	std::ostringstream strs;

	for (std::deque < deque <Results> >::iterator res2 = results.begin(); res2 != results.end(); ++res2)
	{
		for (deque <Results> ::iterator res = res2->begin(); res != res2->end(); ++res)
		{
			strs << "fpHall " << res->fpHall.getFilterParams().c_str() << "\n";
			strs << "fpRes " << res->fpRes.getFilterParams().c_str() << "\n";
			strs << "polinomPowHall " << res->polinomPowHall << "\n";
			strs << "polinomPowMagnetoresistance " << res->polinomPowMagnetoresistance << "\n";
			for (std::vector<long double >::iterator i = res->holeConcentration.begin(),
				 j = res->holeMobility.begin(),
				 k = res->electronConcentration.begin(),
				 l = res->electronMobility.begin(); 
				 i != res->holeConcentration.end(),
				 j != res->holeMobility.end(),
				 k != res->electronConcentration.end(),
				 l != res->electronMobility.end()
				 ; ++i,++j,++k,++l)
			{
				strs << j<< "\t" << i  << "\t" << l << "\t" << k << "\n";
			}

			strs << "targetFuncValue" << res->targetFuncValue << "\n";
			strs << "vPC\n";
			for (std::vector<PeaksCriteria>::iterator i = res->vPC.begin(); i != res->vPC.end(); ++i)
			{
				strs << convertToString(*i) << "\n";
			}

			strs << "additionalData " << convertToString(res->additionalData);
		}
	}

	TStringList * tsl= new TStringList();
	tsl->Text=strs.str().c_str();
	tsl->SaveToFile(filename.c_str());

	delete tsl;

	return true;
}

long double smartCalculation::targetFunction (Results & r, parameterType pt)
{
	switch (pt)
	{
		case ELECTRON_MOBILITY:

		/*
		ярко выраженный экстремум:
		высота экстремума электронов * величины проводимости электронов на границе вычисляемого спектра подвижности 100 м^2/(В∙с) / величины проводимости дырок на границе вычисляемого спектра подвижности 100 м^2/(В∙с)

		слабо выраженный:
		ширина пика электронов - величина первой производной для середины боковой стороны экстремума, соответствующего электронам
		*/
			return 1;
			break;
		case ELECTRON_DENSITY:
		/*
		слабо выраженный:
		высота экстремума электронов * величины проводимости электронов на границе вычисляемого спектра подвижности 100 м^2/(В∙с) / величины проводимости дырок на границе вычисляемого спектра подвижности 100 м^2/(В∙с) /ширина экстремума электронов
		*/
			return 1;
			break;
		case LIGHT_HOLE_MOBILITY:
		/*
		ярко выраженный экстремум:
		ширина пика легких дырок* ширина пика тяжелых дырок / квадрат ширины пика электронов
		*/
			return 1;
			break;
		case LIGHT_HOLE_DENSITY:
		/*
		ярко выраженный экстремум:
		величины проводимости электронов на границе вычисляемого спектра подвижности 100 м^2/(В∙с) / величины проводимости дырок на границе вычисляемого спектра подвижности 100 м^2/(В∙с)
		*/
			return 1;
			break;
		case HEAVY_HOLE_MOBILITY:
		/*
		ярко выраженный экстремум:
		ширина пика легких дырок / ширина пика электронов

		слабо выраженный:
		величины проводимости электронов на границе вычисляемого спектра подвижности 100 м^2/(В∙с)
		*/
			return 1;
			break;
		case HEAVY_HOLE_DENSITY:
		/*
		ярко выраженный экстремум:
		ширина пика легких дырок / ширина пика электронов
		*/
			return 1;
			break;
	}

	return 0;
}