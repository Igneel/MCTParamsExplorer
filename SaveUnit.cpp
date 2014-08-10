#include "SaveUnit.h"

SaveUnit::SaveUnit(MagneticFieldDependence & params)
{
	this->params=params;
}

SaveUnit::~SaveUnit()
{

}


void SaveUnit::SaveAllData(AnsiString FileName)
{
    SaveData(CURRENT_DATA,SOME_POINTS,FileName);
    SaveData(CURRENT_DATA,ALL_POINTS,FileName);
    SaveData(FILTERED_DATA,SOME_POINTS,FileName);
    SaveData(FILTERED_DATA,ALL_POINTS,FileName);
    SaveData(EXTRAPOLATED_DATA,SOME_POINTS,FileName);
    SaveData(EXTRAPOLATED_DATA,ALL_POINTS,FileName);
    SaveData(ORIGINAL_DATA,SOME_POINTS,FileName);
    SaveData(ORIGINAL_DATA,ALL_POINTS,FileName);
}

void SaveUnit::SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName)
{

AnsiString NewFileName;
switch(dataKind)
{
    case CURRENT_DATA:
        NewFileName=FileName+"_current_data";
        SaveDataHelper(B,HallEffect,MagnetoResistance,saveType,NewFileName);
        break;
    case FILTERED_DATA:
        NewFileName=FileName+"_filtered_data";
        SaveDataHelper(FilteredB,FilteredHallEffect,FilteredMagnetoResistance,saveType,NewFileName);       
        break;
    case EXTRAPOLATED_DATA:
        NewFileName=FileName+"_extrapolated_data";
        SaveDataHelper(ExtrapolatedB,ExtrapolatedHallEffect,ExtrapolatedMagnetoResistance,saveType,NewFileName);      
        break;
    case ORIGINAL_DATA:
        NewFileName=FileName+"_original_data";
        SaveDataHelper(OriginalB,OriginalHallEffect,OriginalMagnetoResistance,saveType,NewFileName);       
        break;
}

}


void SaveUnit::SaveDataHelper(DataTypeInContainer &saveB,
DataTypeInContainer & saveHall,
DataTypeInContainer & saveResistance,SaveType mode, AnsiString FileName)
{
    TStringList * tsl=new TStringList();

    DataTypeInContainer savingXData(saveB.begin(),saveB.end());
    DataTypeInContainer savingY1Data(saveResistance.begin(),saveResistance.end());
    DataTypeInContainer savingY2Data(saveHall.begin(),saveHall.end());

    DataTypeInContainer::iterator pos;

    int length=savingXData.size();
    if(isRoundNeeded==true)
	{
		RoundM(savingXData.begin(),savingXData.end());
		RoundM(savingY1Data.begin(),savingY1Data.end());
		RoundM(savingY2Data.begin(),savingY2Data.end());
	}

	if (mode==SOME_POINTS) {

        FileName+="_11Points";
        const int SomePointsCount=11;
		long double points[SomePointsCount]={0};

		long double shag=2.0/(SomePointsCount-1.0);

		for (int i=1; i < SomePointsCount; i++) {
			points[i]=points[i-1]+shag;
		}

		for (int i = 0; i < SomePointsCount; i++) {
			int index=0;
			long double r=4;
			for(int k=0;k<length;k++)
			{
				if(fabs(fabs(savingXData[k])-fabs(points[i]))<=r)
				{
					r=fabs(savingXData[k]-points[i]);
					index=k;
				}
			}

			tsl->Add(FloatToStr(savingXData[index])+"\t"+FloatToStr(savingY1Data[index])+"\t"+FloatToStr(savingY2Data[index]));
		}
	}
	if(mode==ALL_POINTS)
	{
        FileName+="_AllPoints";
		for(int i=0;i<length;i++)
		{
			tsl->Add(FloatToStr(savingXData[i])+"\t"+FloatToStr(savingY1Data[i])+"\t"+FloatToStr(savingY2Data[i]));
		}
	}
    std::string text=tsl->Text.c_str();
   
    ReplaceCommaToDots(text,text);
    tsl->Text=text.c_str();

    FileName+=defaultExtension;
	tsl->SaveToFile(FileName); 	

	delete tsl;
}

//---------------------------------------------------------------------------
// Округление с заданной точностью.
template <class T>
void SaveUnit::RoundM(T *pos, T* endPos)
{
    int S=pow(10,NumberOfDecimalPlaces);
    for(;pos!=endPos;++pos)
    {
        int n=(int)(*pos*S)%10;
        if(n<5)
            *pos=floorl(*pos*S)/S;
        else
            *pos=ceill(*pos*S)/S;
    }
}


//-------------------------------------------------------------------------------
inline void SaveUnit::ReplaceDotsToComma(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich=","; // на что меняем
	std::string strToSearch=".";   // что ищем
	while ((findIndex=s.find(strToSearch,findIndex))!=std::string::npos)
    {
        s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
        strToReplaceWhich.begin(),strToReplaceWhich.end());
    } 
    out=s;
}
//-------------------------------------------------------------------------------
inline void SaveUnit::ReplaceCommaToDots(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich="."; // на что меняем
	std::string strToSearch=",";   // что ищем
    while ((findIndex=s.find(strToSearch,findIndex))!=std::string::npos)
    {
    	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
        strToReplaceWhich.begin(),strToReplaceWhich.end());
    } 
    out=s;
}

//-------------------------------------------------------------------------------
void SaveUnit::setRoundNeeded(bool needRound)
{
    isRoundNeeded=needRound;
}
