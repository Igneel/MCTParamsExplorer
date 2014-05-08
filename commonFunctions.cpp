#include "commonFunctions.h"

inline dist(MyDataType x1, MyDataType x2)
{
    fabs(x1-x2);
}

bool thiningSignal(DataTypeInContainer & inB, DataTypeInContainer & inDependence, DataTypeInContainer & outB, DataTypeInContainer & outDependence, 
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

    if (OldLength==inDependence.size())
    {
        outDependence=inDependence;
        outB=inB;
        return true;
    }

    outB.clear(); // ������ �������� �����������, �� ������ ���� ��� �� ������� �� ���.
    outDependence.clear();

    DataTypeInContainer idealB; // ��� ����� ������� ������� �����.

    MyDataType shag=(right-left)/(static_cast<MyDataType>(NewLength)-1.0); // ��� ���� �������� ��������� �� ���������� ���������� (�� ������� ������ ���������� �����)

    idealB.push_back(left); // �������� � ���������� �������
    for (int i=1; i < NewLength; ++i) 
    {
        idealB.push_back(idealB[i-1]+shag);
    }
    // ��� ���������� �����.
    /*
    DataTypeInContainer indexes(NewLength);
    DataTypeInContainer distance(NewLength);
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
    for (int i = 0; i < NewLength; ++i) 
    {
        unsigned int index=0;
        long double r=4;
        for(int k=0;k<OldLength;++k)
        {
            if(dist(inB[k],idealB[i])<=r)
            {
                r=fabs(inB[k]-idealB[i]);
                index=k;
            }
        }
        if(index<OldLength)
        {
            outB.push_back(inB[index]);
            outDependence.push_back(inDependence[index]);
        }
    }

    return true;
}