#include "HallEffect.h"

#include "Unit1.h"




HallEffect::HallEffect(MyDataType current)

{
    NumberOfDecimalPlaces=5;
    h=0.001;
    NumberOfPoints=10;
    filterParams=new FilterParams();
    isRoundNeeded=true;
    Current=current;
    defaultExtension=".txt";
}

HallEffect::~HallEffect()
{
    delete filterParams;
    filterParams=0;
}


//------------Подгонка данных-------------------------------------------------
//-----Функция не доработана.

void HallEffect::featData(DataKind dataKind, long index, FeatType featType)
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
        //"Получен пустой массив данных!!!");
        return;
    }
    // параметры массив и переменная относительно которой функция нечетна
    // 0 для х, 1 для y

    if(index)
    {
        tempX.swap(tempY);// возможно тут я хватил лишнего.
    }
    for(int i=0;i<size/2;i++)
    {
        tempX[i]=(tempX[i]-tempX[size-1-i])/2.0;
        tempX[size-1-i]=-tempX[i];
        switch(featType)
        {
        case ODD_FEAT: // нечетная подгонка
            tempY[i]=(tempY[i]-tempY[size-1-i])/2.0;
            tempY[size-1-i]=-tempY[i];
            break;
        case EVEN_FEAT: // четная подгонка
            tempY[i]=(tempY[i]+tempY[size-1-i])/2.0;
            tempY[size-1-i]=tempY[i];
            break;
        }

    }

}


//-------------------------------------------------------------------------------
void HallEffect::filterData()
{
    filterDataHelper((*filterParams),HALL_EFFECT);
}
//-------------------------------------------------------------------------------
void HallEffect::filterData(FilterParams &fP)
{
    setFilterParams(fP.SamplingFrequecy, fP.BandwidthFrequency, fP.AttenuationFrequency, fP.filterLength);
    filterData();
}
//-------------------------------------------------------------------------------
void HallEffect::filterDataHelper(FilterParams &fP,
    PlotType dependenceType)
{
    if(HallEffect.size()!=B.size())
    {
        ShowMessage("Количество точек магнитного поля и эффекта Холла не совпадает! filterdataHelper");
        return;
    }
    NumberOfPoints=HallEffect.size();
    std::vector<long double> tempInB(2*NumberOfPoints);
    std::vector<long double> tempInSignal(2*NumberOfPoints);

    std::vector<long double> tempOutB(2*NumberOfPoints+ceil(fP.filterLength/2.0));
    std::vector<long double> tempOutSignal(2*NumberOfPoints+ceil(fP.filterLength/2.0));

    
    // формируем сигнал для фильтра.
    // достраивая его в отрицательные магнитные поля.
    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
        tempInSignal[i]=-HallEffect[NumberOfPoints-i-1]+2*HallEffect[0];
        tempInB[i]=-B[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=HallEffect[i];
        tempInB[i+NumberOfPoints]=B[i];
    }
    
    // фильтруем 
    TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                fP.filterLength,fP.SamplingFrequecy,fP.BandwidthFrequency,fP.AttenuationFrequency);

    // нагло записываем положительную часть фильтрованного сигнала обратно.
    for(unsigned int i=0;i<NumberOfPoints;i++)
    {    
        FilteredHallEffect.push_back(tempOutSignal[i+NumberOfPoints-1]);
    }
}

//-------------------------------------------------------------------------------

bool HallEffect::extrapolateData(const int polinomPowForHallEffect=4)
{
    bool returnValue=true;

    std::vector<long double> koefHallEffect(polinomPowForHallEffect+1);

    std::vector<long double> newB;
    std::vector<long double> newHallEffect;

    std::vector<long double> inBHall(FilteredB);
    std::vector<long double> inHallEffect(FilteredHallEffect);

    NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("Количество точек равно нулю! Я не хочу делить на ноль:)");
    return false;
    }
    h=2.0/NumberOfPoints;
    
    unsigned int i=0;
	for(int i=0;i<500;i++)
	{
		inBHall.push_back(0);
		inHallEffect.push_back(0);
	}

    curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,polinomPowForHallEffect);

    

	newB.clear();
	newB.push_back(0);
	for (unsigned int i = 1; i < NumberOfPoints; i++) {
		newB.push_back(newB[i-1]+h);
	}

	calculatePolinomByKoef(newB,koefHallEffect,newHallEffect);

    ExtrapolatedB=newB;

    ExtrapolatedHallEffect=newHallEffect;

	//----------А вот тут прикручиваем недостающий кусочек в сигналы----

	while(FilteredB[i++]<=0 && i<NumberOfPoints);
	i-=1; // ищем где поле становится положительным.

    for(unsigned int j=i;j<NumberOfPoints;j++)
	{      // перемещаем эти значения в начало.
		FilteredB[j-i]=FilteredB[j];
        FilteredHallEffect[j-i]=FilteredHallEffect[j];
		//Uy[j-i]=Uy[j];
	}

   /*	for(unsigned int j=NumberOfPoints-i;j<NumberOfPoints;j++)
	{     // в конце дописываем экстраполированные значения.
		FilteredB[j]=newB[j];
		Dependence[j]=newDependence[j];
	}
	//------------------------------------------------------------------
   */
return returnValue;   
}
//-------------------------------------------------------------------------------
void HallEffect::averagingData()
{
;              
}
//-------------------------------------------------------------------------------
void HallEffect::multiplyB(DataKind dataKind)
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
void HallEffect::constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color)
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
    ShowMessage("График пуст, строить нечего.");
    return;
    }
    DataTypeInContainer::iterator posX;
    DataTypeInContainer::iterator posY;
    for (posX=pointToX->begin(),posY=pointToY->begin();posX!=pointToX->end();++posX,++posY)
    	s->AddXY(*posX,*posY,"",color);
	
}
//-------------------------------------------------------------------------------
void HallEffect::constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color)
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
bool HallEffect::setFilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    filterParams->setFilterParams(StrToFloat(samplingFrequecy), StrToFloat(bandwidthFrequency), StrToFloat(attenuationFrequency), StrToInt(length));
    return true;
}
//-------------------------------------------------------------------------------
bool HallEffect::setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    filterParams->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
    return true;
}
//-------------------------------------------------------------------------------
void HallEffect::setRoundNeeded(bool needRound)
{
    isRoundNeeded=needRound;
}

//-------------------------------------------------------------------------------
void HallEffect::getSplittedDataFromADC()
{
    std::vector<DataTypeInContainer > tempData(adc->getSplittedData());
    int t=tempData[2].size();
    t=tempData[1].size();
    t=tempData[0].size();
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
        extrapolateData(4,4); // магические числа, степени полиномов для экстраполяции по умолчанию.
        // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.
    }
}
//-------------------------------------------------------------------------------

FilterParams const * HallEffect::getFilterParams()
{
    return filterParams;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getB()
{
    return B;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getOriginalB()
{
    return OriginalB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getFilteredB()
{
    return FilteredB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getExtrapolatedB()
{
    return ExtrapolatedB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getHallEffect()
{
    return HallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getMagnetoResistance()
{
    return MagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getOriginalHallEffect()
{
    return OriginalHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getOriginalMagnetoResistance()
{
    return OriginalMagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getFilteredHallEffect()
{
    return FilteredHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getFilteredMagnetoResistance()
{
    return FilteredMagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getExtrapolatedHallEffect()
{
    return ExtrapolatedHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const & HallEffect::getExtrapolatedMagnetoResistance()
{
    return ExtrapolatedMagnetoResistance;
}
//-------------------------------------------------------------------------------


