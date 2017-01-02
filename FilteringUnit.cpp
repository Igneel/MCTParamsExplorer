//---------------------------------------------------------------------------


#pragma hdrstop

#include "FilteringUnit.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// ������� �������� ��� �������.  ----------------------------------------------
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh)
{
if(!inS->YValues->Count)   // ���� ������ - ������, �������.
{
ShowMessage("������ ������!");
return 0;
}
int size=inS->YValues->Count;  // �������� ������
TSignal in(size);
TSignal out(size);
//double *in=new double[size];  // �������� ������
for(int i=0;i<size;i++)       // ��������
in[i]=inS->YValues->Value[i];
//double *out=new double[size]; // �������� ������ ��� �������� ��������
double k=Filter(in,out,length,Fdisk,Fpropysk,Fzatyh); // �������� ������
 k*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// ��������� ����� ���

//----------------------------------------------
outS->Clear(); // ������ ������, ����� �������

for(int i=0;i<size;i++) // �������
{
outS->AddXY(inS->XValues->Value[i]-k,out[i],"",clBlue);
}

return k;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// ��������!!! �������� �� ��������!!! ������������ ����������!!!---------------
double Filter (const std::vector<long double> &in, std::vector<long double> & out, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
unsigned int N = length; //����� �������
long double Fd = Fdisk; //������� ������������� ������� ������ 2000
//long double Fs = Fpropysk; //������� ����� ������ �����������  10
//long double Fx = Fzatyh; //������� ������ ������ ���������    20

std::vector<long double> H(N);  //���������� �������������� �������
std::vector<long double> H_id(N); //��������� ���������� ��������������
std::vector<long double> W(N);   //������� �������

//������ ���������� �������������� �������
long double Fc = (Fpropysk + Fzatyh) / (2.0 * Fd);
long double Wc = 2*M_PI*Fc;
/*
������ ������ ������ �������� ��� ������� �������� ����������� � ���������� 5.5/N

����� ���� ������� ����� ����� �����

*/
//Fzatyh= Fpropysk/Fd+5.5/(long double) N;
//Fc =  (Fpropysk + Fzatyh) / 2.0;
//Wc = 2*M_PI*Fc;

for (unsigned int i=0;i<N;++i)
{ // ��������� ���������� �������������� ������� ������ ������
if (i==N/2) H_id[i] = 2*Fc;
else H_id[i] =2*Fc*sinl(Wc*(i-N/2))/(Wc*(i-N/2));
}

for (unsigned int i=0;i<N;++i)
{
    //if (i==0) H_id[i] = 2*Fc;
    //else H_id[i] =2*Fc*sinl(Wc*i)/(Wc*i);
    // ������� ������� ��������
    if(N>1)
        W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( long double)N) + 0.08 * cosl((4*M_PI*i) /( long double)N);
    else
        W[i]=1;
    H [i] = H_id[i] * W[i];
}

//���������� ���������� ��������������
long double SUM=0;
for (unsigned int i=0; i<N; ++i) SUM +=H[i];
for (unsigned int i=0; i<N; ++i) H[i]/=SUM; //����� ������������� ����� 1

//���������� ������� ������
unsigned int dataSize=in.size();
for (unsigned int i=0; i<dataSize; ++i)
{
out[i]=0.0;
for (unsigned int j=0; j<=(i>N-1?N-1:i); ++j)// �� ����� ������� �������
out[i]+= H[j]*in[i-j];
}

return (N-1)/2.0;
}


long double TrForMassiveFilter(TSignal & in,TSignal &out,
int lengthFilter,long double Fdisk,
long double Fpropysk,long double Fzatyh)
{
    int lengthMassive=in.size();
    if(lengthMassive==0)
    {
    return 0;
    }
    out.resize(lengthMassive);
    long double k=Filter(in,out,lengthFilter,Fdisk,Fpropysk,Fzatyh); // �������� ������
    return k;
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
    outY.resize(lengthMassive);
    double k=Filter(inY,outY,lengthFilter,Fdisk,Fpropysk,Fzatyh); // �������� ������
    k*=(max_elem(inB)-min_elem(inB))/(long double)lengthMassive;// ��������� ����� ���
    // �������� ��������� � �������� �� ����� �������

//----------------------------------------------
//---------���������� ��� ���������� ���������� ����
// ������
Filter(inB,outB,lengthFilter,Fdisk,Fpropysk,Fzatyh);



//----------------------------------------------
    outB.resize(lengthMassive);
    for(int i=0;i<lengthMassive;i++) // �������
    {
	    outB[i]=inB[i]-k;
    }
    return k;
}



// ������� ���������� ����������� �������.

long double D(long double deltaP, long double deltaS)
{
/*
����� ��� ������� ����� �������.
*/
	long double a1=5.309E-3;
	long double a2=7.114E-2;
	long double a3=-4.761E-1;
	long double a4=-2.66E-3;
	long double a5=-5.941E-1;
	long double a6=-4.278E-1;

return log10(deltaS*(a1*log10(deltaP)*log10(deltaP)+a2*
	log10(deltaP)+a3)+a4*log10(deltaP)*log10(deltaP)+a5*log10(deltaP)+a6);
}

long double f(long double deltaP, long double deltaS)
{
/*
����� ��� ������� ����� �������.
*/

return 11.01217+0.51244*(log10(deltaP)-log10(deltaS));

}

int calcutaleFilterLength(long double deltaP, long double deltaS, long double deltaF)
{
/*
������������ ����� ������� ��� ������ ����������� �������������.
�������� ������� �� �������� �������� ��� ������� ������ ������, �������� 410.

deltaP - ��������������� � ������ �����������.

deltaS - ��������������� � ������ ����������.

deltaF - ������ ������ �����������, ������������� �� ������� �������������.

N=D(deltaP,deltaS)/deltaF-f(deltaP,deltaS)*deltaF+1;

*/

return D(deltaP,deltaS)/deltaF-f(deltaP,deltaS)*deltaF+1;
}

long double fapprox(long double x)
{
return x*x;
}

long double OptimFilter(long double deltaP, long double deltaS, long double FPropusk, long double Fpodavl, long double FDiskr)
{
long double FPropuskN=FPropusk/FDiskr;
long double FPodavlN=Fpodavl/FDiskr;
long double deltaF=FPodavlN-FPropuskN;

int filterLength=calcutaleFilterLength(deltaP,deltaS,deltaF);

double weigthP = 1;
double weigthS = deltaS/deltaP;



}

//----------------------------------------------
//----------------------------------------------
#pragma package(smart_init)
