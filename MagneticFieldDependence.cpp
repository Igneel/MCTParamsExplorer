#include "MagneticFieldDependence.h"
#include "Unit1.h"

MagneticFieldDependence::MagneticFieldDependence()
{
    Current=0.001;
    h=0.001;
    NumberOfPoints=10;
    filterParams=new FilterParams();
}

MagneticFieldDependence::MagneticFieldDependence(MyDataType current)
{
    MagneticFieldDependence();
    Current=current;
}

MagneticFieldDependence::~MagneticFieldDependence()
{
    delete filterParams;
}

void MagneticFieldDependence::SaveData(DataKind dataKind,SaveType saveType,std::string FileName)
{
switch(saveType)
{
    case ALL_POINTS:
        break;
    case SOME_POINTS:
        break;
    default:
        break;
}

}


void MagneticFieldDependence::featData(DataKind dataKind, long index, FeatType featType)
{
    std::vector<MyDataType> tempX;
    std::vector<MyDataType> tempY;
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

void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    DependenceType dependenceType)
{
    std::vector<long double> tempInB(2*NumberOfPoints);
    std::vector<long double> tempInSignal(2*NumberOfPoints);

    std::vector<long double> tempOutB(2*NumberOfPoints+ceil(fP.filterLength/2.0));
    std::vector<long double> tempOutSignal(2*NumberOfPoints+ceil(fP.filterLength/2.0));

    switch(dependenceType)
    {
    case HALL_EFFECT:
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
        FilteredB.push_back(tempOutB[i+NumberOfPoints-1]);
    case HALL_EFFECT:
		FilteredHallEffect.push_back(tempOutSignal[i+NumberOfPoints-1]);
        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance.push_back(tempOutSignal[i+NumberOfPoints-1]);
        break;
    default:
        break;
    }
    }
}

void MagneticFieldDependence::filterData(FilterParams &fP)
{
    filterDataHelper(fP,HALL_EFFECT);
    filterDataHelper(fP,MAGNETORESISTANCE);
}

bool MagneticFieldDependence::extrapolateData()
{
bool returnValue=true;

const int polinomPowForMagnetoResistance=4;
const int polinomPowForHallEffect=4;

std::vector<long double> koefMagnetoResistance(polinomPowForMagnetoResistance+1);
std::vector<long double> koefHallEffect(polinomPowForHallEffect+1);

std::vector<long double> newB;
std::vector<long double> newHallEffect;
std::vector<long double> newMagnetoResistance;

std::vector<long double> inB(FilteredB);
std::vector<long double> inHallEffect(FilteredHallEffect);
std::vector<long double> inMagnetoResistance(FilteredMagnetoResistance);

h=2.0/NumberOfPoints;
unsigned int i=0;
        if(dependenceType==HALL_EFFECT)
			for(int i=0;i<500;i++)
			{
				inB.push_back(0);
				inHallEffect.push_back(0);
			}

        if(dependenceType==HALL_EFFECT)
            curveFittingUniversal(&inB,&inHallEffect, &koefHallEffect,polinomPowForHallEffect);
        if(dependenceType==MAGNETORESISTANCE)
            curveFittingUniversal(&inB,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance);

			newB.clear();
			newB.push_back(0);
			for (unsigned int i = 1; i < NumberOfPoints; i++) {
				newB.push_back(newB[i-1]+h);
			}
        if(dependenceType==MAGNETORESISTANCE)
			calculatePolinomByKoef(newB,koefMagnetoResistance,newMagnetoResistance);
        if(dependenceType==HALL_EFFECT)
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

void MagneticFieldDependence::averagingData()
{
;
}

void MagneticFieldDependence::multiplyB(DataKind dataKind)
{
std::vector<MyDataType> * temp;

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

std::vector<MyDataType>::iterator pos;
for(pos=temp->begin();pos!=temp->end();++pos)
*pos*=10;
}  

inline void MagneticFieldDependence::ReplaceDotsToComma(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich=","; // на что меняем
	std::string strToSearch=".";   // что ищем
	findIndex=s.find(strToSearch,strToSearch.length());
	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
    strToReplaceWhich.begin(),strToReplaceWhich.end());
    out=s;
}

inline void MagneticFieldDependence::ReplaceCommaToDots(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich="."; // на что меняем
	std::string strToSearch=",";   // что ищем
	findIndex=s.find(strToSearch,strToSearch.length());
	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
    strToReplaceWhich.begin(),strToReplaceWhich.end());
    out=s;
}

void MagneticFieldDependence::constructPlotFromTwoMassive(DataKind p,TLineSeries* s,TColor color)
{
    std::vector<MyDataType> * pointToX=0;
    std::vector<MyDataType> * pointToY=0;
	s->Clear();
    switch(p)
    {
    case CURRENT_DATA:
        pointToX=&B;
        pointToY=&HallEffect;
        pointToY=&MagnetoResistance;
        break;
    case FILTERED_DATA:
        pointToX=&FilteredB;
        pointToY=&FilteredHallEffect;
        pointToY=&FilteredMagnetoResistance;
        break;
    case EXTRAPOLATED_DATA:
        pointToX=&ExtrapolatedB;
        pointToY=&ExtrapolatedHallEffect;
        pointToY=&ExtrapolatedMagnetoResistance;
        break;
    case ORIGINAL_DATA:
        pointToX=&OriginalB;
        pointToY=&OriginalHallEffect;
        pointToY=&OriginalMagnetoResistance;
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
    std::vector<MyDataType>::iterator posX;
    std::vector<MyDataType>::iterator posY;
    for (posX=pointToX->begin(),posY=pointToY->begin();posX!=pointToX->end();++posX,++posY)
    	s->AddXY(*posX,*posY,"",color);
	
}
void MagneticFieldDependence::constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color)
{
    std::vector<MyDataType> * temp;
	s->Clear();
    switch(p)
    {
    case MAGNETIC_FIELD:
        temp=&B;
        break;
    case DEPENDENCE:
        temp=&HallEffect;
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

/*
std::vector<MyDataType> const &  MagneticFieldDependence::getDataFromADC()
{
    std::vector<MyDataType> tempData;
    tempData.resize(adc->getData().size());
    tempData=adc->getData();
    // тут дальше парсинг, в зависимости от структуры данных.
    // и вставить всё как надо в массивы B и Dependence

    // а также в массивы Original
    OriginalB.resize(tempData.size());
    OriginalB=tempData;
    OriginalDependence.resize(tempData.size());
    OriginalDependence=tempData;
    //после чего:
    //filterData(dependenceType,400000,50,100,50);
    //extrapolateData();
    return OriginalB;
}   */

void MagneticFieldDependence::getSplittedDataFromADC()
{
    std::vector<std::vector<MyDataType> > tempData(adc->getSplittedData());

    //if(tempData.size()>3) // если это не тестовые замерки
    {
        OriginalB=tempData[2];
        OriginalHallEffect=tempData[0];
        OriginalMagnetoResistance=tempData[1];
        
        B=OriginalB;
        HallEffect=OriginalHallEffect;
        MagnetoResistance=OriginalMagnetoResistance;

        multiplyB(CURRENT_DATA);
        filterData(*filterParams);
        extrapolateData();
    }
}

bool MagneticFieldDependence::setFilterParams(FilterParams & fp)
{
    filterParams->SamplingFrequecy=fp.SamplingFrequecy;
    filterParams->BandwidthFrequency=fp.BandwidthFrequency;
    filterParams->AttenuationFrequency=fp.AttenuationFrequency;
    filterParams->filterLength=fp.filterLength;
    return true;
}

FilterParams & MagneticFieldDependence::getFilterParams()
{
    return *filterParams;
}

std::vector<MyDataType> const & MagneticFieldDependence::getB()
{
    return B;
}

std::vector<MyDataType> const & MagneticFieldDependence::getOriginalB()
{
    return OriginalB;
}

std::vector<MyDataType> const & MagneticFieldDependence::getFilteredB()
{
    return FilteredB;
}

std::vector<MyDataType> const & MagneticFieldDependence::getExtrapolatedB()
{
    return ExtrapolatedB;
}

std::vector<MyDataType> const & MagneticFieldDependence::getHallEffect()
{
    return HallEffect;
}

std::vector<MyDataType> const & MagneticFieldDependence::getMagnetoResistance()
{
    return MagnetoResistance;
}

std::vector<MyDataType> const & MagneticFieldDependence::getOriginalHallEffect()
{
    return OriginalHallEffect;
}

std::vector<MyDataType> const & MagneticFieldDependence::getOriginalMagnetoResistance()
{
    return OriginalMagnetoResistance;
}

std::vector<MyDataType> const & MagneticFieldDependence::getFilteredHallEffect()
{
    return FilteredHallEffect;
}

std::vector<MyDataType> const & MagneticFieldDependence::getFilteredMagnetoResistance()
{
    return FilteredMagnetoResistance;
}

std::vector<MyDataType> const & MagneticFieldDependence::getExtrapolatedHallEffect()
{
    return ExtrapolatedHallEffect;
}

std::vector<MyDataType> const & MagneticFieldDependence::getExtrapolatedMagnetoResistance()
{
    return ExtrapolatedMagnetoResistance;
}
