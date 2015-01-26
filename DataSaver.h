#ifndef DATASAVER_H
#define DATASAVER_H 

#include <string>
#include <vcl.h>
#include <math>
#include "commonFunctions.h"

#include "UsedTypes.h"

enum SaveType {ALL_POINTS,POINTS_11,POINTS_21};

class DataSaver
{
public:
	DataSaver(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness);
	~DataSaver();

	void setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness);
	void getSampleDescription(AnsiString &Temperature, AnsiString &Current, AnsiString &SampleInventoryNumber, AnsiString &length, AnsiString &width, AnsiString &Thickness);

	void SaveSampleDescription(AnsiString FileName);

void DataSaver::SaveData(DataKind dataKind,const TSignal *B,
const TSignal * HallEffect, const TSignal * MagnetoResistance,
SaveType saveType,AnsiString FileName);

    void setBaseFileName(AnsiString FileName);
	void setRoundNeeded(bool needRound);

	void setParamsType(ParamsType pt);

private:

	ParamsType paramsType;
	TSignal leftBound;
	TSignal rightBound;

	AnsiString BaseFileName;

	AnsiString defaultExtension;

	// Сохранение результатов и т.д.----------------------------------
	unsigned int NumberOfDecimalPlaces;
	bool isRoundNeeded;

	AnsiString Temperature; // температура образца во время измерений.
	AnsiString Current; // ток на образце, в амперах.

	AnsiString SampleInventoryNumber; // инвентарный номер образца.

	AnsiString SampleLength; // длина образца, мм
	AnsiString SampleWidth; // ширина образца, мм
	AnsiString SampleThickness; // толщина образца, мкм
    
    void SaveDataHelper(const TSignal *saveB,
	const TSignal * saveHall,
	const TSignal * saveResistance,SaveType mode,
	AnsiString FileName);
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);
	
};

#endif