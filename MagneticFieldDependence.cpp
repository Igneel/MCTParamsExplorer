#include "MagneticFieldDependence.h"
#include "Unit1.h"

MagneticFieldDependence::MagneticFieldDependence()
{
    dependenceType=HALL_EFFECT;
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

    long double * tempInB=new long double[2*NumberOfPoints];
	long double * tempInSignal=new long double[2*NumberOfPoints];
	long double * tempOutB=new long double[2*NumberOfPoints+ceil(lengthFilter/2.0)];
	long double * tempOutSignal=new long double[2*NumberOfPoints+ceil(lengthFilter/2.0)];

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

    // ��������� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                2*NumberOfPoints,lengthFilter,SamplingFrequecy,BandwidthFrequency,AttenuationFrequency);
// ��� ���� ����������������!

/*


	// ��������� ������ ��� �������.
	// ���������� ��� � ������������� ��������� ����.
	for (int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=-idealUs[NumberOfPoints-i-1]+2*idealUs[0];
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=idealUs[i];
		tempInB[i+NumberOfPoints]=B[i];
	}


    // ���������
	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,2*NumberOfPoints,lengthFilter,5000,15,25);

	// ���� �������� ��������� ���������� ����������� ����� ����������.
	// � �������������

	// ����� ���������� ������������� ����� �������������� ������� �������.
	for(int i=0;i<NumberOfPoints;i++)
	{
		Us[i]=tempOutSignal[i+NumberOfPoints-1];
		//Us[i]=tempOutSignal[i];
    }
    // ������ �� �� ����� � ������ ��������, ��, ���� ������������� �������.
	for (int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=idealUy[NumberOfPoints-i-1];   // ���
		//tempInSignal[i]=-idealUy[NumberOfPoints-i-1]+2*idealUy[0];  // �����
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=idealUy[i];
		tempInB[i+NumberOfPoints]=B[i];
	}

	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,2*NumberOfPoints,lengthFilter,5000,15,25);

	for(int i=0;i<NumberOfPoints;i++)
	{
		B[i]=tempOutB[i+NumberOfPoints-1];
		Uy[i]=tempOutSignal[i+NumberOfPoints-1];
	}
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	for(int i=0;i<NumberOfPoints;i++)
	{
		if(B[i]<0)
		{
			B[i]=0;
			//Us[i]=0;
			Uy[i]=0;
		}
	}
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	// ������������� �����������.


	 */
    delete[] tempInB;
	delete[] tempInSignal;
	delete[] tempOutB;
	delete[] tempOutSignal;

	//return 1;
}

void MagneticFieldDependence::extrapolateData()
{

// ���� ���� ��������������.

/*
int returnValue=1;

const int polinomPowForUs=4;
const int polinomPowForUy=4;

std::vector<long double> koefUs;
std::vector<long double> koefUy;

std::vector<long double> newB;
std::vector<long double> newUs;
std::vector<long double> newUy;

koefUs.resize(polinomPowForUs+1);
koefUy.resize(polinomPowForUy+1);

int i=0;
	switch(type)
	{
		case EXTRAPOLATE:

			for(int i=0;i<500;i++)
			{
				B.push_back(0);
				Uy.push_back(0);
			}

			curveFittingUniversal(&B, &Us, &koefUs,polinomPowForUs);
			curveFittingUniversal(&B, &Uy, &koefUy,polinomPowForUy);

            for(int i=0;i<500;i++)
			{
				B.pop_back();
				Uy.pop_back();
			}

			newB.clear();
			newB.push_back(0);
			for (int i = 1; i < NumberOfPoints; i++) {
				newB.push_back(newB[i-1]+h);
			}

			calculatePolinomByKoef(newB,koefUs,newUs);
			calculatePolinomByKoef(newB,koefUy,newUy);

			extrapolatedParams->setB_Us_Uy(newB,newUs,newUy);

			//----------� ��� ��� ������������ ����������� ������� � �������----

			while(B[i++]<=0 && i<NumberOfPoints);
			i-=2; // ���� ��� ���� ���������� �������������.

            for(int j=i;j<NumberOfPoints;j++)
			{      // ���������� ��� �������� � ������.
				B[j-i]=B[j];
				Us[j-i]=Us[j];
				Uy[j-i]=Uy[j];
			}

			for(int j=NumberOfPoints-i;j<NumberOfPoints;j++)
			{     // � ����� ���������� ������������������ ��������.
				B[j]=newB[j];
				Us[j]=newUs[j];
				Uy[j]=newUy[j];
			}

			calculateEffectiveParamsFromSignals();
			calculateTenzorFromEffectiveParams();
			//------------------------------------------------------------------

			break;
		default:
		returnValue=0;
	}

return returnValue;   */
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

void MagneticFieldDependence::constructPlotFromTwoMassive(TLineSeries* s,TColor color)
{
	s->Clear();
        NumberOfPoints=1000;
	for (int i = 0; i < NumberOfPoints; i++)
	{
		s->AddXY(OriginalB[i],OriginalDependence[i],"",color);
	}
}
void MagneticFieldDependence::constructPlotFromOneMassive(TLineSeries* s,TColor color)
{
	s->Clear();
        NumberOfPoints=OriginalDependence.size();
	for (int i = 0; i < NumberOfPoints; i+=1)
	{
		s->AddY(OriginalDependence[i],"",color);
	}
}

std::vector<MyDataType> const &  MagneticFieldDependence::getData()
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
}


