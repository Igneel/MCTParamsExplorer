//---------------------------------------------------------------------------

#ifndef FilteringUnitH
#define FilteringUnitH

#include <Series.hpp>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include "UsedTypes.h"
#include "commonFunctions.h"
#pragma hdrstop
#pragma package(smart_init)

class FilterLowBand
{
public:
    FilterLowBand(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh);
    ~FilterLowBand();

    void FilterDataWithAutoShift(DataTypeInContainer & inB,
    DataTypeInContainer & inY,DataTypeInContainer & outB,
    DataTypeInContainer & outY);

    double FilterData (const std::vector<long double> &in,
    std::vector<long double> & out);

    void setFilterParams(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh);
    
private:

    void calculateImpulseResponse(unsigned int length,long double Fdisk, long double Fpropysk, long double Fzatyh);

    unsigned int N; //����� ������� � ���� ��� �� ������ ��������� �� ������)))
    long double Fd; //������� ������������� ������� ������
    long double Fs; //������� ����� ������ ����������� 
    long double Fx; //������� ������ ������ ���������

    std::vector<long double> H;  //���������� �������������� �������
    std::vector<long double> H_id; //��������� ���������� ��������������
    std::vector<long double> W;   //������� �������
};

MyDataType BlockLowBandFilter(DataTypeInContainer & inB,
DataTypeInContainer & inY,DataTypeInContainer & outB,
DataTypeInContainer & outY,
size_t lengthFilter,MyDataType Fdisk,MyDataType Fpropysk,MyDataType Fzatyh,
size_t blockSize);

MyDataType TrForMassiveFilter(DataTypeInContainer & inB,
DataTypeInContainer & inY,DataTypeInContainer & outB,
DataTypeInContainer & outY,
int lengthFilter,MyDataType Fdisk,MyDataType Fpropysk,MyDataType Fzatyh);

double Filter (const std::vector<long double> &in,
std::vector<long double> & out, int length,
double Fdisk, double Fpropysk,double Fzatyh);




//---------------------------------------------------------------------------
#endif
