#include "MagneticFieldDependence.h"
#include "multizoneFit.h"
#include "mobilityspectrum.h"

extern LCardADC *adc;


MagneticFieldDependence::MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
    AnsiString length, AnsiString width, AnsiString Thickness)

{
    

    filterParamsHall=new FilterParams(); // по идее нужно бы и инциализировать их тут, дабы не было проблем в случае чего:).
    filterParamsResistance=new FilterParams();
    saver =new DataSaver(temperature,current,SampleInventoryNumber, length, width, Thickness);
    MobilitySpectrumObj=0;
    paramsType=DIRECT;
    leftBound.resize(3);
    leftBound[DIRECT]=0;
    leftBound[REVERSE]=-2;
    leftBound[COMBINE]=-2;
    rightBound.resize(3);
    rightBound[DIRECT]=2;
    rightBound[REVERSE]=0;
    rightBound[COMBINE]=2;

    PowPolinomRes=2;
    PowPolinomHall=2;


}

MagneticFieldDependence::~MagneticFieldDependence()
{
    if (saver)
    {
        delete saver;
        saver=0;
    }
    
    if (filterParamsHall)
    {
        delete filterParamsHall;
        filterParamsHall=0;
    }
    
    if (filterParamsResistance)
    {
        delete filterParamsResistance;
        filterParamsResistance=0;
    }    

    if(MobilitySpectrumObj)
    {
        delete MobilitySpectrumObj;
        MobilitySpectrumObj=0;
    }
}


void MagneticFieldDependence::loadDataHelper(TSignal &temp, String AnsiS,const std::string delimiter)
{
    temp.clear();
    std::string s1 = AnsiS.c_str();
    std::string s; 
    std::string::size_type start = 0;
    std::string::size_type finish = 0;
    ReplaceDotsToComma(s1,s);
    do 
        {
            finish = s.find(delimiter, start);
            std::string word = s.substr(start, finish-start);
            temp.push_back(StrToFloat(word.c_str()));
            start=finish+1;
        } while (finish != std::string::npos);
}

void MagneticFieldDependence::loadSampleDescription(TStringList *Names,TStringList *Values,AnsiString FileName)
{
    // Откусывает от имени файла часть, чтобы получить имя файла с описанием образца
    std::string temp = FileName.c_str();

    unsigned int rBound = temp.find_last_of("/\\");
    unsigned int rBound2 = temp.find_first_of("DCRT",rBound);
    AnsiString NewFileName = FileName.SubString(0,rBound2)+"Description"+".txt";

    TStringList * tts = new TStringList();
    if(FileExists(NewFileName))
    {
    tts->LoadFromFile(NewFileName); // загружаем это содержимое сюда.
    const std::string delimiter = "\t"; // и парсим

    std::string s1 = tts->Text.c_str();
    std::string s;
    ReplaceDotsToComma(s1,s); // если что - приводим всё к единому формату
    
    tts->Text = s.c_str();

    Names->Clear();
    Values->Clear();

    for (int i=0;i<tts->Count;++i)
    {
        s=tts->Strings[i].c_str();
        std::string::size_type start = 0;
        std::string::size_type finish = 0;

        if(s.empty())
            continue;

        finish = s.find(delimiter, start);
        Names->Add(s.substr(start, finish-start).c_str());
        Values->Add(s.substr(finish-start+1, s.length()).c_str());
    }

    setSampleDescription(Values->Strings[1],Values->Strings[2],Values->Strings[0],
    Values->Strings[3],Values->Strings[4],Values->Strings[5]);
    }
    delete tts;
}

void MagneticFieldDependence::loadData(TStringList * tts)
{
    /*
    Загрузка самих данных.
    */
    const std::string delimiterTab = "\t";
    const std::string delimiterSpace = " ";
    const int numberOfColls=3;

    TSignal B;
    TSignal Hall;
    TSignal Resistance;
    TSignal temp;

    for(int i=0;i<tts->Count;++i) // по количеству строк
    {
        if(tts->Strings[i].IsEmpty()) // пустые строки пропускаем
            continue;
            
        loadDataHelper(temp,tts->Strings[i],delimiterTab);

        if(temp.size()!=numberOfColls)
        {          
        loadDataHelper(temp,tts->Strings[i],delimiterSpace);
        if(temp.size()!=numberOfColls)
            return;
        }

        B.push_back(temp[0]);
        Resistance.push_back(temp[1]);
        Hall.push_back(temp[2]);
    }

    setDependence(B.begin(),B.end(),Hall.begin(),Resistance.begin());
    //testB= new MagneticField(B.begin(),B.end());
    //testHall= new THallEffect(Hall.begin(),Hall.end());
}

void MagneticFieldDependence::SaveAllData(AnsiString FileName)
{
// ахтунг, это страшный костыль -
// там где эта функция вызывается задаются имена файлов
// Com Dir и Rev
// по ним я и определяю тип сигнала.!!!!
ParamsType pt;
bool isCombinedParams=false;

if(FileName[FileName.Length()]=='m')
    {
        pt=COMBINE;
        isCombinedParams=true;
    }
if(FileName[FileName.Length()]=='v')  pt=REVERSE;
if(FileName[FileName.Length()]=='r')  pt=DIRECT;

    saver->setParamsType(pt);
    saver->SaveData(CURRENT_DATA,&B,&HallEffect,&MagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(CURRENT_DATA,&B,&HallEffect,&MagnetoResistance,ALL_POINTS,FileName);

    if(FilteredB.size()!=0)
    {
        saver->SaveData(FILTERED_DATA,&FilteredB,&FilteredHallEffect,&FilteredMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
        saver->SaveData(FILTERED_DATA,&FilteredB,&FilteredHallEffect,&FilteredMagnetoResistance,ALL_POINTS,FileName);
    }
    //saver->SaveData(EXTRAPOLATED_DATA,&ExtrapolatedB,&ExtrapolatedHallEffect,&ExtrapolatedMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    if(ExtrapolatedB.size()!=0)
    saver->SaveData(EXTRAPOLATED_DATA,&ExtrapolatedB,&ExtrapolatedHallEffect,&ExtrapolatedMagnetoResistance,ALL_POINTS,FileName);

    if(sxx.size()!=0)
    {
        saver->SaveData(TENZOR_DATA,&AveragedB,&sxx,&sxy,POINTS_11,
            FileName+"R"+ filterParamsResistance->getFilterParams()+ "H"+ filterParamsHall->getFilterParams());
        saver->SaveData(TENZOR_DATA,&AveragedB,&sxx,&sxy,ALL_POINTS,
            FileName+"R"+ filterParamsResistance->getFilterParams()+ "H"+ filterParamsHall->getFilterParams());

        saver->SaveData(MOBILITY_DATA,&mobility,&holeConductivity,&electronConductivity,ALL_POINTS,FileName);
    }

    if (pt==COMBINE)
    {
        saver->SaveData(AVERAGED_DATA,&AveragedB,&AveragedHallEffect,&AveragedMagnetoResistance,ALL_POINTS,FileName);
    }

    if(MobilitySpectrumObj)
    {
        std::string t1=FileName.c_str();
        MobilitySpectrumObj->saveEigenValues(t1+"Eigen.txt");
        MobilitySpectrumObj->saveResults(t1+"MobSpecRes.txt");
        MobilitySpectrumObj->savePeakWeigth(t1+"PeakWeigth.txt");
    }
    saver->SaveSampleDescription(FileName);
}

//------------Подгонка данных-------------------------------------------------

void MagneticFieldDependence::averageData(TSignal & inY, TSignal &outY, FeatType featType, int centerOfSimmetry)
{
    /*
    Усредненный сигнал на данный момент существует лишь для положительного магнитного поля.
   
    */
    int size=inY.size();

    // Размер рассчитывается в зависимости от точки симметрии.
    outY.resize(std::min(centerOfSimmetry + 1,size - centerOfSimmetry));
    
    
        switch(featType)
        {
        case ODD_FEAT: // нечетная подгонка
        /*
        Нечетное усреднение
        имеем
        f(-x)=-f(x)
        Тогда среднее есть сумма двух одинаковых значений поделенная пополам.
        В итоге получаем:

        (f(-x)+(-f(x)))/2.0
        Т.е.
        (f(-x)-f(x))/2.0

        Но возникает вопрос со знаком.
        По идее сигнал сюда приходит комбинированный, а значит
        нулевой элемент соответствует индукции -2 Тл.
        Получается нам надо взять знак этого элемента и изменить на противоположный -
        это и будет тот знак, который нам так нужен.

        Если пытаться использовать элементы из середины - велика вероятность ошибки,
        т.к. из-за неоднородностей образца для нулевого магнитного поля идеально
        нулевых значений напряжения Холла получить не удаётся.
        Оно будет близким к нулю, при этом справа или слева - предсказать не получится.
        
        С магнитным полем это действует достаточно хорошо, но вот если речь идет
        о сигнале - возникают некоторые проблемы.
        Поэтому лучше брать знак той точки, которую усредняем. Выглядит логично.


        И вот я нашел ещё один косят - а что если функция не меняет знака?
        На всей протяженности знак одинаковый и тут понеслась....

        Так, мы симметричны относительно точки B=0 и некоторого значения сигнала,
        которое соответствует полю равному 0.
        Вопрос - как нам узнать это значение?
        В общем случае один из краев или центр входного сигнала должен быть
        близким к нулю, но он может быть близким к нему, но так его и не пересечь.

        Возникли проблемы с усреднением фильтрованного графика холловского напряжения.


        */
        for(int i=0;i<size/2+1;++i)
            {
                /*
                Есть такая точка - центр симметрии.
                Мы усредняем график относительно этого центра.
                Таким образом получается
                */

                if (centerOfSimmetry+i<size && centerOfSimmetry-i>=0) // дабы не вывалиться за границы
                {
                    outY[i]=(inY[centerOfSimmetry+i]-inY[centerOfSimmetry-i])/2.0;     

                }
                
                else // Пока значений хватает всё вроде как хорошо
                    // Однако когда они заканчиваются тут начинается простое переписывание оставшихся
                    // Это не очень правильно. Нужно экстраполировать.
                { 
                    /*
                        Нужно определить шаг.
                        Определить сколько точек нужно экстраполировать.
                        Видимо до 2+h Тл
                        Вызывать экстраполяцию.
                        Дописать результаты сюда.
                        Этот подход будет правильней. И без глюков с прыжками.
                    */               

                    /*
                    if (centerOfSimmetry-i<0 && centerOfSimmetry+i<size)
                    {
                        outY[i]=inY[centerOfSimmetry+i];
                    }
                    else
                    {
                        if (centerOfSimmetry-i>=0)
                        {
                            outY[i]=-inY[centerOfSimmetry-i];
                        }                   
                    }*/
                }
/*

            if( sign(inY[0]*sign(inY.back()))<0) // тогда если по краям знаки разные
                outY[i]=-sign(inY[i])*fabs((inY[i]-inY[size-1-i]))/2.0;
            else
                outY[i]=sign(inY[i])*fabs((inY[i]-inY[size-1-i]))/2.0;
*/
                //outY[i]=sign(inY[size-1-i])*fabs((inY[i]-inY[size-1-i]))/2.0;
                //outY[i]=-sign(inY[i])*fabs((inY[i]-inY[size-1-i]))/2.0;
                //outY[size-1-i]=-inY[i];
            }
            // Поскольку начинали с дальнего края - реверсим вектор.
      //  std::reverse(outY.begin(),outY.end());
            break;
        case EVEN_FEAT: // четная подгонка
        /*
        Так, теперь выясняется что и тут не всё так просто.
        Есть у нас четная симметрия.
        Относительно оси Y
        Сие означает, что для противоположных значений индукции магнитного поля значения данной функции равны
        f(-B)=f(B)
        Для обычных входных данных нет никаких проблем и эта простая формула работает.
        Но для фильтрованных сигналов, с увеличением длины фильтра возрастает погрешность - мы усредняем не те точки.
        Нужно усреднять относительно точки В=0
        Т.е. надо:
        1. найти точку В=0
        2. получить её индекс
        3. усреднить относительно её те точки что получится
        4. недостающие (для высоких значений магнитных полей, при длинном фильтре они экстраполируются) взять из исходного сигнала, без усреднения
        P.S. Не уверен насчет поведения недостающих точек, но попробовать стоит.
        5. Поскольку мы живем в мире реальных сигналов, то минимум сигнала (сопротивления) для четной подгонки запросто не совпадает 
        с нулевым значением магнитного поля.
        Для этого вводим новый параметр - индекс элемента, относительно которого строим симметрию.
    

        */

                
        //ShowMessage(FloatToStr(inY[min])+" "+IntToStr((int)min));
            
        for(int i=0;i<size/2+1;++i) // Элементов станет в два раза меньше.
        {
            if (centerOfSimmetry+i<size && centerOfSimmetry-i>=0) // дабы не вывалиться за границы
            {
                outY[i]=(inY[centerOfSimmetry-i]+inY[centerOfSimmetry+i])/2.0;
                if(fabs(outY[i])<0.000001)
                     ShowMessage("O No!!!! MagneticFieldDependence.cpp");     
            }
            /*
            else
            {                
                if (centerOfSimmetry-i<0 && centerOfSimmetry+i<size)
                {
                    outY[i]=inY[centerOfSimmetry+i];
                }
                else
                {
                    if (centerOfSimmetry-i>=0)
                    {
                        outY[i]=inY[centerOfSimmetry-i];
                    }                   
                }
            }*/
            //outY[i]=(inY[i]+inY[size-1-i])/2.0;
            //tempY[size-1-i]=tempY[i];

        }
        break;
    }

    
}

void MagneticFieldDependence::featData(DataKind dataKind)
{
    TSignal tempInB;
    TSignal tempInBHall;
    TSignal tempInBResistance;
    TSignal tempInHall;
    TSignal tempInResistance;

    // Получаем указатели на нужные нам данные.

    tempInBHall=(*getPointerBHall(dataKind));
    tempInBResistance=(*getPointerBResistance(dataKind));
    tempInB=(*getPointerB(dataKind));
    tempInHall=(*getPointerHall(dataKind));
    tempInResistance=(*getPointerMagnetoResistance(dataKind));


    
    if(tempInB.size()==0 || tempInBHall.size()==0 || tempInBResistance.size()==0)
    {
        //"Получен пустой массив данных!!!");
        return;
    }
    
    /*
    Ещё один этап весьма важной проверки - если сигнал не является Совмещенным, т.е. существует только для положительного или только для отрицательного поля
    То усреднять вообще говоря нечего.
    */
    if(indexOfElemClosestToValue(tempInBHall,0)<tempInBHall.size()/10  || indexOfElemClosestToValue(tempInBHall,0)==tempInBHall.size()-1)
    {
        AveragedBHall=tempInBHall;
        AveragedBMagnetoResistance=tempInBResistance;
        AveragedB=tempInB;
        AveragedHallEffect=tempInHall;
        AveragedMagnetoResistance=tempInResistance;

        return;
    }

    averageData(tempInBHall,AveragedBHall,ODD_FEAT,indexOfElemClosestToValue(tempInBHall,0));
    averageData(tempInBResistance,AveragedBMagnetoResistance,ODD_FEAT,indexOfElemClosestToValue(tempInBResistance,0));

    averageData(tempInB,AveragedB,ODD_FEAT,indexOfElemClosestToValue(tempInB,0));
    averageData(tempInHall,AveragedHallEffect,ODD_FEAT,indexOfElemClosestToValue(tempInBHall,0));
    averageData(tempInResistance,AveragedMagnetoResistance,EVEN_FEAT,indexOfElemClosestToValue(tempInBResistance,0));
}

void MagneticFieldDependence::GetEqualNumberOfPoints(TSignal & B,
TSignal & BHall,TSignal & BRes, TSignal & Hall,
TSignal & Res)
{
    MyDataType left, right;
    size_t length;

    TSignal tempB;
    TSignal tempBHall;
    TSignal tempBRes;

    TSignal tempHall;
    TSignal tempResistance;
    
    std::vector<size_t> s;
    s.push_back(B.size());
    s.push_back(BHall.size());
    s.push_back(BRes.size());
    s.push_back(Hall.size());
    s.push_back(Res.size());

    std::sort(s.begin(), s.end());

    if (s[0]==0)
    {
        return;
    }

    length=s[0];
    left=B[0];
    right=B.back();

    // это должно уравнивать количество точек, после фильтрации с разной длиной.
    thiningSignal(BHall, Hall, tempBHall, tempHall, left, right, length);
    thiningSignal(BRes, Res, tempBRes, tempResistance, left, right, length);
    
    BHall.clear();
    Hall.clear();
    BRes.clear();
    Res.clear();
    B.clear();
    
    BHall=tempBHall;
    Hall=tempHall;
    BRes=tempBRes;
    Res=tempResistance;
    B=BHall;
}
//-------------------------------------------------------------------------
void MagneticFieldDependence::blockfilterData()
{
    /*
    Блочный фильтр.
    Весь сигнал разбивается на блоки и фильтруется отдельно.
    После чего объединяется.
    На данный момент работает не очень хорошо.
    */
clearFilteredParams();

    GetEqualNumberOfPoints(B,BHall,BMagnetoResistance,
    HallEffect,MagnetoResistance);

    BlockLowBandFilter(BHall,HallEffect,FilteredBHall,FilteredHallEffect,
        (*filterParamsHall).filterLength,
        (*filterParamsHall).SamplingFrequecy,
        (*filterParamsHall).BandwidthFrequency,
        (*filterParamsHall).AttenuationFrequency,
        200);
    BlockLowBandFilter(BMagnetoResistance,MagnetoResistance,
         FilteredBMagnetoResistance,FilteredMagnetoResistance,
        (*filterParamsResistance).filterLength,
        (*filterParamsResistance).SamplingFrequecy,
        (*filterParamsResistance).BandwidthFrequency,
        (*filterParamsResistance).AttenuationFrequency,
        200);

    FilteredB=FilteredBHall;
    GetEqualNumberOfPoints(FilteredB,FilteredBHall,FilteredBMagnetoResistance,
    FilteredHallEffect,FilteredMagnetoResistance);
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData()
{
    // очищаем результаты предыдущей фильтрации
    clearFilteredParams();
    // уравниваем количество точек
    GetEqualNumberOfPoints(B,BHall,BMagnetoResistance,
    HallEffect,MagnetoResistance);

    DataKind dataKind;
    dataKind=CURRENT_DATA;

    if( B[0]<-1.0 && B.back() >1.0) // Если это комбинированный сигнал
    {
     /*   Dependence h(BHall, HallEffect,true);
        Dependence mr(BMagnetoResistance, MagnetoResistance,false);
        h.Average();
        mr.Average();
        //HallEffect.clear();
        //BMagnetoResistance.clear();
        //BHall.clear();
        TSignal temp(h.getY()->begin(),h.getY()->end());
        TSignal tempB(h.getX()->begin(),h.getX()->end());
        //HallEffect=temp;
        //BMagnetoResistance=tempB;
        //MagnetoResistance.clear();
        TSignal temp2(mr.getY()->begin(),mr.getY()->end());
        TSignal tempB2(mr.getX()->begin(),mr.getX()->end());
        //MagnetoResistance=temp2;
        //BHall=tempB2;*/
    featData(CURRENT_DATA);// его надо усреднить

      /*  AveragedBHall=tempB;
        AveragedBMagnetoResistance=tempB2;
        AveragedB=tempB; // надо потом подумать над компромисным вариантом, либо отказаться от него вообще
        AveragedHallEffect=temp;
        AveragedMagnetoResistance=temp2;
*/
    // и фильтровать будем усредненный сигнал.
    dataKind = AVERAGED_DATA;
    // уравниваем количество точек
    GetEqualNumberOfPoints(AveragedB,AveragedBHall,AveragedBMagnetoResistance,
    AveragedHallEffect,AveragedMagnetoResistance);
	}

    filterDataHelper((*filterParamsHall),HALL_EFFECT,dataKind);

    filterDataHelper((*filterParamsResistance),MAGNETORESISTANCE,dataKind);
    FilteredB=FilteredBHall;

    GetEqualNumberOfPoints(FilteredB,FilteredBHall,FilteredBMagnetoResistance,
    FilteredHallEffect,FilteredMagnetoResistance);

}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData(FilterParams &fPHall, FilterParams &fPResistance)
{
    /*
    Вызывает предыдущую, после установки новых параметров фильтрации.
    */
    setFilterParamsHall(fPHall.SamplingFrequecy, fPHall.BandwidthFrequency, fPHall.AttenuationFrequency, fPHall.filterLength);
    setFilterParamsResistance(fPResistance.SamplingFrequecy, fPResistance.BandwidthFrequency, fPResistance.AttenuationFrequency, fPResistance.filterLength);
    filterData();

}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    SignalType dependenceType, DataKind dataKind)
{
    /*
    Так, функция у нас по заданному типу зависимостей выполняет фильтрацию.
    Стало быть есть указатели на 
    Магнитное поле и сигнал
    */

    // Указатели на поле и сигнал.
    TSignal * inB;
    TSignal * inSignal;

    // Заботу о том комбинированный сигнал или нет, перекладываем на вышестоящую функцию.
    // Получаем нужные зависимости, с учетом типа и вида сигнала.
    switch(dependenceType)
    {
    case HALL_EFFECT:
        inB= getPointerBHall(dataKind);
        inSignal = getPointerHall(dataKind);
        break;

    case MAGNETORESISTANCE:
        inB= getPointerBResistance(dataKind);
        inSignal = getPointerMagnetoResistance(dataKind);
        break;

    default:
        break;
    }

    unsigned int NumberOfPoints; // Количество точек, пригодится для скорости.

    if(inSignal->size()!=inB->size())
    {
        ShowMessage("Количество точек магнитного поля и сигнала не совпадает! filterdataHelper.");
        return;
    }

    NumberOfPoints=inSignal->size();

    if(NumberOfPoints==0)
    {
        ShowMessage("Получен пустой массив данных! filterdataHelper.");
        return;
    }

    if(2*NumberOfPoints<fP.filterLength)
    {
        ShowMessage("Слишком длинный фильтр! filterdataHelper.");
        return;
    }

    // В эти массивы будут достраиваться данные для отрицательных магнитных полей.
    // Это очень мило, а если это сигнал для отрицательного магнитного поля?
    // То теоретически достраивается положительная часть.
    // Надо пофиксить тут комменты на адекватные действительности.
    TSignal tempInB(2*NumberOfPoints);
    TSignal tempInSignal(2*NumberOfPoints);

    TSignal tempOutB(2*NumberOfPoints);
    TSignal tempOutSignal(2*NumberOfPoints);

    switch(dependenceType)
    {
    case HALL_EFFECT:


    // формируем сигнал для фильтра.
    // достраивая его в отрицательные магнитные поля.
    for (unsigned int i = 0; i < NumberOfPoints; ++i)
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
        tempInSignal[i]=-(*inSignal)[NumberOfPoints-i-1]+2*(*inSignal)[0];
        tempInB[i]=-(*inB)[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=(*inSignal)[i];
        tempInB[i+NumberOfPoints]=(*inB)[i];        
    }
    break;
    case MAGNETORESISTANCE:

    

    for (unsigned int i = 0; i < NumberOfPoints; ++i)
    {
        tempInSignal[i]=(*inSignal)[NumberOfPoints-i-1];   // чет
        tempInB[i]=-(*inB)[NumberOfPoints-i-1];        
        tempInSignal[i+NumberOfPoints]=(*inSignal)[i];
        tempInB[i+NumberOfPoints]=(*inB)[i];        
    }
    break;

    default:
        break;
    }

    /*
    В случае отрицательного магнитного поля надо инвертировать порядок элементов
    Потому что впереди выстраиваются значения для положительного магнитного поля.
    */
    if(tempInB[0]>1.0)
    {
        std::reverse(tempInB.begin(),tempInB.end());
        std::reverse(tempInSignal.begin(),tempInSignal.end());
    }
    // фильтруем 
    TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                fP.filterLength,fP.SamplingFrequecy,fP.BandwidthFrequency,fP.AttenuationFrequency);


    // Размер внутри фильтра меняется, в зависимости от длины фильтра.
    NumberOfPoints=tempOutB.size();
    for(unsigned int i=fP.filterLength;i<NumberOfPoints;++i)
    {
        
    switch(dependenceType)
    {
    case HALL_EFFECT:
        FilteredHallEffect.push_back(tempOutSignal[i]);
        FilteredBHall.push_back(tempOutB[i]);
        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance.push_back(tempOutSignal[i]);
        FilteredBMagnetoResistance.push_back(tempOutB[i]);
        break;
    default:
        break;
    }
    }
}

//-------------------------------------------------------------------------------

bool MagneticFieldDependence::extrapolateData( DataKind dataKind, const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect)
{
	/*
	Раз уж пошли такие баги - буду разбираться комментированием кода.
	Имеются три указателя на экстраполируемые данные.
	В зависимости от того какие нужно, это будут либо фильтрованные, либо усредненные данные.
	Если вдруг что-то пошло не так - мы убегаем.
	Экстраполяция проходит только по части сигнала - на данный момент по последним двум третям.

	Потом проходит увеличение веса точки 0, 0 для холловского напряжения.

	Выделяем память для коэффициентов и вызываем функцию поиска решения ( curveFittingUniversal ).
	Эта функция кстати тоже не безгрешна, но там баги искать это совсем страшное зло.

	Магнитное поле заполняется заданным шагом (кстати, возможно шаг нужно рассчитывать как-то по другому), начиная с последнего текущего значения.

	*/
    bool returnValue=true;

    TSignal * BHallToExtrapolate=NULL;
    TSignal * BResToExtrapolate=NULL;
    TSignal * BToExtrapolate= NULL;
    TSignal * HallToExtrapolate= NULL;
    TSignal * ResistanceToExtrapolate= NULL;

    if (dataKind==FILTERED_DATA)
    {
        BToExtrapolate= & FilteredB;
        BHallToExtrapolate=& FilteredBHall;
        BResToExtrapolate=& FilteredBMagnetoResistance;
        HallToExtrapolate= & FilteredHallEffect;
        ResistanceToExtrapolate= & FilteredMagnetoResistance;
    }
    if(dataKind ==AVERAGED_DATA)
    {
        BToExtrapolate= & AveragedB;
        BHallToExtrapolate=& AveragedBHall;
        BResToExtrapolate=& AveragedBMagnetoResistance;
        HallToExtrapolate= & AveragedHallEffect;
        ResistanceToExtrapolate= & AveragedMagnetoResistance;
    }   
    // Если вдруг что-то пошло не так - мы убегаем.
    if(BToExtrapolate==NULL)
    {
    	return false;
    } 

    TSignal inBHall;
    TSignal inBMagnetoResistance;

    TSignal inHallEffect;
    TSignal inMagnetoResistance;

    /*
    Экстраполяция данных ведется по последней четверти фильтрованных значений.

    Вот тут собственно и косяк.
    Экстраполировать нужно либо по фильтрованным, либо по усредненным значениям.
    */
    for (size_t i = (16*BToExtrapolate->size())/20; i < BToExtrapolate->size(); ++i)
    {
        inBHall.push_back( (*BToExtrapolate)[i]);
        inBMagnetoResistance.push_back( (*BToExtrapolate)[i]);
        inHallEffect.push_back( (*HallToExtrapolate)[i]);
        inMagnetoResistance.push_back( (*ResistanceToExtrapolate)[i]);
    }

    unsigned int NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("Количество точек равно нулю! Я не хочу делить на ноль:) extrapolateData");
    return false;
    }

    MyDataType h=(max_abs_elem(inBHall)- min_elem(inBHall)) /NumberOfPoints;

    TSignal koefMagnetoResistance(polinomPowForMagnetoResistance+1);
    TSignal koefHallEffect(polinomPowForHallEffect+1);

    if(!curveFittingUniversal(&inBMagnetoResistance,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance))
    return false;
    if(!curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,polinomPowForHallEffect))
    return false;


    TSignal newB;
    TSignal newBHall;
    TSignal newBRes;
    TSignal newHallEffect;
    TSignal newMagnetoResistance;

    // Допустим нам всегда надо доводить поле до rightFieldBoundary
    long double rightFieldBoundary = 2.5; // Тл
    // Тогда нам надо расчитать кол-во точек, исходя из среднего шага.
    if (rightFieldBoundary>(BToExtrapolate->back()-h))
    {
        NumberOfPoints=(rightFieldBoundary-(BToExtrapolate->back()-h))/h+1;
    }
    

    newB.push_back(BToExtrapolate->back()-h); // заполняем магнитное поле.
    newBHall.push_back(BHallToExtrapolate->back()-h);
    newBRes.push_back(BResToExtrapolate->back()-h);
    for (unsigned int i = 1; i<NumberOfPoints; ++i) {
        newB.push_back(newB[i-1]+h);
    }
    for (unsigned int i = 1; i<inBHall.size(); ++i) {
        newBHall.push_back(newBHall[i-1]+h);
    }
    for (unsigned int i = 1; i<inBMagnetoResistance.size(); ++i) {
        newBRes.push_back(newBRes[i-1]+h);
    }

    // вычисляем экстраполированные зависимости.

    calculatePolinomByKoef(newBRes,koefMagnetoResistance,newMagnetoResistance);
    calculatePolinomByKoef(newBHall,koefHallEffect,newHallEffect);

    ExtrapolatedB.clear();
    ExtrapolatedMagnetoResistance.clear();
    ExtrapolatedHallEffect.clear();

    ExtrapolatedB=newB;
    ExtrapolatedMagnetoResistance=newMagnetoResistance;
    ExtrapolatedHallEffect=newHallEffect;
    
    //----------А вот тут прикручиваем недостающий кусочек в сигналы----
    
    unsigned int i=0;
    while(i<BToExtrapolate->size() && newB[i]<=BToExtrapolate->back())
    ++i; // находим элемент на котором заканчивается фильтрованное магнитное поле.

    for(unsigned int j=i;j<NumberOfPoints;j++)
    {     // в конце дописываем экстраполированные значения.
        BToExtrapolate->push_back(newB[j]);
        BHallToExtrapolate->push_back(newBHall[j]);
        BResToExtrapolate->push_back(newBRes[j]);
        ResistanceToExtrapolate->push_back(newMagnetoResistance[j]);
        HallToExtrapolate->push_back(newHallEffect[j]);
    }
    //------------------------------------------------------------------
    inBHall.clear();
    inBMagnetoResistance.clear();
    inHallEffect.clear();
    inMagnetoResistance.clear();


    // Эта часть повторяется ради экстраполяции слева
    /*
    Экстраполяция данных ведется по последней четверти фильтрованных значений.

    Вот тут собственно и косяк.
    Экстраполировать нужно либо по фильтрованным, либо по усредненным значениям.
    */
    
    for (size_t i = 0; i < 4*BToExtrapolate->size()/20; ++i)
    {
        inBHall.push_back( (*BToExtrapolate)[i]);
        inBMagnetoResistance.push_back( (*BToExtrapolate)[i]);
        inHallEffect.push_back( (*HallToExtrapolate)[i]);
        inMagnetoResistance.push_back( (*ResistanceToExtrapolate)[i]);
    }

    NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("Количество точек равно нулю! Я не хочу делить на ноль:) extrapolateData");
    return false;
    }

    h=(max_elem(inBHall)- min_elem(inBHall)) /NumberOfPoints;

    koefMagnetoResistance.clear();
    koefHallEffect.clear();

    koefMagnetoResistance.resize(polinomPowForMagnetoResistance+1);
    koefHallEffect.resize(polinomPowForHallEffect+1);

    if(!curveFittingUniversal(&inBMagnetoResistance,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance))
    return false;
    if(!curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,polinomPowForHallEffect))
    return false;

    newB.clear();
    newBHall.clear();
    newBRes.clear();
    newHallEffect.clear();
    newMagnetoResistance.clear();





    // Допустим нам всегда надо доводить поле до rightFieldBoundary Тл
    // Тогда нам надо расчитать кол-во точек, исходя из среднего шага.
    if (rightFieldBoundary>fabs(BToExtrapolate->front()+h))
    {
        NumberOfPoints=(rightFieldBoundary+(BToExtrapolate->front()+h))/h+1;
    }

    newB.push_back(BToExtrapolate->front()+h); // заполняем магнитное поле.
    newBHall.push_back(BHallToExtrapolate->front()+h);
    newBRes.push_back(BResToExtrapolate->front()+h);
    for (unsigned int i = 1; i<NumberOfPoints; ++i) {
        newB.push_back(newB[i-1]-h);
    }
    for (unsigned int i = 1; i<NumberOfPoints; ++i) {
        newBHall.push_back(newBHall[i-1]-h);
    }
    for (unsigned int i = 1; i<NumberOfPoints; ++i) {
        newBRes.push_back(newBRes[i-1]-h);
    }

    // вычисляем экстраполированные зависимости.
    calculatePolinomByKoef(newBRes,koefMagnetoResistance,newMagnetoResistance);
    calculatePolinomByKoef(newBHall,koefHallEffect,newHallEffect);


   // ExtrapolatedB=newB;
   // ExtrapolatedMagnetoResistance=newMagnetoResistance;
   // ExtrapolatedHallEffect=newHallEffect;

    //----------А вот тут прикручиваем недостающий кусочек в сигналы----
    std::reverse(newB.begin(),newB.end());
    std::reverse(newBHall.begin(),newBHall.end());
    std::reverse(newBRes.begin(),newBRes.end());
    std::reverse(newHallEffect.begin(),newHallEffect.end());
    std::reverse(newMagnetoResistance.begin(),newMagnetoResistance.end());

    //i=0;
    //while(i<NumberOfPoints && newB[i]<=BToExtrapolate->front())
    //++i; // находим элемент на котором начинается фильтрованное магнитное поле.

    for(unsigned int k=0;k<BToExtrapolate->size();++k)
    {     // в конце дописываем экстраполированные значения.
        newB.push_back(BToExtrapolate->operator[](k));
        newBHall.push_back(BHallToExtrapolate->operator[](k));
        newBRes.push_back(BResToExtrapolate->operator[](k));
        newHallEffect.push_back(HallToExtrapolate->operator[](k));
        newMagnetoResistance.push_back(ResistanceToExtrapolate->operator[](k));
    }

    BToExtrapolate->clear();
    BHallToExtrapolate->clear();
    BResToExtrapolate->clear();
    ResistanceToExtrapolate->clear();
    HallToExtrapolate->clear();

    for (int i = 0; i < newB.size(); ++i)
    {
        BToExtrapolate->push_back(newB[i]);
        BHallToExtrapolate->push_back(newBHall[i]);
        BResToExtrapolate->push_back(newBRes[i]);
        ResistanceToExtrapolate->push_back(newMagnetoResistance[i]);
        HallToExtrapolate->push_back(newHallEffect[i]);
    }
        
    /* TStringList * tsl= new TStringList;
    for (int i=0; i<FilteredMagnetoResistance.size(); ++i)
    {
        tsl->Add(FloatToStr(FilteredB[i]) + "\t"+ FloatToStr(FilteredMagnetoResistance[i]));
    }
    tsl->SaveToFile("E:\\Дела\\Институт физики полупроводников\\Lcard\\MCTParamsExplorer\\filterOutPutAftExtr.txt");
*/

return returnValue;   
}

//-------------------------------------------------------------------------------
void MagneticFieldDependence::multiplyB(DataKind dataKind)
{
    TSignal * temp;
    temp=getPointerB(dataKind);

    TSignal::iterator pos;
    for(pos=temp->begin();pos!=temp->end();++pos)
    *pos*=10;
}  
//-------------------------------------------------------------------------------
void MagneticFieldDependence::multiplySignal(SignalType s, MyDataType x)
{
TSignal * p;
if(s==HALL_EFFECT) p=&HallEffect;
if(s==MAGNETORESISTANCE) p=&MagnetoResistance;
if(p!=INVALID_HANDLE_VALUE)
{
    TSignal::iterator begin=p->begin();
    TSignal::iterator end=p->end();

    for(;begin!=end;++begin)
        *begin*=x;
}
}

//-------------------------------------------------------------------------------
inline void MagneticFieldDependence::ReplaceDotsToComma(std::string &in, std::string & out)
{
    unsigned int findIndex=0;
	std::string s=in;
	std::string strToReplaceWhich=","; // на что меняем
	std::string strToSearch=".";   // что ищем
	while ((findIndex=s.find(strToSearch,findIndex))!=std::string::npos)
    {
        s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),
        strToReplaceWhich.begin(),strToReplaceWhich.end());
    } 
    out=s;
}
//-------------------------------------------------------------------------------*/
bool MagneticFieldDependence::constructPlotFromTwoMassive(SignalType pt, DataKind dk,TLineSeries* s,TColor color)
{
    TSignal * pointToX=0; // указатели на выводимые данные
    TSignal * pointToY=0;
	s->Clear(); // чистим график.
    switch(pt) // определяем что выводить.
    {
    case HALL_EFFECT:
        pointToX=getPointerBHall(dk);
        pointToY=getPointerHall(dk);
        break;
    case MAGNETORESISTANCE:
        pointToX=getPointerBResistance(dk);
        pointToY=getPointerMagnetoResistance(dk);
        break;
    case SXX:
        pointToX=getPointerB(dk);
        pointToY=getPointerSxx(dk);
        break;

    case SXY:
        pointToX=getPointerB(dk);
        pointToY=getPointerSxy(dk);
        break;
    default:
        break;
    }
    unsigned int NumberOfPoints=pointToX->size();
    if(NumberOfPoints==0)
    {
        //ShowMessage("График пуст, строить нечего.");
        return false;
    }
    /*
    Из-за определенных тормозов с отображением мириадов точек было принято
    решение отображать только фиксированное количество.
    */
    TSignal::iterator posX;
    TSignal::iterator posY;
    // если точек в массиве меньше чем мы обчно выводим - выводим сколько есть.
    long double PointsToShow=(500>pointToX->size()?pointToX->size():500);

    if (pointToX->size() != pointToY->size())
    {
    // ShowMessage("Разное количество точек в массивах! Где-то до этого есть косяк");
    return false;
    }

    unsigned int shag = roundM(static_cast<long double> (pointToX->size()) / PointsToShow,0);
    if(shag<1) shag = 1;
    for (unsigned int i=0, j=0;j<PointsToShow && i<pointToX->size() && i<pointToY->size()  ;j++,i+=shag)
    {
        s->AddXY((*pointToX)[i],(*pointToY)[i],"",color);
    }
    return true;	
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::constructPlotFromOneMassive(SignalType p,TLineSeries* s,TColor color)
{
    TSignal * temp;
	s->Clear();
    switch(p)
    {
    case MAGNETIC_FIELD:
        temp=&B;
        break;
    case MAGNETIC_FIELD_F:
        temp=&FilteredB;
        break;
    case HALL_EFFECT:
        temp=&HallEffect;
        break;
    case MAGNETORESISTANCE:
        temp=&MagnetoResistance;
        break;
    default:
        break;
    }
    unsigned int NumberOfPoints=temp->size();
	for (unsigned int i = 0; i < NumberOfPoints; ++i)
	{
		s->AddY((*temp)[i],"",color);
	} 
}

//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsHall(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    filterParamsHall->setFilterParams(StrToFloat(samplingFrequecy), StrToFloat(bandwidthFrequency), StrToFloat(attenuationFrequency), StrToInt(length));
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsHall(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    filterParamsHall->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsResistance(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    filterParamsResistance->setFilterParams(StrToFloat(samplingFrequecy), StrToFloat(bandwidthFrequency), StrToFloat(attenuationFrequency), StrToInt(length));
    return true;
}
//-------------------------------------------------------------------------------
bool MagneticFieldDependence::setFilterParamsResistance(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    filterParamsResistance->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
    return true;
}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::setRoundNeeded(bool needRound)
{
    saver->setRoundNeeded(needRound);
}

//-------------------------------------------------------------------------------


void MagneticFieldDependence::setDependence(TSignal::iterator beginB, 
        TSignal::iterator endB, TSignal::iterator beginHall, 
        TSignal::iterator beginResistance)
{
    clearCurrentParams();

    for ( ; beginB != endB; ++beginB, ++beginResistance, ++beginHall)
    {
        B.push_back(*beginB);
        MagnetoResistance.push_back(*beginResistance);
        HallEffect.push_back(*beginHall); 
    }
    if(fabs(max_abs_elem(B))<0.5)
    {
        multiplyB(CURRENT_DATA);
    }
    BHall=B;
    BMagnetoResistance=B;


    filterData();
    extrapolateData(FILTERED_DATA,PowPolinomRes,PowPolinomHall);
    // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.      
}

//---------------------------------------------------------------------------------

void MagneticFieldDependence::getSplittedDataFromADC()
{
    TwoDimensionalContainer * tempData1=adc->getSplittedData(1);

    TwoDimensionalContainer & tempData(*tempData1);
    clearCurrentParams();
             
    B=tempData[2];
    HallEffect=tempData[0]; // последовательность закреплена и не важна.
    MagnetoResistance=tempData[1];

    adc->dataisntNeeded();
    // при смене каналов на вкладке настроек - эти настройки можно не трогать.
    // программа сама разберется, т.к. АЦП возвращает данные согласно контрольной таблице:).
    
    BHall=B;
    BMagnetoResistance=B;

    multiplyB(CURRENT_DATA);
    filterData();
    //extrapolateData(4,4); // магические числа, степени полиномов для экстраполяции по умолчанию.
    // в перспективе степень будет зависеть от температуры и возможно чего-нибудь ещё.
}

void MagneticFieldDependence::setSampleDescription(AnsiString Temperature, AnsiString Current, AnsiString SampleInventoryNumber,
    AnsiString length, AnsiString width, AnsiString Thickness)
{
    saver->setSampleDescription(Temperature, Current, SampleInventoryNumber, length, width, Thickness);
}
//-------------------------------------------------------------------------------

FilterParams const * MagneticFieldDependence::getFilterParamsHall()
{
    return filterParamsHall;
}
//------------------------------------------------------------------------------
FilterParams const * MagneticFieldDependence::getFilterParamsResistance()
{
    return filterParamsResistance;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getB()
{
    return &B;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getFilteredB()
{
    return &FilteredB;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getExtrapolatedB()
{
    return &ExtrapolatedB;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getHallEffect()
{
    return &HallEffect;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getMagnetoResistance()
{
    return &MagnetoResistance;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getFilteredHallEffect()
{
    return &FilteredHallEffect;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getFilteredMagnetoResistance()
{
    return &FilteredMagnetoResistance;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getExtrapolatedHallEffect()
{
    return &ExtrapolatedHallEffect;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getExtrapolatedMagnetoResistance()
{
    return &ExtrapolatedMagnetoResistance;
}
//-------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getSxx()
{
    return &sxx;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getSxy()
{
    return &sxy;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getAveragedB()
{
    return &AveragedB;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getRh_eff()
{
    return &Rh_eff;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getS_eff()
{
    return &s_eff;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getMobility()
{
    return &mobility;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getHoleConductivity()
{
    return &holeConductivity;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getElectronConductivity()
{
    return &electronConductivity;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getHoleConcentration()
{
    return &holeConcentration;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getHoleMobility()
{
    return &holeMobility;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getElectronConcentration()
{
    return &electronConcentration;
}
//----------------------------------------------------------------------------------
TSignal const * MagneticFieldDependence::getElectronMobility()
{
    return &electronMobility;
}
//----------------------------------------------------------------------------------

void MagneticFieldDependence::calculateEffectiveParamsFromSignals()
{
    /*
    Расчет производится по усредненным значениям.
    */
    if (AveragedB.size() && AveragedMagnetoResistance.size() && AveragedHallEffect.size())
    {
        AnsiString Current;
        AnsiString SampleLength;
        AnsiString SampleWidth;
        AnsiString SampleThickness;

        saver->getSampleDescription(NULL, Current, NULL, SampleLength, SampleWidth, SampleThickness);

        MyDataType I=StrToFloat(Current)*1E-6;
        MyDataType SLength=StrToFloat(SampleLength)*1E-3;
        MyDataType SWidth=StrToFloat(SampleWidth)*1E-3;
        MyDataType SThickness=StrToFloat(SampleThickness)*1E-6;

        unsigned int NumberOfPoints = AveragedB.size();
        s_eff.resize(NumberOfPoints);
        Rh_eff.resize(NumberOfPoints);

        for (unsigned int i = 0; i < NumberOfPoints ; ++i)
        {
            if(fabs(AveragedMagnetoResistance[i])<THEALMOSTZERO)
                s_eff[i]=0;
            else
            {
                s_eff[i]=SLength/SWidth/SThickness*I/AveragedMagnetoResistance[i];
            }
            if(AveragedB[i] == 0)
                Rh_eff[i] = 0;
            else
                Rh_eff[i]=SThickness*AveragedHallEffect[i]/I;
        }
    }
}

//--------------------------------------------------------------------------------

void MagneticFieldDependence::calculateTenzorFromEffectiveParams()
{
    if (s_eff.size() && Rh_eff.size() && s_eff.size() == Rh_eff.size())
    {
        unsigned int NumberOfPoints = s_eff.size();
        sxx.resize(NumberOfPoints);
        sxy.resize(NumberOfPoints);

        for (unsigned int i = 0; i < NumberOfPoints ; ++i)
        {
            sxx[i]=s_eff[i]/
                (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1.0);
            sxy[i]=s_eff[i]*s_eff[i]*Rh_eff[i]/
               (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1.0);
        }
    }
}
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerB(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &B;
    case FILTERED_DATA:
        return &FilteredB;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedB;
    case AVERAGED_DATA:
        return &AveragedB;
    default:
        break;
    }
    return NULL;
}

TSignal * MagneticFieldDependence::getPointerBHall(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &BHall;
    case FILTERED_DATA:
        return &FilteredBHall;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedBHall;
    case AVERAGED_DATA:
        return &AveragedBHall;
    default:
        break;
    }
    return NULL;
}

TSignal * MagneticFieldDependence::getPointerBResistance(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &BMagnetoResistance;
    case FILTERED_DATA:
        return &FilteredBMagnetoResistance;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedBMagnetoResistance;
    case AVERAGED_DATA:
        return &AveragedBMagnetoResistance;
    default:
        break;
    }
    return NULL;
}
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerHall(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &HallEffect;
    case FILTERED_DATA:
        return &FilteredHallEffect;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedHallEffect;
    case AVERAGED_DATA:
        return &AveragedHallEffect;
    default:
        break;
    }
    return NULL;
}
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerMagnetoResistance(DataKind dataKind)
{
    switch(dataKind)
    {
    case CURRENT_DATA:
        return &MagnetoResistance;
    case FILTERED_DATA:
        return &FilteredMagnetoResistance;
    case EXTRAPOLATED_DATA:
        return &ExtrapolatedMagnetoResistance;
    case AVERAGED_DATA:
        return &AveragedMagnetoResistance;
    default:
        break;
    }
    return NULL;
}
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerSxx(DataKind dataKind)
{
    return &sxx;
}
//-------------------------------------------------------------------------
TSignal * MagneticFieldDependence::getPointerSxy(DataKind dataKind)
{
    return &sxy;
}
//-------------------------------------------------------------------------

void MagneticFieldDependence::calculateTenzor(DataKind dataKind)
{
    featData(dataKind);
    calculateEffectiveParamsFromSignals();
    calculateTenzorFromEffectiveParams();
}

//-------------------------------------------------------------------------
void  MagneticFieldDependence::cutData(DataKind dataKind)
{
    // А если тут ничего не происходит... зачем я её делал?))))
    // оставляет только положительные значения магнитного поля
    // Хм, надо будет разобраться надо ли оно нам. По идее - её нужно бы использовать для обработки
    // сигналов положительных и отрицательных.
   ;
}

void MagneticFieldDependence::clearCurrentParams()
{
    B.clear();
    BHall.clear();
    BMagnetoResistance.clear();
    HallEffect.clear();
    MagnetoResistance.clear();    
}

void MagneticFieldDependence::clearFilteredParams()
{
    FilteredB.clear();
    FilteredBHall.clear();
    FilteredBMagnetoResistance.clear();
    FilteredHallEffect.clear();
    FilteredMagnetoResistance.clear();
}

void MagneticFieldDependence::setParamsType(ParamsType pt)
{
    paramsType=pt;
}

void MagneticFieldDependence::shiftCurve(DataKind dataKind,SignalType dependenceType,MyDataType shiftValue,MyDataType leftBound, MyDataType rightBound)
{
    TSignal * pointToY=0;
    TSignal * pointToX=0;
    switch (dependenceType)
    {
        case HALL_EFFECT:
            pointToY=getPointerHall(dataKind);
            pointToX=getPointerB(dataKind);
            break;
        case MAGNETORESISTANCE:
            pointToY=getPointerMagnetoResistance(dataKind);
            pointToX=getPointerB(dataKind);
            break;
        default:
        break;
    }
    if (pointToY==0)
    {
        return;
    }

    for (TSignal::iterator i = pointToY->begin(), j=pointToX->begin(); i != pointToY->end() && j!=pointToX->end(); ++i,++j)
    {
        if (*j>=leftBound && *j<=rightBound)
        {
            *i+=shiftValue;    
        }        
    }
}

void MagneticFieldDependence::setChannelsInfo(channelsInfo & cI)
{
    chanInfo=cI;    
}

void MagneticFieldDependence::rearrangeSignal()
{
    HallEffect.swap(MagnetoResistance);
}

void MagneticFieldDependence::setExtrapolateParams(int powPolinowHall,int powPolinomRes)
{
    PowPolinomRes=powPolinowHall;
    PowPolinomHall=powPolinomRes;
}

// Спектр подвижности
bool MagneticFieldDependence::calculateMobilitySpectrum()
{
    TSignal nB;
    TSignal nSxx;
    TSignal nSxy;

    thiningSignal(AveragedB, sxx, nB, nSxx,0, 2, 11);
    thiningSignal(AveragedB, sxy, nB, nSxy,0, 2, 11);

    if (MobilitySpectrumObj!=NULL)
    {
        delete MobilitySpectrumObj;
    }

    MobilitySpectrumObj= new mobilitySpectrum(nB,nSxx,nSxy,nB.size());
    MobilitySpectrumObj->getResults(mobility, holeConductivity, electronConductivity);
    MobilitySpectrumObj->getExtremums(holeConcentration, holeMobility, electronConcentration, electronMobility);
    return true;
}
// Многозонная подгонка
bool MagneticFieldDependence::runMultiCarrierFit(long double VesGxx, long double VesGxy)
{

    // важный вопрос - в каком порядке нужно помещать сюда данные.
    // Порядок такой: подвижность электронов, легких дырок и тяжелых дырок.
    // Далее - концентрации в том же порядке.
    std::vector<long double> ExactBound; // сюда нужны пики из спектра

    ExactBound.push_back(-electronMobility[0]); // Подвижность электронов
    ExactBound.push_back(holeMobility[1]); // Подвижность легких дырок
    ExactBound.push_back(holeMobility[0]); // Подвижность тяжелых дырок

    ExactBound.push_back(-electronConcentration[0]); // Концентрация электронов
    ExactBound.push_back(holeConcentration[1]); // Концентрация легких дырок
    ExactBound.push_back(holeConcentration[0]); // Концентрация тяжелых дырок


    /* Необходимо проверить все ли границы установлены корректно.
    Если подвижность пика равна минимальной исследуемой подвижности - пик не был найден.
    В этом случае необходимо задать границы поиска исходя из другой априорной информации.*/

    /*
    В теории на данный момент спектры не имеющие пика не допускаются к расчету.
    */
    long double leftBoundMobility=0.001;

    if (ExactBound[0]==leftBoundMobility ||
        ExactBound[1]==leftBoundMobility ||
        ExactBound[2]==leftBoundMobility)
    {
        /* code */
    }

    // Нужно выяснить отчего зависит разброс.
    // Полагаю это какие-то коэффициенты
    std::vector<long double> coef;

    coef.push_back(0.8); // Подвижность электронов
    coef.push_back(0.2); // Подвижность легких дырок
    coef.push_back(0.1); // Подвижность тяжелых дырок
    coef.push_back(0.9); // Концентрация электронов
    coef.push_back(0.2); // Концентрация легких дырок
    coef.push_back(0.1); // Концентрация тяжелых дырок


    std::vector<long double> LowBound; // сюда - границы для поиска
    std::vector<long double> UpBound;

    for (int i = 0; i < ExactBound.size(); ++i)
    {
        LowBound.push_back(ExactBound[i]-ExactBound[i]*coef[i]);
        UpBound.push_back(ExactBound[i]+ExactBound[i]*coef[i]);
    }

    // Получаем сам спектр и компоненты тензора
    InDataSpectr MagSpectr(AveragedB.begin(),AveragedB.end());
    InDataSpectr GxxIn(sxx.begin(),sxx.end());
    InDataSpectr GxyIn(sxy.begin(),sxy.end());

    


    thiningSignal(MagSpectr, GxxIn, nMagSpectr, nGxxIn,0, 2, 11);
    thiningSignal(MagSpectr, GxyIn, nMagSpectr, nGxyIn,0, 2, 11);

    

    int GxxSize=nMagSpectr.size();
    int numberOfCarrierTypes=3;

    MultiZoneFit * mzf=new MultiZoneFit(100,VesGxx, VesGxy);


    int result=mzf->RunMultizoneFeat(LowBound,  UpBound,
                          nMagSpectr,  nGxxIn, nGxyIn,
                           outGxx,  outGxy,
                           outValues,
                           numberOfCarrierTypes);

   

/*

    Это постройка графиков. Здесь не нужна.

     ChSxx_theor->Clear();
    ChSxy_theor->Clear();
    ChSxxExper->Clear();
    ChSxyExper->Clear();
    long double koefSxx=1, koefSxy=1;

    if (CheckBox3->Checked)
    {
        koefSxx=max_elem(outGxx);
        koefSxy=max_elem(outGxy);

        if (max_elem(nGxxIn)>koefSxx)
        {
            koefSxx=max_elem(nGxxIn);
        }
        if (max_elem(nGxyIn)>koefSxy)
        {
            koefSxy=max_elem(nGxyIn);
        }
    }

    for (int i = 0; i < outGxx.size(); ++i)
    {
        ChSxx_theor->AddXY(nMagSpectr[i],outGxx[i]/koefSxx,"",clRed);
        ChSxy_theor->AddXY(nMagSpectr[i],outGxy[i]/koefSxy,"",clRed);
    }

    for (int i = 0; i < nMagSpectr.size(); ++i)
    {
        ChSxxExper->AddXY(nMagSpectr[i],nGxxIn[i]/koefSxx,"",clBlue);
        ChSxyExper->AddXY(nMagSpectr[i],nGxyIn[i]/koefSxy,"",clBlue);
    }
*/
    // Результаты идут в формате:
    // По первому индексу:
    // Подвижность электронов, подвижность легких дырок, подвижность тяжелых дырок
    // Концентрация электронов, концентрация легких дырок, концентрация тяжелых дырок
    // По второму индексу: минимальные, средние, СКО, СКО %

    /*
    Сохранение и вывод результатов на форму. Тоже здесь не нужно.
    TStringList * tsl = new TStringList();

        for (int j = 0; j < 4; ++j)
        {

        for(int i=0;i<2*numberOfCarrierTypes;++i)
        {
        tsl->Add(FloatToStr(outValues[i][j]));
        }
        }

        tsl->SaveToFile(SaveDialog1->FileName+eBandWidthFRes->Text+eBandWidthFHall->Text+"MZFitRes.txt");

        // min
        for(int i=0;i<2*numberOfCarrierTypes;++i)
        {

            if(i<numberOfCarrierTypes)
                FitResults->Cells[i+1][1]=FloatToStrF(outValues[i][0],ffFixed,6,6);
            else
          FitResults->Cells[i+1][1]=FloatToStrF(outValues[i][0],ffExponent,5,2);
        }
        //ErrorLog->Lines->Add("middle");
        for(int i=0;i<2*numberOfCarrierTypes+1;++i)
        {
            if(i<numberOfCarrierTypes)
                FitResults->Cells[i+1][2]=FloatToStrF(outValues[i][1],ffFixed,6,6);
            else
           FitResults->Cells[i+1][2]=FloatToStrF(outValues[i][1],ffExponent,5,2);
        }
        //ErrorLog->Lines->Add("SKO");
        for(int i=0;i<2*numberOfCarrierTypes+1;++i)
        {
            if(i<numberOfCarrierTypes)
                FitResults->Cells[i+1][3]=FloatToStrF(outValues[i][2],ffFixed,6,6);
            else
           FitResults->Cells[i+1][3]=FloatToStrF(outValues[i][2],ffExponent,5,2);
        }
        //ErrorLog->Lines->Add("SKOPers");
        for(int i=0;i<2*numberOfCarrierTypes+1;++i)
        {
            if(i<numberOfCarrierTypes)
                FitResults->Cells[i+1][4]=FloatToStrF(outValues[i][3],ffFixed,6,6);
            else
           FitResults->Cells[i+1][4]=FloatToStrF(outValues[i][3],ffExponent,5,2);
        }
        delete mzf;
        delete tsl;

    }
    */

    return true;
}

void MagneticFieldDependence::getMultiCarrierFitResults(InDataSpectr & nMagSpectr, InDataSpectr & nGxxIn, InDataSpectr & GxyIn, 
    MyData_spektr & outGxx, MyData_spektr &  outGxy, TStatistic & outValues)
{
    nMagSpectr=this->nMagSpectr;
    nGxxIn=this->nGxxIn;
    GxyIn=this->GxyIn;
    outGxx=this->outGxx;
    outGxy=this->outGxy;
    outValues=this->outValues;
}



void MagneticFieldDependence::getExtrapolateParams(int & powPolinomH,int & powPolinomR)
{
    powPolinomH=PowPolinomHall;
    powPolinomR=PowPolinomRes;
}