#include "MagneticFieldDependence.h"
#include "Unit1.h"




MagneticFieldDependence::MagneticFieldDependence(MyDataType current)

{
    NumberOfDecimalPlaces=5;
    h=0.001;
    NumberOfPoints=10;
    filterParams=new FilterParams();
    isRoundNeeded=true;
    Current=current;
    defaultExtension=".txt";
}

MagneticFieldDependence::~MagneticFieldDependence()
{
    delete filterParams;
    filterParams=0;
}



void MagneticFieldDependence::SaveAllData(AnsiString FileName)
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

void MagneticFieldDependence::SaveData(DataKind dataKind,SaveType saveType,AnsiString FileName)
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


void MagneticFieldDependence::SaveDataHelper(DataTypeInContainer &saveB,
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
// ќкругление с заданной точностью.
template <class T>
void MagneticFieldDependence::RoundM(T *pos, T* endPos)
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

//------------ѕодгонка данных-------------------------------------------------
//-----‘ункци€ не доработана.

void MagneticFieldDependence::featData(DataKind dataKind, long index, FeatType featType)
{
    DataTypeInContainer tempX;
    DataTypeInContainer tempY;
    /*
    switch(dataKind)
    {
    case CURRENT_DATA:
        tempX=B;
        tempY=Dependence;
        break;
    case FILTERED_DATA:
        tempX=FilteredB;
        tempY=FilteredDependence;
        break;
    case EXTRAPOLATED_DATA:
        tempX=ExtrapolatedB;
        tempY=ExtrapolatedDependence;
        break;
    case ORIGINAL_DATA:
        tempX=OriginalB;
        tempY=OriginalDependence;
        break;
    default:
        return;
    }  */
    //------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int size=tempX.size();
    if(size==0)
    {
        //"ѕолучен пустой массив данных!!!");
        return;
    }
    // параметры массив и переменна€ относительно которой функци€ нечетна
    // 0 дл€ х, 1 дл€ y

    if(index)
    {
        tempX.swap(tempY);// возможно тут € хватил лишнего.
    }
    for(int i=0;i<size/2;i++)
    {
        tempX[i]=(tempX[i]-tempX[size-1-i])/2.0;
        tempX[size-1-i]=-tempX[i];
        switch(featType)
        {
        case ODD_FEAT: // нечетна€ подгонка
            tempY[i]=(tempY[i]-tempY[size-1-i])/2.0;
            tempY[size-1-i]=-tempY[i];
            break;
        case EVEN_FEAT: // четна€ подгонка
            tempY[i]=(tempY[i]+tempY[size-1-i])/2.0;
            tempY[size-1-i]=tempY[i];
            break;
        }

    }

}


//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData()
{
    filterDataHelper((*filterParams),HALL_EFFECT);
    filterDataHelper((*filterParams),MAGNETORESISTANCE);
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData(FilterParams &fP)
{
    setFilterParams(fP.SamplingFrequecy, fP.BandwidthFrequency, fP.AttenuationFrequency, fP.filterLength);
    filterData();
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    PlotType dependenceType)
{
    NumberOfPoints=HallEffect.size();   
    std::vector<long double> tempInB(2*NumberOfPoints);
    std::vector<long double> tempInSignal(2*NumberOfPoints);

    std::vector<long double> tempOutB(2*NumberOfPoints+ceil(fP.filterLength/2.0));
    std::vector<long double> tempOutSignal(2*NumberOfPoints+ceil(fP.filterLength/2.0));

    switch(dependenceType)
    {
    case HALL_EFFECT:
    // формируем сигнал дл€ фильтра.
    // достраива€ его в отрицательные магнитные пол€.
    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
        tempInSignal[i]=-HallEffect[NumberOfPoints-i-1]+2*HallEffect[0];
        tempInB[i]=-B[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=HallEffect[i];
        tempInB[i+NumberOfPoints]=B[i];
    }
    break;
    case MAGNETORESISTANCE:

    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
        tempInSignal[i]=MagnetoResistance[NumberOfPoints-i-1];   // чет
        //tempInSignal[i]=-Dependence[NumberOfPoints-i-1]+2*Dependence[0];  // нечет
        tempInB[i]=-B[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=MagnetoResistance[i];
        tempInB[i+NumberOfPoints]=B[i];
    }
    break;

    default:
        break;
    }

    // фильтруем 
    TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                fP.filterLength,fP.SamplingFrequecy,fP.BandwidthFrequency,fP.AttenuationFrequency);


    // нагло записываем положительную часть фильтрованного сигнала обратно.
    for(unsigned int i=0;i<NumberOfPoints;i++)
    {
        
    switch(dependenceType)
    {        
    case HALL_EFFECT:
        FilteredHallEffect.push_back(tempOutSignal[i+NumberOfPoints-1]);
        
        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance.push_back(tempOutSignal[i+NumberOfPoints-1]);
        FilteredB.push_back(tempOutB[i+NumberOfPoints-1]);
        break;
    default:
        break;
    }
    }
}

//-------------------------------------------------------------------------------

bool MagneticFieldDependence::extrapolateData(const int polinomPowForMagnetoResistance=4, const int polinomPowForHallEffect=4)
{
    bool returnValue=true;

    std::vector<long double> koefMagnetoResistance(polinomPowForMagnetoResistance+1);
    std::vector<long double> koefHallEffect(polinomPowForHallEffect+1);

    std::vector<long double> newB;
    std::vector<long double> newHallEffect;
    std::vector<long double> newMagnetoResistance;

    std::vector<long double> inB(FilteredB);
    std::vector<long double> inHallEffect(FilteredHallEffect);
    std::vector<long double> inMagnetoResistance(FilteredMagnetoResistance);
    NumberOfPoints=inB.size();
    if(NumberOfPoints==0)
    {
    ShowMessage(" оличество точек равно нулю! я не хочу делить на ноль:)");
    return false;
    }
    h=2.0/NumberOfPoints;
    unsigned int i=0;
	for(int i=0;i<500;i++)
	{
		inB.push_back(0);
		inHallEffect.push_back(0);
	}

    curveFittingUniversal(&inB,&inHallEffect, &koefHallEffect,polinomPowForHallEffect);
    curveFittingUniversal(&inB,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance);

	newB.clear();
	newB.push_back(0);
	for (unsigned int i = 1; i < NumberOfPoints; i++) {
		newB.push_back(newB[i-1]+h);
	}

	calculatePolinomByKoef(newB,koefMagnetoResistance,newMagnetoResistance);
	calculatePolinomByKoef(newB,koefHallEffect,newHallEffect);

    ExtrapolatedB=newB;

    ExtrapolatedMagnetoResistance=newMagnetoResistance;
    ExtrapolatedHallEffect=newHallEffect;

	//----------ј вот тут прикручиваем недостающий кусочек в сигналы----

	while(FilteredB[i++]<=0 && i<NumberOfPoints);
	i-=1; // ищем где поле становитс€ положительным.

    for(unsigned int j=i;j<NumberOfPoints;j++)
	{      // перемещаем эти значени€ в начало.
		FilteredB[j-i]=FilteredB[j];
		FilteredMagnetoResistance[j-i]=FilteredMagnetoResistance[j];
        FilteredHallEffect[j-i]=FilteredHallEffect[j];
		//Uy[j-i]=Uy[j];
	}

   /*	for(unsigned int j=NumberOfPoints-i;j<NumberOfPoints;j++)
	{     // в конце дописываем экстраполированные значени€.
		FilteredB[j]=newB[j];
		Dependence[j]=newDependence[j];
	}
	//------------------------------------------------------------------
   */
return returnValue;   
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::averagingData()
{
;              
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::multiplyB(DataKind dataKind)
{
    DataTypeInContainer * temp;

    switch (dataKind)
    {
    case CURRENT_DATA:
        temp=&B;
        break;
    case FILTERED_DATA:
        temp=&FilteredB;
        break;
    case EXTRAPOLATED_DATA:
        temp=&ExtrapolatedB;
        break;
    case ORIGINAL_DATA:
        temp=&OriginalB;
        break;
    default:
        return;
    }

    DataTypeInContainer::iterator pos;
    for(pos=temp->begin();pos!=temp->end();++pos)
    *pos*=10;
}  
//-------------------------------------------------------------------------------
inline void MagneticFieldDependence::ReplaceDotsToComma(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich=","; // на что мен€ем
	std::string strToSearch=".";   // что ищем
	while ((findIndex=s.find(strToSearch,findIndex))!=std::string::npos)
    {
        s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
        strToReplaceWhich.begin(),strToReplaceWhich.end());
    } 
    out=s;
}
//-------------------------------------------------------------------------------
inline void MagneticFieldDependence::ReplaceCommaToDots(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich="."; // на что мен€ем
	std::string strToSearch=",";   // что ищем
    while ((findIndex=s.find(strToSearch,findIndex))!=std::string::npos)
    {
    	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
        strToReplaceWhich.begin(),strToReplaceWhich.end());
    } 
    out=s;
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color)
{
    DataTypeInContainer * pointToX=0;
    DataTypeInContainer * pointToY=0;
	s->Clear();
    switch(pt)
    {
    case HALL_EFFECT:
    switch(dk)
    {
    case CURRENT_DATA:
        pointToX=&B;
        pointToY=&HallEffect;
        break;
    case FILTERED_DATA:
        pointToX=&FilteredB;
        pointToY=&FilteredHallEffect;
        break;
    case EXTRAPOLATED_DATA:
        pointToX=&ExtrapolatedB;
        pointToY=&ExtrapolatedHallEffect;
        break;
    case ORIGINAL_DATA:
        pointToX=&OriginalB;
        pointToY=&OriginalHallEffect;
        break;
    default:
        break;
    }
    break;
    case MAGNETORESISTANCE:
    switch(dk)
    {
    case CURRENT_DATA:
        pointToX=&B;
        pointToY=&MagnetoResistance;
        break;
    case FILTERED_DATA:
        pointToX=&FilteredB;
        pointToY=&FilteredMagnetoResistance;
        break;
    case EXTRAPOLATED_DATA:
        pointToX=&ExtrapolatedB;
        pointToY=&ExtrapolatedMagnetoResistance;
        break;
    case ORIGINAL_DATA:
        pointToX=&OriginalB;
        pointToY=&OriginalMagnetoResistance;
        break;
    default:
        break;
    }
    break;
    default:
    break;
    }
    NumberOfPoints=pointToX->size();
    if(NumberOfPoints==0)
    {
    ShowMessage("√рафик пуст, строить нечего.");
    return;
    }
    DataTypeInContainer::iterator posX;
    DataTypeInContainer::iterator posY;
    for (posX=pointToX->begin(),posY=pointToY->begin();posX!=pointToX->end();++posX,++posY)
    	s->AddXY(*posX,*posY,"",color);
	
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color)
{
    DataTypeInContainer * temp;
	s->Clear();
    switch(p)
    {
    case MAGNETIC_FIELD:
        temp=&B;
        break;
    case HALL_EFFECT:
        temp=&HallEffect;
        break;
    case MAGNETORESISTANCE:
        temp=&MagnetoResistance;
        break;
    default:
        break;
    }
        NumberOfPoints=temp->size();
	for (unsigned int i = 0; i < NumberOfPoints; i++)
	{
		s->AddY((*temp)[i],"",color);
	} 
}

//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    filterParams->setFilterParams(StrToFloat(samplingFrequecy), StrToFloat(bandwidthFrequency), StrToFloat(attenuationFrequency), StrToInt(length));
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    filterParams->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
    return true;
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::setRoundNeeded(bool needRound)
{
    isRoundNeeded=needRound;
}

//-------------------------------------------------------------------------------
void MagneticFieldDependence::getSplittedDataFromADC()
{
    std::vector<DataTypeInContainer > tempData(adc->getSplittedData());
    int t=tempData[2].size();
    t;
    //if(tempData.size()>3) // если это не тестовые замерки
    {
        OriginalB=tempData[2];
        OriginalHallEffect=tempData[0]; // вот тут мог перепутать, надо будеть проверить.
        OriginalMagnetoResistance=tempData[1];
        
        B=OriginalB;
        HallEffect=OriginalHallEffect;
        MagnetoResistance=OriginalMagnetoResistance;

        multiplyB(CURRENT_DATA);
        filterData();
        extrapolateData(4,4); // магические числа, степени полиномов дл€ экстрапол€ции по умолчанию.
        // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещЄ.
    }
}
//-------------------------------------------------------------------------------

FilterParams const * MagneticFieldDependence::getFilterParams()
{
    return filterParams;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getB()
{
    return B;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getOriginalB()
{
    return OriginalB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getFilteredB()
{
    return FilteredB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getExtrapolatedB()
{
    return ExtrapolatedB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getHallEffect()
{
    return HallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getMagnetoResistance()
{
    return MagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getOriginalHallEffect()
{
    return OriginalHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getOriginalMagnetoResistance()
{
    return OriginalMagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getFilteredHallEffect()
{
    return FilteredHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getFilteredMagnetoResistance()
{
    return FilteredMagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getExtrapolatedHallEffect()
{
    return ExtrapolatedHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getExtrapolatedMagnetoResistance()
{
    return ExtrapolatedMagnetoResistance;
}
//-------------------------------------------------------------------------------


