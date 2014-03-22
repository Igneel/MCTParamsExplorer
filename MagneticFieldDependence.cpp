#include "MagneticFieldDependence.h"
#include "Unit1.h"

MagneticFieldDependence::MagneticFieldDependence(DependenceType d)
{
    dependenceType=d;
    Current=0.001;
    h=0.001;
    NumberOfPoints=10;
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

}

void MagneticFieldDependence::SaveData(DataKind dataKind,SaveType saveType,string FileName)
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
        //"������� ������ ������ ������!!!");
        return;
    }
    // ��������� ������ � ���������� ������������ ������� ������� �������
    // 0 ��� �, 1 ��� y

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
        case ODD_FEAT: // �������� ��������
            tempY[i]=(tempY[i]-tempY[size-1-i])/2.0;
            tempY[size-1-i]=-tempY[i];
            break;
        case EVEN_FEAT: // ������ ��������
            tempY[i]=(tempY[i]+tempY[size-1-i])/2.0;
            tempY[size-1-i]=tempY[i];
            break;
        }

    }

}

void MagneticFieldDependence::filterData(DependenceType dependenceType,MyDataType SamplingFrequecy, MyDataType BandwidthFrequency,
     MyDataType AttenuationFrequency,int lengthFilter)
{
    std::vector<long double> tempInB(2*NumberOfPoints);
    std::vector<long double> tempInSignal(2*NumberOfPoints);

    std::vector<long double> tempOutB(2*NumberOfPoints+ceil(lengthFilter/2.0));
    std::vector<long double> tempOutSignal(2*NumberOfPoints+ceil(lengthFilter/2.0));

    switch(dependenceType)
    {
    case HALL_EFFECT:
    // ��������� ������ ��� �������.
	// ���������� ��� � ������������� ��������� ����.
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
		tempInSignal[i]=Dependence[NumberOfPoints-i-1];   // ���
		//tempInSignal[i]=-Dependence[NumberOfPoints-i-1]+2*Dependence[0];  // �����
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=Dependence[i];
		tempInB[i+NumberOfPoints]=B[i];
	}
    break;
    }

    // ��������� 
	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                lengthFilter,SamplingFrequecy,BandwidthFrequency,AttenuationFrequency);


    // ����� ���������� ������������� ����� �������������� ������� �������.
	for(int i=0;i<NumberOfPoints;i++)
	{
        FilteredB[i]=tempOutB[i+NumberOfPoints-1];
		FilteredDependence[i]=tempOutSignal[i+NumberOfPoints-1];
    }
    /*
    switch(dependenceType)
    {
    case HALL_EFFECT:
    // ������� ������������� �������� ���������� ����, ��� ������� �����.
    // ������ ��� � ����� 0.
    // ���� �����...� �� �������� ��� ��� �������������...
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

std::vector<long double> inB;
std::vector<long double> inDependence;


int i=0;
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
			for (int i = 1; i < NumberOfPoints; i++) {
				newB.push_back(newB[i-1]+h);
			}
        if(dependenceType==MAGNETORESISTANCE)
			calculatePolinomByKoef(newB,koefUs,newDependence);
        if(dependenceType==HALL_EFFECT)
			calculatePolinomByKoef(newB,koefUy,newDependence);

            ExtrapolatedB=newB;

            ExtrapolatedDependence=newDependence;

			//----------� ��� ��� ������������ ����������� ������� � �������----

			while(FilteredB[i++]<=0 && i<NumberOfPoints);
			i-=2; // ���� ��� ���� ���������� �������������.

            for(int j=i;j<NumberOfPoints;j++)
			{      // ���������� ��� �������� � ������.
				FilteredB[j-i]=FilteredB[j];
				FilteredDependence[j-i]=FilteredDependence[j];
				//Uy[j-i]=Uy[j];
			}

			for(int j=NumberOfPoints-i;j<NumberOfPoints;j++)
			{     // � ����� ���������� ������������������ ��������.
				FilteredB[j]=newB[j];
				Dependence[j]=newDependence[j];
			}
			//------------------------------------------------------------------

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
    *temp=B;
    break;
    case FILTERED_DATA:
    *temp=FilteredB;
    break;
    case EXTRAPOLATED_DATA:
    *temp=ExtrapolatedB;
    break;
    case ORIGINAL_DATA:
    *temp=OriginalB;
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
	std::string strToReplaceWhich=","; // �� ��� ������
	std::string strToSearch=".";   // ��� ����
	findIndex=s.find(strToSearch,strToSearch.length());
	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
    strToReplaceWhich.begin(),strToReplaceWhich.end());
    out=s;
}

inline void MagneticFieldDependence::ReplaceCommaToDots(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich="."; // �� ��� ������
	std::string strToSearch=",";   // ��� ����
	findIndex=s.find(strToSearch,strToSearch.length());
	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
    strToReplaceWhich.begin(),strToReplaceWhich.end());
    out=s;
}

void MagneticFieldDependence::constructPlotFromTwoMassive(DataKind p,TLineSeries* s,TColor color)
{
    std::vector<MyDataType> * pointToX;
    std::vector<MyDataType> * pointToY;
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
    }

    NumberOfPoints=pointToX->size();
	for (int i = 0; i < NumberOfPoints; i++)
	{
		s->AddXY((*pointToX)[i],(*pointToY)[i],"",color);
	}
}
void MagneticFieldDependence::constructPlotFromOneMassive(PlotType p,TLineSeries* s,TColor color)
{
    std::vector<MyDataType> * temp;
	s->Clear();
    switch(p)
    {
    case MAGNETIC_FIELD:
    temp=&OriginalB;
    break;
    case DEPENDENCE:
    temp=&OriginalDependence;
    break;
    }
        NumberOfPoints=temp->size();
	for (int i = 0; i < NumberOfPoints; i+=1)
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
    // ��� ������ �������, � ����������� �� ��������� ������.
    // � �������� �� ��� ���� � ������� B � Dependence

    // � ����� � ������� Original
    OriginalB.resize(tempData.size());
    OriginalB=tempData;
    OriginalDependence.resize(tempData.size());
    OriginalDependence=tempData;
    //����� ����:
    //filterData(dependenceType,400000,50,100,50);
    //extrapolateData();
    return OriginalB;
}   */

void MagneticFieldDependence::getSplittedDataFromADC()
{
std::vector<std::vector<MyDataType> > tempData(adc->getSplittedData());
if(tempData.size()>3) // ���� ��� �� �������� �������
{
OriginalB=tempData[1]; // ������ ���� 2, �� ���� �������
OriginalDependence=tempData[0];

}

}

bool MagneticFieldDependence::setFilterParams(FilterParams & fp)
{
filterParams=fp;
}

FilterParams MagneticFieldDependence::getFilterParams()
{
return filterParams;
}
