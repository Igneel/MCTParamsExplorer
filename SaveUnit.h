#ifndef SAVEUNIT_H
#define SAVEUNIT_H 

#include <vector>
#include <algorithm>

#include "UsedTypes.h"



class SaveUnit
{
public:
	SaveUnit(MagneticFieldDependence & params);
	~SaveUnit();

	void SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName);
	void SaveAllData(AnsiString FileName);

	void setRoundNeeded(bool needRound);
	
private:

	MagneticFieldDependence & params;

	bool isRoundNeeded;

	AnsiString defaultExtension;
    template <class T>
    void SaveUnit::RoundM(T *pos, T* endPos);

    void SaveUnit::SaveDataHelper(TSignal &saveB,
	TSignal & saveHall,
	TSignal & saveResistance,SaveType mode,
	AnsiString FileName);


	inline void ReplaceDotsToComma(std::string &in, std::string & out);
	inline void ReplaceCommaToDots(std::string &in, std::string & out);

};

#endif