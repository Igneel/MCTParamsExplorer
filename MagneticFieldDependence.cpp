#include "MagneticFieldDependence.h"
extern LCardADC *adc;


MagneticFieldDependence::MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
    AnsiString length, AnsiString width, AnsiString Thickness)

{
    filterParamsHall=new FilterParams(); // по идее нужно бы и инциализировать их тут, дабы не было проблем в случае чего:).
    filterParamsResistance=new FilterParams();
    saver =new DataSaver(temperature,current,SampleInventoryNumber, length, width, Thickness);
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

void MagneticFieldDependence::loadSampleDescription(TStringList *Names,TStringList *Values,AnsiString FileName)
{
    std::string temp = FileName.c_str();

    unsigned int rBound = temp.find_last_of("/\\");
    unsigned int rBound2 = temp.find_first_of("DCR",rBound);
    AnsiString NewFileName = FileName.SubString(0,rBound2)+"Description"+".txt";
    TStringList * tts = new TStringList();
    if(FileExists(NewFileName))
    {
    tts->LoadFromFile(NewFileName);
    const std::string delimiter = "\t";

    std::string s1 = tts->Text.c_str();
    std::string s;
    ReplaceDotsToComma(s1,s);
    
    tts->Text = s.c_str();

    Names->Clear();
    Values->Clear();

    for (int i=0;i<tts->Count;i++)
    {
        s=tts->Strings[i].c_str();
        std::string::size_type start = 0;
        std::string::size_type finish = 0;

        if(s.empty())
            continue;

        finish = s.find(delimiter, start);
        Names->Add(s.substr(start, finish-start).c_str());
        Values->Add(s.substr(finish-start+1, s.length()).c_str());
    }

    setSampleDescription(Values->Strings[1],Values->Strings[2],Values->Strings[0],
    Values->Strings[3],Values->Strings[4],Values->Strings[5]);
    }
    delete tts;
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
// ахтунг, это страшный костыль -
// там где эта функция вызывается задаются имена файлов
// Com Dir и Rev
// по ним я и определяю тип сигнала.!!!!
ParamsType pt;

if(FileName[FileName.Length()-1]=='m')  pt=COMBINE;
if(FileName[FileName.Length()-1]=='v')  pt=REVERSE;
if(FileName[FileName.Length()-1]=='r')  pt=DIRECT;

saver->setParamsType(pt);
    saver->SaveData(CURRENT_DATA,&B,&HallEffect,&MagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(CURRENT_DATA,&B,&HallEffect,&MagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(FILTERED_DATA,&FilteredB,&FilteredHallEffect,&FilteredMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(FILTERED_DATA,&FilteredB,&FilteredHallEffect,&FilteredMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(EXTRAPOLATED_DATA,&ExtrapolatedB,&ExtrapolatedHallEffect,&ExtrapolatedMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(EXTRAPOLATED_DATA,&ExtrapolatedB,&ExtrapolatedHallEffect,&ExtrapolatedMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveSampleDescription(FileName);
}

//------------Подгонка данных-------------------------------------------------
template <class T>
int sigh(T in)
{
    if (in>=0)
    {
        return 1;
    }
    else 
        return -1;
}

void MagneticFieldDependence::averageData(DataTypeInContainer & inY, DataTypeInContainer &outY, FeatType featType)
{
    /*
    усредненный сигнал на данный момент существует лишь для положительного магнитного поля.
    */
    int size=inY.size();
    outY.resize(size/2);
    for(int i=0;i<size/2;i++)
    {
        switch(featType)
        {
        case ODD_FEAT: // нечетная подгонка
            outY[i]=sigh(inY[size-1-i])*fabs((inY[i]-inY[size-1-i]))/2.0;
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
    averageData(tempInHall,AveragedHallEffect,ODD_FEAT);
    averageData(tempInResistance,AveragedMagnetoResistance,EVEN_FEAT);
}

void MagneticFieldDependence::GetEqualNumberOfPoints(DataTypeInContainer & B,
DataTypeInContainer & BHall,DataTypeInContainer & BRes, DataTypeInContainer & Hall,
DataTypeInContainer & Res)
{
    MyDataType left, right;
    size_t length;

    DataTypeInContainer tempB;
    DataTypeInContainer tempHall;
    DataTypeInContainer tempResistance;
    if(Hall.size()==0 || Res.size()==0 || B.size()==0)
    {
    return;
    }
    // ---------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (Hall.size()>Res.size())
    {
        left=Res[0];
        right=Res.back();
        length=Res.size();
    }
    else
    {
        left=Hall[0];
        right=Hall.back();
        length=Hall.size();
    }
    // это должно уравнивать количество точек, после фильтрации с разной длиной.
    thiningSignal(BHall, Hall, tempB, tempHall, left, right, length);
    thiningSignal(BRes, Res, tempB, tempResistance, left, right, length);
    
    BHall.clear();
    Hall.clear();
    BRes.clear();
    Res.clear();
    B.clear();
    
    BHall=tempB;
    Hall=tempHall;
    BRes=tempB;
    Res=tempResistance;
    B=BHall;
}
//-------------------------------------------------------------------------
void MagneticFieldDependence::blockfilterData()
{
    /*
    Блочный фильтр.
    Весь сигнал разбивается на блоки и фильтруется отдельно.
    После чего объединяется.
    На данный момент работает не очень хорошо.
    */
clearFilteredParams();

    GetEqualNumberOfPoints(B,BHall,BMagnetoResistance,
    HallEffect,MagnetoResistance);

    BlockLowBandFilter(BHall,HallEffect,FilteredBHall,FilteredHallEffect,
        (*filterParamsHall).filterLength,
        (*filterParamsHall).SamplingFrequecy,
        (*filterParamsHall).BandwidthFrequency,
        (*filterParamsHall).AttenuationFrequency,
        200);
    BlockLowBandFilter(BMagnetoResistance,MagnetoResistance,
         FilteredBMagnetoResistance,FilteredMagnetoResistance,
        (*filterParamsResistance).filterLength,
        (*filterParamsResistance).SamplingFrequecy,
        (*filterParamsResistance).BandwidthFrequency,
        (*filterParamsResistance).AttenuationFrequency,
        200);

    FilteredB=FilteredBHall;
    GetEqualNumberOfPoints(FilteredB,FilteredBHall,FilteredBMagnetoResistance,
    FilteredHallEffect,FilteredMagnetoResistance);
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData()
{
    // очищаем результаты предыдущей фильтрации
    clearFilteredParams();
    // уравниваем количество точек
    GetEqualNumberOfPoints(B,BHall,BMagnetoResistance,
    HallEffect,MagnetoResistance);

    filterDataHelper((*filterParamsHall),HALL_EFFECT);
    filterDataHelper((*filterParamsResistance),MAGNETORESISTANCE);
    FilteredB=FilteredBHall;
    GetEqualNumberOfPoints(FilteredB,FilteredBHall,FilteredBMagnetoResistance,
    FilteredHallEffect,FilteredMagnetoResistance);
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData(FilterParams &fPHall, FilterParams &fPResistance)
{
    /*
    Вызывает предыдущую, после установки новых параметров фильтрации.
    */
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

    DataTypeInContainer * inB;
    DataTypeInContainer * inHallEffect;
    DataTypeInContainer * inMagnetoResistance;
    unsigned int NumberOfPoints;

    if ((B[0]+2.0)<0.5) // если это комбинированный сигнал.
    {
        featData(CURRENT_DATA); // его надо усреднить
        inB=&AveragedB; // фильтровать будем усредненный сигнал
        inHallEffect=&AveragedHallEffect;
        inMagnetoResistance=&AveragedMagnetoResistance;
        NumberOfPoints=AveragedB.size();
    }
    else // если это обычный сигнал - фильтруем как есть.
    {
        inB=&B;
        inHallEffect=&HallEffect;
        inMagnetoResistance=&MagnetoResistance;
        NumberOfPoints=HallEffect.size();
    }

    // В эти массивы будут достраиваться данные для отрицательных магнитных полей.
    DataTypeInContainer tempInB(2*NumberOfPoints);
    DataTypeInContainer tempInSignal(2*NumberOfPoints);

    DataTypeInContainer tempOutB(2*NumberOfPoints);
    DataTypeInContainer tempOutSignal(2*NumberOfPoints);

    switch(dependenceType)
    {
    case HALL_EFFECT:


    // формируем сигнал для фильтра.
    // достраивая его в отрицательные магнитные поля.
    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
        tempInSignal[i]=-(*inHallEffect)[NumberOfPoints-i-1]+2*(*inHallEffect)[0];
        tempInB[i]=-(*inB)[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=(*inHallEffect)[i];
        tempInB[i+NumberOfPoints]=(*inB)[i];
    }
    break;
    case MAGNETORESISTANCE:

    

    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
        tempInSignal[i]=(*inMagnetoResistance)[NumberOfPoints-i-1];   // чет
        //tempInSignal[i]=-Dependence[NumberOfPoints-i-1]+2*Dependence[0];  // нечет
        tempInB[i]=-(*inB)[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=(*inMagnetoResistance)[i];
        tempInB[i+NumberOfPoints]=(*inB)[i];
    }
    break;

    default:
        break;
    }

    // фильтруем 
    TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                fP.filterLength,fP.SamplingFrequecy,fP.BandwidthFrequency,fP.AttenuationFrequency);

    
    // Размер внутри фильтра меняется, в зависимости от длины фильтра.
    NumberOfPoints=tempOutB.size();
    for(unsigned int i=fP.filterLength;i<NumberOfPoints;i++)
    {
        
    switch(dependenceType)
    {
    case HALL_EFFECT:
        FilteredHallEffect.push_back(tempOutSignal[i]);
        FilteredBHall.push_back(tempOutB[i]);
        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance.push_back(tempOutSignal[i]);
        FilteredBMagnetoResistance.push_back(tempOutB[i]);
        break;
    default:
        break;
    }
    }

    
/*
    unsigned int i=0;

    while(i<NumberOfPoints && tempOutB[i]<=0 ) ++i; // ищем где поле становится положительным.

    // нагло записываем положительную часть фильтрованного сигнала обратно.
    for(;i<NumberOfPoints;i++)
    {

    switch(dependenceType)
    {
    case HALL_EFFECT:
        FilteredHallEffect.push_back(tempOutSignal[i]);

        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance.push_back(tempOutSignal[i]);
        FilteredB.push_back(tempOutB[i]);
        break;
    default:
        break;
    }
    } */


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

    // копируем фильтрованные данные
    /*DataTypeInContainer inBHall(FilteredB);
    DataTypeInContainer inBMagnetoResistance(FilteredB);

    DataTypeInContainer inHallEffect(FilteredHallEffect);
    DataTypeInContainer inMagnetoResistance(FilteredMagnetoResistance);
*/
    DataTypeInContainer inBHall;
    DataTypeInContainer inBMagnetoResistance;

    DataTypeInContainer inHallEffect;
    DataTypeInContainer inMagnetoResistance;

    /*
    Экстраполяция данных ведется по последней четверти фильтрованных значений.
    */
    for (size_t i = 3*FilteredB.size()/4; i < FilteredB.size(); ++i)
    {
        inBHall.push_back( FilteredB[i]);
        inBMagnetoResistance.push_back( FilteredB[i]);
        inHallEffect.push_back( FilteredHallEffect[i]);
        inMagnetoResistance.push_back( FilteredMagnetoResistance[i]);
    }
/*
    for (unsigned int i=B.size()-filterParamsHall->filterLength; i<B.size();++i)
    { // дописываем измеренные данные
        inHallEffect.push_back(HallEffect[i]);
        inMagnetoResistance.push_back(MagnetoResistance[i]);
        inBHall.push_back(B[i]);
        inBMagnetoResistance.push_back(B[i]);
    }
*/   
    unsigned int NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("Количество точек равно нулю! Я не хочу делить на ноль:)");
    return false;
    }
    MyDataType h=2.2/NumberOfPoints;
    

	for(int i=0;i<500;i++) // увеличиваем вес точки (0,0) для эффекта Холла.
	{
		inBHall.push_back(0);
		inHallEffect.push_back(0);
	}

    
    if(!curveFittingUniversal(&inBMagnetoResistance,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance))
    return false;
    if(!curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,polinomPowForHallEffect))
    return false;

    

	newB.clear();
	newB.push_back(0); // заполняем магнитное поле.
	for (unsigned int i = 1; i < NumberOfPoints; i++) {
		newB.push_back(newB[i-1]+h);
	}

    // вычисляем экстраполированные зависимости.
	calculatePolinomByKoef(newB,koefMagnetoResistance,newMagnetoResistance);
	calculatePolinomByKoef(newB,koefHallEffect,newHallEffect);

    ExtrapolatedB.clear();
    ExtrapolatedMagnetoResistance.clear();
    ExtrapolatedHallEffect.clear();

    ExtrapolatedB=newB;    
    ExtrapolatedMagnetoResistance=newMagnetoResistance;
    ExtrapolatedHallEffect=newHallEffect;
    
	//----------А вот тут прикручиваем недостающий кусочек в сигналы----
    unsigned int i=0;
	while(i<NumberOfPoints && newB[i]<FilteredB.back())
    ++i;

   	for(unsigned int j=i;j<NumberOfPoints;j++)
	{     // в конце дописываем экстраполированные значения.
		FilteredB.push_back(newB[j]);
        FilteredMagnetoResistance.push_back(newMagnetoResistance[j]);
        FilteredHallEffect.push_back(newHallEffect[j]);
	}
	//------------------------------------------------------------------
       
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
void MagneticFieldDependence::multiplyB(DataTypeInContainer::iterator beginB, DataTypeInContainer::iterator endB)
{    
    for(;beginB!=endB;++beginB)
    *beginB*=10;
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
    DataTypeInContainer * pointToX=0; // указатели на выводимые данные
    DataTypeInContainer * pointToY=0;
	s->Clear(); // чистим график.
    switch(pt) // определяем что выводить.
    {
    case HALL_EFFECT:
        pointToX=getPointerB(dk);
        pointToY=getPointerHall(dk);
        break;
    case MAGNETORESISTANCE:
        pointToX=getPointerB(dk);
        pointToY=getPointerMagnetoResistance(dk);
        break;
    case SXX:
        pointToX=getPointerB(dk);
        pointToY=getPointerSxx(dk);
        break;

    case SXY:
        pointToX=getPointerB(dk);
        pointToY=getPointerSxy(dk);
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
    long double PointsToShow=500;
    unsigned int shag = roundM(static_cast<long double> (pointToX->size()) / PointsToShow,0);
    if(shag<1) shag = 1;
    for (unsigned int i=0, j=0;j<PointsToShow && i<pointToX->size() && i<pointToY->size();j++,i+=shag)
    {
        s->AddXY((*pointToX)[i],(*pointToY)[i],"",color);
    }	
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
    case MAGNETIC_FIELD_F:
        temp=&FilteredB;
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
    clearCurrentParams();

    for ( ; beginB != endB; ++beginB, ++beginResistance, ++beginHall)
    {
        B.push_back(*beginB);
        MagnetoResistance.push_back(*beginResistance);
        HallEffect.push_back(*beginHall); 
    }
    if(max_elem(B)<0.5)
    {
        multiplyB(CURRENT_DATA);
    }
    BHall=B;
    BMagnetoResistance=B;


    filterData();
    extrapolateData(4,4); // магические числа, степени полиномов для экстраполяции по умолчанию.
    // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.      
}

//---------------------------------------------------------------------------------

void MagneticFieldDependence::getSplittedDataFromADC()
{
    TwoDimensionalContainer * tempData1=adc->getSplittedData(1);

    TwoDimensionalContainer & tempData(*tempData1);
    clearCurrentParams();
             
    B=tempData[chanInfo[2].first];
    HallEffect=tempData[chanInfo[0].first]; // последовательность закреплена и не важна.
    MagnetoResistance=tempData[chanInfo[1].first];

    adc->dataisntNeeded();
    // при смене каналов на вкладке настроек - эти настройки можно не трогать.
    // программа сама разберется, т.к. АЦП возвращает данные согласно контрольной таблице:).
    
    BHall=B;
    BMagnetoResistance=B;

    multiplyB(CURRENT_DATA);
    filterData();
    //extrapolateData(4,4); // магические числа, степени полиномов для экстраполяции по умолчанию.
    // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.
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
DataTypeInContainer const * MagneticFieldDependence::getB()
{
    return &B;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getFilteredB()
{
    return &FilteredB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getExtrapolatedB()
{
    return &ExtrapolatedB;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getHallEffect()
{
    return &HallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getMagnetoResistance()
{
    return &MagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getFilteredHallEffect()
{
    return &FilteredHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getFilteredMagnetoResistance()
{
    return &FilteredMagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getExtrapolatedHallEffect()
{
    return &ExtrapolatedHallEffect;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getExtrapolatedMagnetoResistance()
{
    return &ExtrapolatedMagnetoResistance;
}
//-------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getSxx()
{
    return &sxx;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getSxy()
{
    return &sxy;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getAveragedB()
{
    return &AveragedB;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getRh_eff()
{
    return &Rh_eff;
}
//----------------------------------------------------------------------------------
DataTypeInContainer const * MagneticFieldDependence::getS_eff()
{
    return &s_eff;
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

        for (unsigned int i = 0; i < NumberOfPoints ; i++)
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

        for (unsigned int i = 0; i < NumberOfPoints ; i++)
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
    case FILTERED_DATA:
        return &FilteredB;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedB;
    case AVERAGED_DATA:
        return &AveragedB;
    default:
        break;
    }
    return NULL;
}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerHall(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &HallEffect;
    case FILTERED_DATA:
        return &FilteredHallEffect;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedHallEffect;
    case AVERAGED_DATA:
        return &AveragedHallEffect;
    default:
        break;
    }
    return NULL;
}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerMagnetoResistance(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &MagnetoResistance;
    case FILTERED_DATA:
        return &FilteredMagnetoResistance;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedMagnetoResistance;
    case AVERAGED_DATA:
        return &AveragedMagnetoResistance;
    default:
        break;
    }
    return NULL;
}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerSxx(DataKind dataKind)
{
    return &sxx;
}
//-------------------------------------------------------------------------
DataTypeInContainer * MagneticFieldDependence::getPointerSxy(DataKind dataKind)
{
    return &sxy;
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

void MagneticFieldDependence::clearCurrentParams()
{
    B.clear();
    BHall.clear();
    BMagnetoResistance.clear();
    HallEffect.clear();
    MagnetoResistance.clear();    
}

void MagneticFieldDependence::clearFilteredParams()
{
    FilteredB.clear();
    FilteredBHall.clear();
    FilteredBMagnetoResistance.clear();
    FilteredHallEffect.clear();
    FilteredMagnetoResistance.clear();
}

void MagneticFieldDependence::setParamsType(ParamsType pt)
{
    paramsType=pt;
}

void MagneticFieldDependence::shiftCurve(DataKind dataKind,PlotType dependenceType,MyDataType shiftValue,MyDataType leftBound, MyDataType rightBound)
{
    DataTypeInContainer * pointToY=0;
    DataTypeInContainer * pointToX=0;
    switch (dependenceType)
    {
        case HALL_EFFECT:
            pointToY=getPointerHall(dataKind);
            pointToX=getPointerB(dataKind);
            break;
        case MAGNETORESISTANCE:
            pointToY=getPointerMagnetoResistance(dataKind);
            pointToX=getPointerB(dataKind);
            break;
        default:
        break;
    }
    if (pointToY==0)
    {
        return;
    }

    for (DataTypeInContainer::iterator i = pointToY->begin(), j=pointToX->begin(); i != pointToY->end() && j!=pointToX->end(); ++i,++j)
    {
        if (*j>=leftBound && *j<=rightBound)
        {
            *i+=shiftValue;    
        }        
    }
}

void MagneticFieldDependence::setChannelsInfo(channelsInfo & cI)
{
    chanInfo=cI;    
}