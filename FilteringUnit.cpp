//---------------------------------------------------------------------------
#include "FilteringUnit.h"

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

for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2.0*M_PI*Fc;
else H_id[i] = sinl(2.0*M_PI*Fc*i )/(M_PI*i);
// ������� ������� ��������
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1.0));
H [i] = H_id[i] * W[i];
}

//���������� ���������� ��������������
long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //����� ������������� ����� 1

//���������� ������� ������
for (int i=0; i<in.size(); i++)
{
out[i]=0.0;
for (int j=0; j<(i>N-1?N-1:i); j++)// �� ����� ������� �������
out[i]+= H[j]*in[i-j];
}

return (N-1)/2.0;
}

template <class T>
inline T max_elem(const std::vector<T> & in)
{
    if(in.size()==0)
    {
        //"������ ����� ������������ �������, ����� ��� �� ������");
        return 0;
    }
    std::vector<T>::const_iterator pos;
    T max=*in.begin();
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(*pos>max)
        max=*pos;
    }
    return max;
}

template <class T>
inline T min_elem(const std::vector<T> & in)
{
    if(in.size()==0)
    {
        //"������ ����� ������������ �������, ����� ��� �� ������");
        return 0;
    }
    std::vector<T>::const_iterator pos;
    T min=*in.begin();
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(*pos<min)
        min=*pos;
    }
    return min;
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


