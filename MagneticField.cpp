
#include "MagneticField.h"

MagneticField::MagneticField(long double * array, size_t length):Signal(array, length)
{

}

MagneticField::MagneticField(TSignalIt begin, TSignalIt end):Signal(begin, end)
{

}

MagneticField::~MagneticField():~Signal()
{

}

virtual void MagneticField::featTo(TSignal & s)
{	
	TSignal temp;
	OddFeat(signal,temp);
	s=temp;
}

virtual void MagneticField::averageTo(TSignal & s)
{
	TSignal temp;
	OddFeat(signal,temp);
	s=temp;
}

virtual void MagneticField::filterTo(TSignal & s)
{	
	TSignal * in;
   
    unsigned int NumberOfPoints;

    // если это комбинированный сигнал.
    if( s[0]<-1.0 && s.back() >1.0)
    {
        feat(); // его надо усреднить
    // фильтровать будем усредненный сигнал
    }
    // если это обычный сигнал - фильтруем как есть.

    in=signal;  
    NumberOfPoints=in.size();

    // В эти массивы будут достраиваться данные для отрицательных магнитных полей.
    // Это очень мило, а если это сигнал для отрицательного магнитного поля?
    // То теоретически достраивается положительная часть.
    // Надо пофиксить тут комменты на адекватные действительности.
    TSignal tempIn(2*NumberOfPoints);
    TSignal tempOut(2*NumberOfPoints);

    // формируем сигнал для фильтра.
    // достраивая его в отрицательные магнитные поля.
    for (unsigned int i = 0; i < NumberOfPoints; i++)
    {
	    /*
	    Давайте внимательно сюда посмотрим.
	    У эффекта Холла отрицательная симметрия, относительно точки
	    B==0;
	    С чего вообще я взял, что это нулевой элемент? /(О_о)\

	    Получается для сигнала с положительным магнитным полем - это выполняется.
	    Для сигнала комбинированного, т.е. уже объединенного - это выполняется,
	    потому что фильтруется усредненный сигнал (по сути имеющий только значения
	    положительного магнитного поля.

	    Для отрицательного магнитного поля сие равенство, насколько мне ясно - не выполняется.
	    */        
        tempIn[i]=-(*inB)[NumberOfPoints-i-1];        
        tempIn[i+NumberOfPoints]=(*in)[i];
    } 

    /*
    В случае отрицательного магнитного поля надо инвертировать порядок элементов
    Потому что впереди выстраиваются значения для положительного магнитного поля.
    */

    if(tempIn[0]>1.0)
    {
        std::reverse(tempIn.begin(),tempIn.end());
    }
    // фильтруем 
    TrForMassiveFilter(tempIn,tempOut,filterParams.filterLength,filterParams.SamplingFrequecy,
                filterParams.BandwidthFrequency,filterParams.AttenuationFrequency);    
    // Размер внутри фильтра меняется, в зависимости от длины фильтра.
    NumberOfPoints=tempOut.size();
    s.clear();
    for(unsigned int i=fP.filterLength;i<NumberOfPoints;i++)
    {      
        s.push_back(tempOut[i]);
    }
}




