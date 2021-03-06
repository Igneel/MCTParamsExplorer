
#include "THallEffect.h"

THallEffect::THallEffect(long double * array, size_t length):Signal(array, length)
{
	int i=0;
 ;
}

THallEffect::THallEffect(TSignalIt begin, TSignalIt end):Signal(begin, end)
{
 int i=0;
}

THallEffect::~THallEffect()
{
 int i=0;
}


virtual void THallEffect::featTo(TSignal & s)
{
	TSignal temp;
	OddFeat(signal,temp);
	s=temp;
}

virtual void THallEffect::averageTo(TSignal & s)
{
	TSignal temp;
	OddFeat(signal,temp);
	s=temp;
}

virtual void THallEffect::filterTo(TSignal & s)
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

virtual void THallEffect::extrapolateTo(TSignal & s,TSignal & B)
{
	bool returnValue=true;
    
    TSignal koef(ExtapolatePolinomDegree+1);

    TSignal newB;
    TSignal newS;

    TSignal inB;
    TSignal in;

    /*
    Экстраполяция данных ведется по последней четверти фильтрованных значений.
    */
    for (size_t i = 3*B.size()/4; i < B.size(); ++i)
    {
        inB.push_back( B[i]);        
        in.push_back( signal[i]);
    }

    unsigned int NumberOfPoints=in.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("Количество точек равно нулю! Я не хочу делить на ноль:)");

    }
    MyDataType h=2.2/NumberOfPoints;
    

	for(int i=0;i<500;i++) // увеличиваем вес точки (0,0) для эффекта Холла.
	{
		inB.push_back(0);
		in.push_back(0);
	}

    curveFittingUniversal(&inB,&in, &koef,ExtapolatePolinomDegree);
    

    

	newB.clear();
	newB.push_back(0); // заполняем магнитное поле.
	for (unsigned int i = 1; i < NumberOfPoints; i++) {
		newB.push_back(newB[i-1]+h);
	}

    // вычисляем экстраполированные зависимости.
	calculatePolinomByKoef(newB,koef,newS);

    B=newB;
    s=newS;
    /*
	//----------А вот тут прикручиваем недостающий кусочек в сигналы----

    // Упс, а мы точки-то лишние из фильтрованного удаляем?
    // Где-то явно не удаляем, но возможно и не здесь.
    
    unsigned int i=0;
	while(i<NumberOfPoints && newB[i]<FilteredB.back())
    ++i; // находим элемент на котором заканчивается фильтрованное магнитное поле.

   	for(unsigned int j=i;j<NumberOfPoints;j++)
	{     // в конце дописываем экстраполированные значения.
		FilteredB.push_back(newB[j]);
        FilteredMagnetoResistance.push_back(newMagnetoResistance[j]);
        FilteredTHallEffect.push_back(newTHallEffect[j]);
	}
	//------------------------------------------------------------------
	*/
}
