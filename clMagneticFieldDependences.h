//---------------------------------------------------------------------------

#ifndef clMagneticFieldDependencesH
#define clMagneticFieldDependencesH

#include "film.h"
#include "ExtrapolateUnit.h"
#include "NoiseUnit.h"
#include <cmath>
#include <vector>
#include "commonFunctions.h"

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

	//loadDataFromFile(ChartType * type, string )
	bool saveDataToFile(SignalType type, FileSaveMode saveMode, String filePath);

	~clMagneticFieldDependences();

	TSignal const & getSignalB();
	TSignal const & getSignalUs();
	TSignal const & getSignalUy();
	TSignal const & getSxx();
	TSignal const & getSxy();

	int modifySignals(ModifyType type,clMagneticFieldDependences * ExtrapolatedParams);

	int modifySignals(void (*ShumAdding)(TSignal const &x,TSignal &out,
	TSignal &ret, long double koeff,const int l),const TSignal & idealUs,
	 const TSignal & idealUy,TSignal &returnData,long double koeff);

	int modifySignals(void (*ShumAdding)(TSignal const &x,TSignal &out,
	TSignal &ret, long double koeff,const int l,long double mzr, int amplitude),
	const TSignal & idealB,
	const TSignal & idealUs,
	const TSignal & idealUy,TSignal &returnData,long double koeff,
	long double mzr, int amplitude);

	int modifySignals(long double (*TrForMassiveFilter)(TSignal & inB,
	TSignal & inY,TSignal & outB,
	TSignal & outY,int lengthFilter,long double Fdisk,
	long double Fpropysk,long double Fzatyh),
	TSignal const & idealUs,TSignal const & idealUy,int lengthFilter);



	void setB_Us_Uy(long double *newB, long double * newUs,long double *newUy);

	void setB_Us_Uy(TSignal &newB, TSignal & newUs,TSignal &newUy);

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

	TSignal sxx;
	TSignal sxy;
	TSignal B;
	TSignal Us; // продольный сигнал
	TSignal Uy; // поперечный сигнал
	TSignal s_eff;
	TSignal Rh_eff;

};


//---------------------------------------------------------------------------
#endif
