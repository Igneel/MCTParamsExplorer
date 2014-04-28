//---------------------------------------------------------------------------
#include "FilteringUnit.h"

FilterLowBand::FilterLowBand(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh)
{
    N=0;
    calculateImpulseResponse(length, Fdisk, Fpropysk, Fzatyh);
}

FilterLowBand::~FilterLowBand()
{
    ;
}

void FilterLowBand::calculateImpulseResponse(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh)
{
    if (N!=length || Fd!=Fdisk || Fs!=Fpropysk || Fx != Fzatyh)
    {
        N = length; //����� �������
        Fd = Fdisk; //������� ������������� ������� ������ 
        Fs = Fpropysk; //������� ����� ������ �����������  
        Fx = Fzatyh; //������� ������ ������ ���������    

        H.resize(N);
        H_id.resize(N);
        W.resize(N);
        //������ ���������� �������������� �������
        long double Fc = (Fs + Fx) / (2.0 * Fd);

        for (int i=0;i<N;++i)
        {
            if (i==0) H_id[i] = 2.0*M_PI*Fc;
            else H_id[i] = sinl(2.0*M_PI*Fc*i )/(M_PI*i);
            // ������� ������� ��������
            W[i] = 0.42 - 0.5 * cosl((2.0*M_PI*i) /( N-1.0)) + 0.08 * cosl((4.0*M_PI*i) /( N-1.0));
            H[i] = H_id[i] * W[i];
        }
        //���������� ���������� ��������������
        long double SUM=0.0;
        for (int i=0; i<N; ++i) SUM +=H[i];
        for (int i=0; i<N; ++i) H[i]/=SUM; //����� ������������� ����� 1
    }
}

void FilterLowBand::setFilterParams(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh)
{
    calculateImpulseResponse(length, Fdisk, Fpropysk, Fzatyh);
}

double FilterLowBand::FilterData (const std::vector<long double> &in, std::vector<long double> & out)
{
    //���������� ������� ������
    unsigned int dataSize=in.size();
    out.resize(dataSize);
    for (int i=0; i<dataSize; ++i)
    {
        out[i]=0.0;
        for (int j=0; j<(i>N-1?N-1:i); ++j)// �� ����� ������� �������
            out[i]+= H[j]*in[i-j];
    }
    return (N-1.0)/2.0;
}

void FilterLowBand::FilterDataWithAutoShift(DataTypeInContainer & inB,
    DataTypeInContainer & inY,DataTypeInContainer & outB,
    DataTypeInContainer & outY)
{
    int lengthMassive=inY.size();
    if(lengthMassive==0)
    {
    return ;
    }
    std::vector<long double> out(lengthMassive);

    double k=FilterData(inY,out); // �������� ������
    k*=(max_elem(inB)-min_elem(inB))/(double)lengthMassive;// ��������� ����� ���
    // �������� ��������� � �������� �� ����� �������
    outB.resize(lengthMassive);
    outY=out;
    for(int i=0;i<lengthMassive;i++) // �������
    {
        outB[i]=inB[i]-k;
    }
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// ��������!!! �������� �� ��������!!! ������������ ����������!!!---------------
double Filter (const std::vector<long double> &in, std::vector<long double> & out, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = length; //����� �������
long double Fd = Fdisk; //������� ������������� ������� ������ 2000
long double Fs = Fpropysk; //������� ����� ������ �����������  10
long double Fx = Fzatyh; //������� ������ ������ ���������    20

std::vector<long double> H(N);  //���������� �������������� �������
std::vector<long double> H_id(N); //��������� ���������� ��������������
std::vector<long double> W(N);   //������� �������

//������ ���������� �������������� �������
long double Fc = (Fs + Fx) / (2.0 * Fd);

for (int i=0;i<N;++i)
{
if (i==0) H_id[i] = 2.0*M_PI*Fc;
else H_id[i] = sinl(2.0*M_PI*Fc*i )/(M_PI*i);
// ������� ������� ��������
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1.0));
H [i] = H_id[i] * W[i];
}

//���������� ���������� ��������������
long double SUM=0;
for (int i=0; i<N; ++i) SUM +=H[i];
for (int i=0; i<N; ++i) H[i]/=SUM; //����� ������������� ����� 1

//���������� ������� ������
unsigned int dataSize=in.size();
for (int i=0; i<dataSize; ++i)
{
out[i]=0.0;
for (int j=0; j<(i>N-1?N-1:i); ++j)// �� ����� ������� �������
out[i]+= H[j]*in[i-j];
}

return (N-1)/2.0;
}




// ������� �������� ��� �������.  ----------------------------------------------
long double TrForMassiveFilter(std::vector<long double> & inB,
std::vector<long double> & inY,std::vector<long double> & outB,
std::vector<long double> & outY,int lengthFilter,long double Fdisk,
long double Fpropysk,long double Fzatyh)
{
    int lengthMassive=inY.size();
    if(lengthMassive==0)
    {
    return 0;
    }
    std::vector<long double> in(inY);
    std::vector<long double> out(lengthMassive);

    double k=Filter(in,out,lengthFilter,Fdisk,Fpropysk,Fzatyh); // �������� ������
    k*=(max_elem(inB)-min_elem(inB))/(double)lengthMassive;// ��������� ����� ���
    // �������� ��������� � �������� �� ����� �������
 
//----------------------------------------------
//---------���������� ��� ���������� ���������� ����
// �������� - ��� ���� �������� �� ��������� ��� ������ ����������� ������ ��
// ��� �����.
/*double *in2=new double[size];  // �������� ������
for(int i=0;i<size;i++)       // ��������
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // �������� ������ ��� �������� ��������
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // �������� ������
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// ��������� ����� ���
*/

//----------------------------------------------
    outB.resize(lengthMassive);
    outY=out;
    for(int i=0;i<lengthMassive;i++) // �������
    {
	    outB[i]=inB[i]-k;
    }
    return k;
}


