#include "DataSaver.h"


DataSaver::DataSaver(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber, AnsiString length, AnsiString width, AnsiString Thickness)
{
	setSampleDescription(Temperature, Current, SampleInventoryNumber, length, width, Thickness);
	isRoundNeeded=true;
	defaultExtension=".txt";
	NumberOfDecimalPlaces=5;
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



void DataSaver::SaveData(DataKind dataKind,DataTypeInContainer &B,
DataTypeInContainer & HallEffect, DataTypeInContainer & MagnetoResistance,
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


void DataSaver::SaveDataHelper(DataTypeInContainer &saveB,
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

	if (mode==POINTS_11) {

        FileName+="_11Points";
        const int SomePointsCount=11;
		long double points[SomePointsCount]={0};

		long double shag=2.0/(SomePointsCount-1.0);

		for (int i=1; i < SomePointsCount; i++) {
			points[i]=points[i-1]+shag;
		}

		for (int i = 0; i < SomePointsCount; i++) {
			unsigned int index=0;
			long double r=4;
			for(int k=0;k<length;k++)
			{
				if(fabs(fabs(savingXData[k])-fabs(points[i]))<=r)
				{
					r=fabs(fabs(savingXData[k])-fabs(points[i]));
					index=k;
				}
			}
            if(index<savingXData.size())
			tsl->Add(FloatToStrF(savingXData[index],ffFixed,9,5)+"\t"+FloatToStrF(savingY1Data[index],ffFixed,9,5)+"\t"+FloatToStrF(savingY2Data[index],ffFixed,9,5));
		}
	}
    if (mode==POINTS_21)
    {
        FileName+="_21Points";
        const int SomePointsCount=21;
        long double points[SomePointsCount]={-2.0};

        long double shag=4.0/(SomePointsCount-1.0);

        for (int i=1; i < SomePointsCount; i++) {
            points[i]=points[i-1]+shag;
        }

        for (int i = 0; i < SomePointsCount; i++) {
            unsigned int index=0;
            long double r=4;
            for(int k=0;k<length;k++)
            {
                if(fabs(savingXData[k]-points[i])<=r)
                {
                    r=fabs(savingXData[k]-points[i]);
                    index=k;
                }
            }
            if(index<savingXData.size())
            tsl->Add(FloatToStrF(savingXData[index],ffFixed,9,5)+"\t"+FloatToStrF(savingY1Data[index],ffFixed,9,5)+"\t"+FloatToStrF(savingY2Data[index],ffFixed,9,5));
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
