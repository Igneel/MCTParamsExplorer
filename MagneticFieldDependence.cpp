#include "MagneticFieldDependence.h"
extern LCardADC *adc;


void MagneticFieldDependence::enableChangeChannels()
{
ChangeChannels=true;
}

void MagneticFieldDependence::disableChangeChannels()
{
ChangeChannels=false;
}



MagneticFieldDependence::MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
    AnsiString length, AnsiString width, AnsiString Thickness)

{
    ChangeChannels=false;
    filterParamsHall=new FilterParams(); // по идее нужно бы и инциализировать их тут, дабы не было проблем в случае чего:).
    filterParamsResistance=new FilterParams();
    saver =new DataSaver(temperature,current,SampleInventoryNumber, length, width, Thickness);
}

MagneticFieldDependence::~MagneticFieldDependence()
{
    delete saver;
    saver=0;
    delete filterParamsHall;
    filterParamsHall=0;
    delete filterParamsResistance;
    filterParamsResistance=0;
}


void MagneticFieldDependence::loadDataHelper(DataTypeInContainer &temp, String AnsiS,const std::string delimiter)
{
    temp.clear();
    std::string s1 = AnsiS.c_str();
    std::string s; 
    std::string::size_type start = 0;
    std::string::size_type finish = 0;
    ReplaceDotsToComma(s1,s);
    do 
        {
            finish = s.find(delimiter, start);
            std::string word = s.substr(start, finish-start);
            temp.push_back(StrToFloat(word.c_str()));
            start=finish+1;
        } while (finish != std::string::npos);
}

void MagneticFieldDependence::loadData(TStringList * tts)
{
    const std::string delimiterTab = "\t";
    const std::string delimiterSpace = " ";
    const int numberOfColls=3;

    DataTypeInContainer B;
    DataTypeInContainer Hall;
    DataTypeInContainer Resistance;
    DataTypeInContainer temp;

    for(int i=0;i<tts->Count;i++) // по количеству строк
    {
        if(tts->Strings[i].IsEmpty()) // пустые строки пропускаем
            continue;
            
        loadDataHelper(temp,tts->Strings[i],delimiterTab);

        if(temp.size()!=numberOfColls)
        {          
        loadDataHelper(temp,tts->Strings[i],delimiterSpace);
        if(temp.size()!=numberOfColls)
            return;
        }

        B.push_back(temp[0]);
        Resistance.push_back(temp[1]);
        Hall.push_back(temp[2]);
    }

    setDependence(B.begin(),B.end(),Hall.begin(),Resistance.begin());

}

void MagneticFieldDependence::SaveAllData(AnsiString FileName,bool isCombinedParams)
{
    saver->SaveData(CURRENT_DATA,B,HallEffect,MagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(CURRENT_DATA,B,HallEffect,MagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(FILTERED_DATA,FilteredB,FilteredHallEffect,FilteredMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(FILTERED_DATA,FilteredB,FilteredHallEffect,FilteredMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(EXTRAPOLATED_DATA,ExtrapolatedB,ExtrapolatedHallEffect,ExtrapolatedMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(EXTRAPOLATED_DATA,ExtrapolatedB,ExtrapolatedHallEffect,ExtrapolatedMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(ORIGINAL_DATA,OriginalB,OriginalHallEffect,OriginalMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(ORIGINAL_DATA,OriginalB,OriginalHallEffect,OriginalMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveSampleDescription(FileName);
}

//------------Подгонка данных-------------------------------------------------
//-----Функция не доработана.

void MagneticFieldDependence::averageData(DataTypeInContainer & inY, DataTypeInContainer &outY, FeatType featType)
{
    int size=inY.size();
    outY.resize(size/2);
    for(int i=0;i<size/2;i++)
    {
        switch(featType)
        {
        case ODD_FEAT: // нечетная подгонка
            outY[i]=(inY[i]-inY[size-1-i])/2.0;
            //outY[size-1-i]=-inY[i];
            break;
        case EVEN_FEAT: // четная подгонка
            outY[i]=(inY[i]+inY[size-1-i])/2.0;
            //tempY[size-1-i]=tempY[i];
            break;
        }
    }
    std::reverse(outY.begin(),outY.end());
}

void MagneticFieldDependence::featData(DataKind dataKind)
{
    DataTypeInContainer tempInX;
    DataTypeInContainer tempInHall;
    DataTypeInContainer tempInResistance;
    
    tempInX=(*getPointerB(dataKind));
    tempInHall=(*getPointerHall(dataKind));
    tempInResistance=(*getPointerMagnetoResistance(dataKind));
    
    if(tempInX.size()==0)
    {
        //"Получен пустой массив данных!!!");
        return;
    }
    
    averageData(tempInX,AveragedB,ODD_FEAT);
    for (DataTypeInContainer::iterator i = AveragedB.begin(); i != AveragedB.end(); ++i)
    {
        *i*=-1;    
    }

    averageData(tempInHall,AveragedHallEffect,ODD_FEAT);
    averageData(tempInResistance,AveragedMagnetoResistance,EVEN_FEAT);   
}


//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData()
{
    filterDataHelper((*filterParamsHall),HALL_EFFECT);
    filterDataHelper((*filterParamsResistance),MAGNETORESISTANCE);
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData(FilterParams &fPHall, FilterParams &fPResistance)
{
    setFilterParamsHall(fPHall.SamplingFrequecy, fPHall.BandwidthFrequency, fPHall.AttenuationFrequency, fPHall.filterLength);
    setFilterParamsResistance(fPResistance.SamplingFrequecy, fPResistance.BandwidthFrequency, fPResistance.AttenuationFrequency, fPResistance.filterLength);
    filterData();
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    PlotType dependenceType)
{
    if(HallEffect.size()!=B.size())
    {
        ShowMessage("Количество точек магнитного поля и эффекта Холла не совпадает! filterdataHelper");
        return;
    }
    unsigned int NumberOfPoints=HallEffect.size();
    DataTypeInContainer tempInB(2*NumberOfPoints);
    DataTypeInContainer tempInSignal(2*NumberOfPoints);

    DataTypeInContainer tempOutB(2*NumberOfPoints+ceil(fP.filterLength/2.0));
    DataTypeInContainer tempOutSignal(2*NumberOfPoints+ceil(fP.filterLength/2.0));



    switch(dependenceType)
    {
    case HALL_EFFECT:

    FilteredHallEffect.clear();
    // формируем сигнал для фильтра.
    // достраивая его в отрицательные магнитные поля.
    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
        tempInSignal[i]=-HallEffect[NumberOfPoints-i-1]+2*HallEffect[0];
        tempInB[i]=-B[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=HallEffect[i];
        tempInB[i+NumberOfPoints]=B[i];
    }
    break;
    case MAGNETORESISTANCE:

    FilteredB.clear();
    FilteredMagnetoResistance.clear();

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

//------------------------------------------------------------------------------
 // эта функция фильтрует как есть, не достраивая симметричную часть зависимости.
void MagneticFieldDependence::filterDataHelper2(FilterParams &fP,
    PlotType dependenceType)
{
    if(HallEffect.size()!=B.size())
    {
        ShowMessage("Количество точек магнитного поля и эффекта Холла не совпадает! filterdataHelper");
        return;
    }
    unsigned int NumberOfPoints=HallEffect.size();
    DataTypeInContainer tempInB(NumberOfPoints);
    DataTypeInContainer tempInSignal(NumberOfPoints);

    DataTypeInContainer tempOutB(NumberOfPoints+ceil(fP.filterLength/2.0));
    DataTypeInContainer tempOutSignal(NumberOfPoints+ceil(fP.filterLength/2.0));



    switch(dependenceType)
    {
    case HALL_EFFECT:

    FilteredHallEffect.clear();
    // формируем сигнал для фильтра.
        tempInSignal=HallEffect;
        tempInB=B;
    break;
    case MAGNETORESISTANCE:

    FilteredB.clear();
    FilteredMagnetoResistance.clear();

        tempInSignal=MagnetoResistance;
        tempInB=B;

    break;

    default:
        break;
    }
    // фильтруем 
    TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                fP.filterLength,fP.SamplingFrequecy,fP.BandwidthFrequency,fP.AttenuationFrequency);


    // нагло записываем положительную часть фильтрованного сигнала обратно.
    switch(dependenceType)
    {        
    case HALL_EFFECT:
        FilteredHallEffect=tempOutSignal;
        
        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance=tempOutSignal;
        FilteredB=tempOutB;
        break;
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

bool MagneticFieldDependence::extrapolateData(const int polinomPowForMagnetoResistance=4, const int polinomPowForHallEffect=4)
{
    bool returnValue=true;

    DataTypeInContainer koefMagnetoResistance(polinomPowForMagnetoResistance+1);
    DataTypeInContainer koefHallEffect(polinomPowForHallEffect+1);

    DataTypeInContainer newB;
    DataTypeInContainer newHallEffect;
    DataTypeInContainer newMagnetoResistance;

    DataTypeInContainer inBHall(FilteredB);
    DataTypeInContainer inBMagnetoResistance(FilteredB);
    DataTypeInContainer inHallEffect(FilteredHallEffect);
    DataTypeInContainer inMagnetoResistance(FilteredMagnetoResistance);
    unsigned int NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("Количество точек равно нулю! Я не хочу делить на ноль:)");
    return false;
    }
    MyDataType h=2.0/NumberOfPoints;
    
    unsigned int i=0;
	for(int i=0;i<500;i++)
	{
		inBHall.push_back(0);
		inHallEffect.push_back(0);
	}

    curveFittingUniversal(&inBMagnetoResistance,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance);
    curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,polinomPowForHallEffect);

    

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

	//----------А вот тут прикручиваем недостающий кусочек в сигналы----

	while(FilteredB[i++]<=0 && i<NumberOfPoints);
	i-=1; // ищем где поле становится положительным.

    for(unsigned int j=i;j<NumberOfPoints;j++)
	{      // перемещаем эти значения в начало.
		FilteredB[j-i]=FilteredB[j];
		FilteredMagnetoResistance[j-i]=FilteredMagnetoResistance[j];
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
void MagneticFieldDependence::multiplyB(DataKind dataKind)
{
    DataTypeInContainer * temp;
    temp=getPointerB(dataKind);

    DataTypeInContainer::iterator pos;
    for(pos=temp->begin();pos!=temp->end();++pos)
    *pos*=10;
}  
//-------------------------------------------------------------------------------
inline void MagneticFieldDependence::ReplaceDotsToComma(std::string &in, std::string & out)
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
//-------------------------------------------------------------------------------*/
void MagneticFieldDependence::constructPlotFromTwoMassive(PlotType pt, DataKind dk,TLineSeries* s,TColor color)
{
    DataTypeInContainer * pointToX=0;
    DataTypeInContainer * pointToY=0;
	s->Clear();
    switch(pt)
    {
    case HALL_EFFECT:
        pointToX=getPointerB(dk);
        pointToY=getPointerHall(dk);
        break;
    case MAGNETORESISTANCE:
        pointToX=getPointerB(dk);
        pointToY=getPointerMagnetoResistance(dk);
        break;
    default:
        break;
    }
    unsigned int NumberOfPoints=pointToX->size();
    if(NumberOfPoints==0)
    {
    ShowMessage("График пуст, строить нечего.");
    return;
    }
    DataTypeInContainer::iterator posX;
    DataTypeInContainer::iterator posY;
    for (posX=pointToX->begin(),posY=pointToY->begin();posX!=pointToX->end();++posX,++posY)
    	if(*posX <1000000 && *posY<1000000)
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
    unsigned int NumberOfPoints=temp->size();
	for (unsigned int i = 0; i < NumberOfPoints; i++)
	{
		s->AddY((*temp)[i],"",color);
	} 
}

//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsHall(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    filterParamsHall->setFilterParams(StrToFloat(samplingFrequecy), StrToFloat(bandwidthFrequency), StrToFloat(attenuationFrequency), StrToInt(length));
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsHall(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    filterParamsHall->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsResistance(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    filterParamsResistance->setFilterParams(StrToFloat(samplingFrequecy), StrToFloat(bandwidthFrequency), StrToFloat(attenuationFrequency), StrToInt(length));
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsResistance(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    filterParamsResistance->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
    return true;
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::setRoundNeeded(bool needRound)
{
    saver->setRoundNeeded(needRound);
}

//-------------------------------------------------------------------------------


void MagneticFieldDependence::setDependence(DataTypeInContainer::iterator beginB, 
        DataTypeInContainer::iterator endB, DataTypeInContainer::iterator beginHall, 
        DataTypeInContainer::iterator beginResistance)
{
    OriginalB.clear();
    OriginalHallEffect.clear();
    OriginalMagnetoResistance.clear();

    for ( ; beginB != endB; ++beginB, ++beginResistance, ++beginHall)
    {
        OriginalB.push_back(*beginB);
        OriginalMagnetoResistance.push_back(*beginResistance);
        OriginalHallEffect.push_back(*beginHall); 
    }

    B=OriginalB;

    BHall=OriginalB;
    BMagnetoResistance=OriginalB;

    HallEffect=OriginalHallEffect;
    MagnetoResistance=OriginalMagnetoResistance;

    filterData();
    extrapolateData(4,4); // магические числа, степени полиномов для экстраполяции по умолчанию.
    // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.      
}

//---------------------------------------------------------------------------------

void MagneticFieldDependence::getSplittedDataFromADC()
{
    TwoDimensionalContainer * tempData1=adc->getSplittedData(1);

    TwoDimensionalContainer & tempData(*tempData1);
    int t=tempData[2].size();
    t=tempData[1].size();
    t=tempData[0].size();
    //if(tempData.size()>3) // если это не тестовые замерки
    {
        OriginalB=tempData[2];
        OriginalHallEffect=tempData[0]; // последовательность закреплена и не важна.
        OriginalMagnetoResistance=tempData[1];
        // при смене каналов на вкладке настроек - эти настройки можно не трогать.
        // программа сама разберется, т.к. АЦП возвращает данные согласно контрольной таблице:).
        
        B=OriginalB;

        BHall=OriginalB;
        BMagnetoResistance=OriginalB;

        HallEffect=OriginalHallEffect;
        MagnetoResistance=OriginalMagnetoResistance;

        multiplyB(CURRENT_DATA);
        filterData();
        extrapolateData(4,4); // магические числа, степени полиномов для экстраполяции по умолчанию.
        // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.
    }
}

void MagneticFieldDependence::setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber,
    AnsiString length, AnsiString width, AnsiString Thickness)
{
    saver->setSampleDescription(Temperature, Current, SampleInventoryNumber, length, width, Thickness);
}
//-------------------------------------------------------------------------------

FilterParams const * MagneticFieldDependence::getFilterParamsHall()
{
    return filterParamsHall;
}
//------------------------------------------------------------------------------
FilterParams const * MagneticFieldDependence::getFilterParamsResistance()
{
    return filterParamsResistance;
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
DataTypeInContainer const & MagneticFieldDependence::getSxx()
{
    return sxx;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getSxy()
{
    return sxy;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getAveragedB()
{
    return AveragedB;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getRh_eff()
{
    return Rh_eff;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const & MagneticFieldDependence::getS_eff()
{
    return s_eff;
}
//----------------------------------------------------------------------------------


void MagneticFieldDependence::calculateEffectiveParamsFromSignals()
{
    if (AveragedB.size() != 0)
    {
        AnsiString Current;
        AnsiString SampleLength;
        AnsiString SampleWidth;
        AnsiString SampleThickness;

        saver->getSampleDescription(NULL, Current, NULL, SampleLength, SampleWidth, SampleThickness);

        MyDataType I=StrToFloat(Current)*1E-6;
        MyDataType SLength=StrToFloat(SampleLength)*1E-3;
        MyDataType SWidth=StrToFloat(SampleWidth)*1E-3;
        MyDataType SThickness=StrToFloat(SampleThickness)*1E-6;

        unsigned int NumberOfPoints = AveragedB.size();
        s_eff.resize(NumberOfPoints);
        Rh_eff.resize(NumberOfPoints);

        for (int i = 0; i < NumberOfPoints ; i++)
        {
            if(fabs(AveragedMagnetoResistance[i])<THEALMOSTZERO)
                s_eff[i]=0;
            else
            {
                s_eff[i]=SLength/SWidth/SThickness*I/AveragedMagnetoResistance[i];
            }
            if(B[i]==0)
                Rh_eff[i]=0;
            else
                Rh_eff[i]=SThickness*AveragedHallEffect[i]/I;
        }
    }
}

//--------------------------------------------------------------------------------

void MagneticFieldDependence::calculateTenzorFromEffectiveParams()
{
    if (s_eff.size() != 0)
    {
        unsigned int NumberOfPoints = s_eff.size();
        sxx.resize(NumberOfPoints);
        sxy.resize(NumberOfPoints);

        for (int i = 0; i < NumberOfPoints ; i++)
        {
            sxx[i]=s_eff[i]/
                (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1.0);
            sxy[i]=s_eff[i]*s_eff[i]*Rh_eff[i]/
               (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1.0);
        }
        
    }
}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerB(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &B;
        break;
    case FILTERED_DATA:
        return &FilteredB;
        break;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedB;
        break;
    case ORIGINAL_DATA:
        return &OriginalB;
        break;
    case AVERAGED_DATA:
        return &AveragedB;
        break;
    default:
        break;
    }
}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerHall(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &HallEffect;
        break;
    case FILTERED_DATA:
        return &FilteredHallEffect;
        break;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedHallEffect;
        break;
    case ORIGINAL_DATA:
        return &OriginalHallEffect;
        break;
    case AVERAGED_DATA:
        return &AveragedHallEffect;
        break;
    default:
        break;
    }

}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerMagnetoResistance(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &MagnetoResistance;
        break;
    case FILTERED_DATA:
        return &FilteredMagnetoResistance;
        break;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedMagnetoResistance;
        break;
    case ORIGINAL_DATA:
        return &OriginalMagnetoResistance;
        break;
    case AVERAGED_DATA:
        return &AveragedMagnetoResistance;
        break;
    default:
        break;
    }
}
//-------------------------------------------------------------------------

void MagneticFieldDependence::calcutaleTenzor(DataKind dataKind)
{
    featData(dataKind);
    cutData(AVERAGED_DATA);
    calculateEffectiveParamsFromSignals();
    calculateTenzorFromEffectiveParams();
}

//-------------------------------------------------------------------------
void  MagneticFieldDependence::cutData(DataKind dataKind)
{
   ;
}