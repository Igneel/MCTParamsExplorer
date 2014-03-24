#include "MagneticFieldDependence.h"
#include "Unit1.h"

MagneticFieldDependence::MagneticFieldDependence(DependenceType d)
{
    dependenceType=d;
    Current=0.001;
    h=0.001;
    NumberOfPoints=10;
    filterParams=new FilterParams();
}

MagneticFieldDependence::MagneticFieldDependence(DependenceType dt, MyDataType current, MyDataType step)
{
    dependenceType=dt;
    Current=current;
    h=step;
    NumberOfPoints=10;
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
    }


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
        tempX.swap(tempY);
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

void MagneticFieldDependence::filterData(FilterParams &fP)
{
    MyDataType SamplingFrequecy=fP.SamplingFrequecy;
    MyDataType BandwidthFrequency=fP.BandwidthFrequency;
    MyDataType AttenuationFrequency=fP.AttenuationFrequency;
    int lengthFilter=fP.filterLength;

    NumberOfPoints=Dependence.size();

    std::vector<long double> tempInB(2*NumberOfPoints);
    std::vector<long double> tempInSignal(2*NumberOfPoints);

    std::vector<long double> tempOutB(2*NumberOfPoints+ceil(lengthFilter/2.0));
    std::vector<long double> tempOutSignal(2*NumberOfPoints+ceil(lengthFilter/2.0));

    switch(dependenceType)
    {
    case HALL_EFFECT:
    // формируем сигнал дл€ фильтра.
	// достраива€ его в отрицательные магнитные пол€.
	for (unsigned int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=-Dependence[NumberOfPoints-i-1]+2*Dependence[0];
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=Dependence[i];
		tempInB[i+NumberOfPoints]=B[i];
	}
    break;
    case MAGNETORESISTANCE:

    for (unsigned int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=Dependence[NumberOfPoints-i-1];   // чет
		//tempInSignal[i]=-Dependence[NumberOfPoints-i-1]+2*Dependence[0];  // нечет
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=Dependence[i];
		tempInB[i+NumberOfPoints]=B[i];
	}
    break;
    }

    // фильтруем 
	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                lengthFilter,SamplingFrequecy,BandwidthFrequency,AttenuationFrequency);


    // нагло записываем положительную часть фильтрованного сигнала обратно.
    //FilteredB.resize(NumberOfPoints);
    //FilteredDependence.resize(NumberOfPoints);

	for(unsigned int i=0;i<NumberOfPoints;i++)
	{
        FilteredB.push_back(tempOutB[i+NumberOfPoints-1]);
		FilteredDependence.push_back(tempOutSignal[i+NumberOfPoints-1]);
    }
    /*
    switch(dependenceType)
    {
    case HALL_EFFECT:
    // убираем отрицательные значени€ магнитного пол€, дл€ эффекта ’олла.
    // —тавим его в точку 0.
    // хот€ зачем...€ же добавл€ю это при экстрапол€ции...
    for(int i=0;i<NumberOfPoints;i++)
	{
		if(B[i]<0)
		{
			B[i]=0;
			//Us[i]=0;
			Uy[i]=0;
		}
	}
    break;
    case MAGNETORESISTANCE:

    break;
    }   */
}

bool MagneticFieldDependence::extrapolateData()
{
bool returnValue=true;

const int polinomPowForUs=4;
const int polinomPowForUy=4;

std::vector<long double> koefUs(polinomPowForUs+1);
std::vector<long double> koefUy(polinomPowForUy+1);

std::vector<long double> newB;
std::vector<long double> newDependence;

std::vector<long double> inB(FilteredB);
std::vector<long double> inDependence(FilteredDependence);

h=2.0/NumberOfPoints;
unsigned int i=0;
        if(dependenceType==HALL_EFFECT)
			for(int i=0;i<500;i++)
			{
				inB.push_back(0);
				inDependence.push_back(0);
			}

        if(dependenceType==HALL_EFFECT)
            curveFittingUniversal(&inB,&inDependence, &koefUy,polinomPowForUy);
        if(dependenceType==MAGNETORESISTANCE)
            curveFittingUniversal(&inB,&inDependence, &koefUs,polinomPowForUs);

			newB.clear();
			newB.push_back(0);
			for (unsigned int i = 1; i < NumberOfPoints; i++) {
				newB.push_back(newB[i-1]+h);
			}
        if(dependenceType==MAGNETORESISTANCE)
			calculatePolinomByKoef(newB,koefUs,newDependence);
        if(dependenceType==HALL_EFFECT)
			calculatePolinomByKoef(newB,koefUy,newDependence);

            ExtrapolatedB=newB;

            ExtrapolatedDependence=newDependence;

			//----------ј вот тут прикручиваем недостающий кусочек в сигналы----

			while(FilteredB[i++]<=0 && i<NumberOfPoints);
			i-=1; // ищем где поле становитс€ положительным.

            for(unsigned int j=i;j<NumberOfPoints;j++)
			{      // перемещаем эти значени€ в начало.
				FilteredB[j-i]=FilteredB[j];
				FilteredDependence[j-i]=FilteredDependence[j];
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
	std::string strToReplaceWhich=","; // на что мен€ем
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
	std::string strToReplaceWhich="."; // на что мен€ем
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
        pointToY=&Dependence;
        break;
    case FILTERED_DATA:
        pointToX=&FilteredB;
        pointToY=&FilteredDependence;
        break;
    case EXTRAPOLATED_DATA:
        pointToX=&ExtrapolatedB;
        pointToY=&ExtrapolatedDependence;
        break;
    case ORIGINAL_DATA:
        pointToX=&OriginalB;
        pointToY=&OriginalDependence;
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
        temp=&Dependence;
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
    // и вставить всЄ как надо в массивы B и Dependence

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
        OriginalB=tempData[1]; // должно быть 2, но пока отладка
        OriginalDependence=tempData[0];

        B=OriginalB;
        Dependence=OriginalDependence;
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

std::vector<MyDataType> const & MagneticFieldDependence::getDependence()
{
    return Dependence;
}

std::vector<MyDataType> const & MagneticFieldDependence::getOriginalB()
{
    return OriginalB;
}
std::vector<MyDataType> const & MagneticFieldDependence::getOriginalDependence()
{
    return OriginalDependence;
}
std::vector<MyDataType> const & MagneticFieldDependence::getFilteredB()
{
    return FilteredB;
}
std::vector<MyDataType> const & MagneticFieldDependence::getFilteredDependence()
{
    return FilteredDependence;
}
std::vector<MyDataType> const & MagneticFieldDependence::getExtrapolatedB()
{
    return ExtrapolatedB;
}
std::vector<MyDataType> const & MagneticFieldDependence::getExtrapolatedDependence()
{
    return ExtrapolatedDependence;
}
