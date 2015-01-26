#include "commonFunctions.h"
//------------------------------------------------------------------------------
inline MyDataType dist(MyDataType x1, MyDataType x2)
{
    return fabs(x1-x2);
}
//------------------------------------------------------------------------------
/*
������������ ������������ - �� ���� ������� �������������.
��������� �� ������� ����� ����� �������� ������ ������� �����.
��� ���� �������������� ����� ����������� ��� �� �����.

������������ ���:
1. ������������ ���������� ������ ����� ��������� �� � ���.
2. ������������ ������ ����� ����������.
3. ������������ ������ ����� �������������.
4. ���������� ������ � �������� ����� � ����������� �����.


���� ����� �������� - ������ ��� �������������� ���������� ���� ��������� ����
� ������� �� 0 �� -2
�.�. ������ ������ � ��������, ������� ��� �� �����.
*/
bool thiningSignal(TSignal & inB, TSignal & inDependence, TSignal & outB, TSignal & outDependence,
    MyDataType left, MyDataType right, size_t NewLength)
{
    if (right<left) // ���� ��� ������ ���������� �������.
    {
        MyDataType temp=right;
        right = left;
        left = temp;
    }

    size_t OldLength = inB.size();

    if (NewLength>OldLength) // ���� ������ ��������� ���������� �����.
    {
        return false;
    }

    if (OldLength != inDependence.size())
    {
        return false; // ���� ����������� ������ ����� - �� �� ����� �� ������.
    }

    if (OldLength==NewLength)
    {
    // ��� ���������� �������� - ��� ������ ������ ������.
        outDependence=inDependence;
        outB=inB;
        return true;
    }

    outB.clear(); // ������ �������� �����������, �� ������ ���� ��� �� ������� �� ���.
    outDependence.clear();

    TSignal idealB; // ��� ����� ������� ������� �����.

    // ��� ���� �������� ��������� �� ���������� ���������� (�� ������� ������ ���������� �����)
    MyDataType shag=(right-left)/(static_cast<MyDataType>(NewLength)-1.0);

    idealB.push_back(left); // �������� � ���������� �������
    for (unsigned int i=1; i < NewLength; ++i) 
    {
    // ������� ����� ��� ���������� ����.
        idealB.push_back(idealB[i-1]+shag);
    }
    // ��� ���������� �����.
    /*
    TSignal indexes(NewLength);
    TSignal distance(NewLength);
    for (int i = 0; i < NewLength; ++i)
    {
        indexes[i]=0;
        distance[i]=dist(inB[i],idealB[i]);
    }

    for (int i = 1; i < NewLength; ++i)
    {
        if (dist(inB[i],idealB[i])<distance[i])
        {
            distance[i]=dist(inB[i],idealB[i]);
            indexes[i]=i;
        }
    }*/
    // �������� ����� ���������� ����� ����������� �����.
    /*
    for (unsigned int i = 0; i < NewLength; ++i)
    {
    // ���� �� ����� ����� ���� � ���� ���������.
    // ����� ���������� ���� �������� �����?
    // ������-�� �� ���� �������������
    // �� ������ ����� ����� �������� �������� ��������� ��������.
        unsigned int index=0;
        long double r=4;
        for(unsigned int k=0;k<OldLength;++k)
        {
            if(dist(inB[k],idealB[i])<=r)
            {
                r=dist(inB[k],idealB[i]);
                index=k;
            }
        }
        if(index<OldLength)
        {
            outB.push_back(inB[index]);
            outDependence.push_back(inDependence[index]);
        }
    }
       */
    for (unsigned int i = 0; i < NewLength; ++i)
    {

    if(inB[0]>inB[inB.size()/2]) // ��������� �������� ������������� ������.
    {    // ��� ������� ������ ������� ���������
    // �.�. � ������ � ��� � ������ ����
    // � ����� � ������ -2
    std::reverse(inB.begin(),inB.end());
    }
    std::vector<MyDataType>::iterator b=std::lower_bound(inB.begin(),inB.end(),idealB[i]);
    if(b==inB.end())
        --b;
    if(b!=inB.begin())
    {
        if (fabs(fabs(*b)-fabs(idealB[i]))>fabs (fabs(*(b-1))-fabs(idealB[i])))
            --b;
    }
    outB.push_back(*b);
    int d=std::distance(inB.begin(),b);
    outDependence.push_back(inDependence[d]);

    }

    return true;
}
//------------------------------------------------------------------------------


