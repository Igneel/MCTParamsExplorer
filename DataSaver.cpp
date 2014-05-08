#include "DataSaver.h"


DataSaver::DataSaver(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness)
{
	setSampleDescription(Temperature, Current, SampleInventoryNumber, length, width, Thickness);
	isRoundNeeded=true;
	defaultExtension=".txt";
	NumberOfDecimalPlaces=5;
	paramsType=DIRECT;

	leftBound.resize(3);
	leftBound[DIRECT]=0;
	leftBound[REVERSE]=-2;
	leftBound[COMBINE]=-2;
	rightBound.resize(3);
	rightBound[DIRECT]=2;
	rightBound[REVERSE]=0;
	rightBound[COMBINE]=2;
}

void DataSaver::setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness)
{
	this->Current=Current;
	this->SampleInventoryNumber=SampleInventoryNumber;
	this->Temperature=Temperature;	
	this->SampleLength=length;
	this->SampleWidth=width;
	this->SampleThickness=Thickness;
}

void DataSaver::getSampleDescription(AnsiString &Temperature, AnsiString &Current, AnsiString &SampleInventoryNumber, AnsiString &length, AnsiString &width, AnsiString &Thickness)
{
	if(Current!=NULL)
	Current=this->Current;
	if(SampleInventoryNumber!=NULL)
	SampleInventoryNumber=this->SampleInventoryNumber;
	if(Temperature!=NULL)
	Temperature=this->Temperature;
	if(length!=NULL)
	length=this->SampleLength;
	if(width!=NULL)
	width=this->SampleWidth;
	if(Thickness!=NULL)
	Thickness=this->SampleThickness;
}

void DataSaver::SaveSampleDescription(AnsiString FileName)
{
	AnsiString NewFileName;
	NewFileName=FileName+"_SampleN_"+SampleInventoryNumber+"_T_"+Temperature+"_I_"+Current+"Description";
	TStringList * tsl=new TStringList();

	tsl->Add("SampleInventoryNumber\t"+SampleInventoryNumber);
	tsl->Add("Temperature\t"+Temperature);
	tsl->Add("Current\t"+Current);
	tsl->Add("SampleLength\t"+SampleLength);
	tsl->Add("SampleWidth\t"+SampleWidth);
	tsl->Add("SampleThickness\t"+SampleThickness);

	FileName+=defaultExtension;
	tsl->SaveToFile(FileName); 	

	delete tsl;
}

DataSaver::~DataSaver()
{
	;
}



void DataSaver::SaveData(DataKind dataKind,const DataTypeInContainer *B,
const DataTypeInContainer * HallEffect, const DataTypeInContainer * MagnetoResistance,
SaveType saveType,AnsiString FileName)
{

	AnsiString NewFileName;
	NewFileName=FileName+"_SampleN_"+SampleInventoryNumber+"_T_"+Temperature+"_I_"+Current;
	switch(dataKind)
	{
	    case CURRENT_DATA:
	        NewFileName+="_current_data";
	        break;
	    case FILTERED_DATA:
	        NewFileName+="_filtered_data";
	        break;
	    case EXTRAPOLATED_DATA:
	        NewFileName+="_extrapolated_data";
	        break;
	    case ORIGINAL_DATA:
	        NewFileName+="_original_data";
	        break;
	}

	SaveDataHelper(B,HallEffect,MagnetoResistance,saveType,NewFileName);

}


void DataSaver::SaveDataHelper(const DataTypeInContainer *saveB,
const DataTypeInContainer * saveHall,
const DataTypeInContainer * saveResistance,SaveType mode, AnsiString FileName)
{
    TStringList * tsl=new TStringList();

    DataTypeInContainer savingXData(saveB->begin(),saveB->end());
    DataTypeInContainer savingY1Data(saveResistance->begin(),saveResistance->end());
    DataTypeInContainer savingY2Data(saveHall->begin(),saveHall->end());

    DataTypeInContainer::iterator pos;

    int length=savingXData.size();
    if(isRoundNeeded==true)
	{
		RoundM(savingXData.begin(),savingXData.end());
		RoundM(savingY1Data.begin(),savingY1Data.end());
		RoundM(savingY2Data.begin(),savingY2Data.end());
	}

	if (mode==POINTS_11) 
	{
        FileName+="_11Points";
        DataTypeInContainer tempX;
        DataTypeInContainer tempY;
        thiningSignal(savingXData,savingY1Data,tempX,tempY,leftBound[paramsType],rightBound[paramsType],11);
        savingY1Data = tempY;

        thiningSignal(savingXData,savingY2Data,tempX,tempY,leftBound[paramsType],rightBound[paramsType],11);
        savingXData = tempX;
        savingY2Data = tempY;

        for (unsigned int i = 0; i < savingXData.size(); ++i)
        {
        	tsl->Add(FloatToStrF(savingXData[i],ffFixed,9,5)+"\t"+FloatToStrF(savingY1Data[i],ffFixed,9,5)+"\t"+FloatToStrF(savingY2Data[i],ffFixed,9,5));
        }
	}
    if (mode==POINTS_21)
    {
        FileName+="_21Points";
        
        DataTypeInContainer tempX;
        DataTypeInContainer tempY;
        thiningSignal(savingXData,savingY1Data,tempX,tempY,leftBound[paramsType],rightBound[paramsType],21);
        savingY1Data = tempY;

        thiningSignal(savingXData,savingY2Data,tempX,tempY,leftBound[paramsType],rightBound[paramsType],21);
        savingXData = tempX;
        savingY2Data = tempY;

        for (unsigned int i = 0; i < savingXData.size(); ++i)
        {
        	tsl->Add(FloatToStrF(savingXData[i],ffFixed,9,5)+"\t"+FloatToStrF(savingY1Data[i],ffFixed,9,5)+"\t"+FloatToStrF(savingY2Data[i],ffFixed,9,5));
        }        
    }
	if(mode==ALL_POINTS)
	{
        FileName+="_AllPoints";
		for(int i=0;i<length;i++)
		{
			tsl->Add(FloatToStrF(savingXData[i],ffFixed,9,5)+"\t"+FloatToStrF(savingY1Data[i],ffFixed,9,5)+"\t"+FloatToStrF(savingY2Data[i],ffFixed,9,5));
		}
	}
    //std::string text=tsl->Text.c_str();
   
    //ReplaceCommaToDots(text,text);
    //tsl->Text=text.c_str();

    FileName+=defaultExtension;
	tsl->SaveToFile(FileName); 	

	delete tsl;
}

//---------------------------------------------------------------------------
// Округление с заданной точностью.
template <class T>
void DataSaver::RoundM(T *pos, T* endPos)
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

void DataSaver::setRoundNeeded(bool needRound)
{
    isRoundNeeded=needRound;
}

//-------------------------------------------------------------------------------
inline void DataSaver::ReplaceDotsToComma(std::string &in, std::string & out)
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
inline void DataSaver::ReplaceCommaToDots(std::string &in, std::string & out)
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

void DataSaver::setParamsType(ParamsType pt)
{
	paramsType=pt;
}