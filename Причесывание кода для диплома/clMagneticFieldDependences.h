//---------------------------------------------------------------------------

#ifndef clMagneticFieldDependencesH
#define clMagneticFieldDependencesH

#include "film.h"
#include "ExtrapolateUnit.h"
#include "FilteringUnit.h"
#include "NoiseUnit.h"
#include <cmath>
#include <vector>

#include <VCLTee.Series.hpp>

enum SignalType {SXX,SXY,US,UY,S_EFF,RH_EFF};
enum FileSaveMode {ALL_POINTS, SOME_POINTS}; // сейчас SOME_POINTS - это конечно 11 точек.
enum ModifyType {SHUM_ADDING, FILTERING, EXTRAPOLATE};

class clMagneticFieldDependences {

	public:
    friend void extrapolateNoiseFiltered(clMagneticFieldDependences * NoisyParams,clMagneticFieldDependences * FilteredParams,clMagneticFieldDependences * ExtrapolatedParams);


	clMagneticFieldDependences(int size,long double shag,long double molarCompositionCadmium,
	long double Temperature,long double heavyHoleConcentrerion,long double AFactor,long double KFactor,
	long double thickness,long double cbRatio,long double currentIntensity,long double numberOfCarrierTypes);

	clMagneticFieldDependences(int size,long double shag,film * cp);

	bool saveDataToFile(SignalType type, FileSaveMode saveMode, String filePath);

	~clMagneticFieldDependences();

	std::vector<long double> const & getSignalUs();
	std::vector<long double> const & getSignalUy();
	std::vector<long double> const & getSxx();
	std::vector<long double> const & getSxy();

	int modifySignals(ModifyType type,clMagneticFieldDependences * ExtrapolatedParams);

	int modifySignals(void (*ShumAdding)(std::vector<long double> const &x,std::vector<long double> &out,
	std::vector<long double> &ret, long double koeff,const int l),const std::vector<long double> & idealUs,
	 const std::vector<long double> & idealUy,std::vector<long double> &returnData,long double koeff);


	int modifySignals(double (*TrForMassiveFilter)(long double *inB,
	long double *inY,long double* outB,long double *outY,
	int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh),
	std::vector<long double> const & idealUs,std::vector<long double> const & idealUy,int lengthFilter);

	void setB_Us_Uy(long double *newB, long double * newUs,long double *newUy);

	void setB_Us_Uy(std::vector<long double> &newB, std::vector<long double> & newUs,std::vector<long double> &newUy);

	void calculateEffectiveParamsFromSignals();
	void calculateTenzorFromEffectiveParams();
	void calculateMagneticFieldDependences();
	void constructPlotFromTwoMassive(SignalType type,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(SignalType type, TLineSeries* s,TColor color);

	film *carrierParams;

	private:


	void calculateTenzorFromCarrierParams();
	void calculateEffectiveParamsFromTenzor();
	void calculateSignalsFromEffectiveParams();
	void calculateMagneticFieldPoints(); // расчет точек магнитного поля.
	void MemoryAllocation(); // выделение памяти.

	static const long double THEALMOSTZERO;
	static const long double electronCharge;

	long double h;
	const int NumberOfPoints;

	/*long double *sxx;
	long double *sxy;
	long double *B;
	long double *Us;
	long double *Uy;
	long double *s_eff;
	long double *Rh_eff;*/

	std::vector<long double> sxx;
	std::vector<long double> sxy;
	std::vector<long double> B;
	std::vector<long double> Us; // продольный сигнал
	std::vector<long double> Uy; // поперечный сигнал
	std::vector<long double> s_eff;
	std::vector<long double> Rh_eff;

};


//---------------------------------------------------------------------------
#endif
