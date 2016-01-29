#include "commonFunctions.h"
//------------------------------------------------------------------------------
inline MyDataType dist(MyDataType x1, MyDataType x2)
{
    return fabs(x1-x2);
}



//------------------------------------------------------------------------------
/*
Прореживание зависимостей - по идее функция универсальная.
Способная из массива одной длины получить массив меньшей длины.
При этом рассчитывается новый равномерный шаг по сетке.

Используется для:
1. Согласования измеренных данных после получения их с АЦП.
2. Согласования данных после фильтрации.
3. Согласования данных после экстраполяции.
4. Сохранении данных с заданным шагом и количеством точек.


Есть такая проблема - сигнал для отрицательного магнитного поля поступает сюда
в порядке от 0 до -2
т.е. вообще говоря в обратном, поэтому это не пашет.
*/
bool thiningSignal(TSignal & inB, TSignal & inDependence, TSignal & outB, TSignal & outDependence,
    MyDataType left, MyDataType right, size_t NewLength)
{
    if (right<left) // если при вызове перепутали границы.
    {
        MyDataType temp=right;
        right = left;
        left = temp;
    }

    size_t OldLength = inB.size();

    if (NewLength>OldLength) // если просят увеличить количество точек.
    {
        return false;
    }

    if (OldLength != inDependence.size())
    {
        return false; // если зависимости разной длины - мы не можем им помочь.
    }

    if (OldLength==NewLength)
    {
    // при совпадении размеров - нам делать совсем нечего.
        outDependence=inDependence;
        outB=inB;
        return true;
    }

    outB.clear(); // чистим выходные зависимости, на случай если это не сделали за нас.
    outDependence.clear();

    TSignal idealB; // тут будем хранить опорные точки.

    // шаг есть величина диапазона на количество интервалов (на единицу меньше количества точек)
    MyDataType shag=(right-left)/(static_cast<MyDataType>(NewLength)-1.0);

    idealB.push_back(left); // начинаем с наименьшей границы
    for (unsigned int i=1; i < NewLength; ++i) 
    {
    // создаем сетку для магнитного поля.
        idealB.push_back(idealB[i-1]+shag);
    }
    // тут начинается поиск.
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
    // возможно стоит прикрутить более оптимальный поиск.
    /*
    for (unsigned int i = 0; i < NewLength; ++i)
    {
    // одно из самых тупых мест в этой программе.
    // может прикрутить сюда бинарный поиск?
    // данные-то по идее отсортированы
    // не считая точек возле нулевого значения магнитной индукции.
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

    if(inB[0]>inB[inB.size()/2]) // попробуем отделить отрицательный сигнал.
    {    // нас волнует именно порядок элементов
    // т.е. в начале у нас в районе нуля
    // в конце в районе -2
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


