#ifndef DATASAVER_H
#define DATASAVER_H 

#include <string>
#include <vcl.h>
#include <math.h>
//#include <System.hpp>
#include "UsedTypes.h"

enum SaveType {ALL_POINTS,POINTS_11,POINTS_21};
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};

class DataSaver
{
public:
	DataSaver(MyDataType Temperature, MyDataType Current, AnsiString SampleInventoryNumber);
	~DataSaver();


void DataSaver::SaveData(DataKind dataKind,DataTypeInContainer &B,
DataTypeInContainer & HallEffect, DataTypeInContainer & MagnetoResistance,
SaveType saveType,AnsiString FileName);

	/*void SaveAllData(AnsiString FileName,bool isCombinedParams=false)
{
    SaveData(CURRENT_DATA,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    SaveData(CURRENT_DATA,ALL_POINTS,FileName);
    SaveData(FILTERED_DATA,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    SaveData(FILTERED_DATA,ALL_POINTS,FileName);
    SaveData(EXTRAPOLATED_DATA,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    SaveData(EXTRAPOLATED_DATA,ALL_POINTS,FileName);
    SaveData(ORIGINAL_DATA,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    SaveData(ORIGINAL_DATA,ALL_POINTS,FileName);
}*/

    void setBaseFileName(AnsiString FileName);
	void setRoundNeeded(bool needRound);

private:

	AnsiString BaseFileName;

	AnsiString defaultExtension;

	// Сохранение результатов и т.д.----------------------------------
	unsigned int NumberOfDecimalPlaces;
	bool isRoundNeeded;

	MyDataType Temperature; // температура образца во время измерений.
	MyDataType Current; // ток на образце, в амперах.

	AnsiString SampleInventoryNumber; // инвентарный номер образца.

	
    template <class T>
    void RoundM(T *pos, T* endPos);

    
    void SaveDataHelper(DataTypeInContainer &saveB,
	DataTypeInContainer & saveHall,
	DataTypeInContainer & saveResistance,SaveType mode,
	AnsiString FileName);
	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);
	
};

#endif