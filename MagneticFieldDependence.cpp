#include "MagneticFieldDependence.h"
extern LCardADC *adc;


MagneticFieldDependence::MagneticFieldDependence(AnsiString current, AnsiString temperature, AnsiString SampleInventoryNumber,
    AnsiString length, AnsiString width, AnsiString Thickness)

{
    

    filterParamsHall=new FilterParams(); // �� ���� ����� �� � ��������������� �� ���, ���� �� ���� ������� � ������ ����:).
    filterParamsResistance=new FilterParams();
    saver =new DataSaver(temperature,current,SampleInventoryNumber, length, width, Thickness);
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
    delete saver;
    saver=0;
    delete filterParamsHall;
    filterParamsHall=0;
    delete filterParamsResistance;
    filterParamsResistance=0;
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
    std::string temp = FileName.c_str();

    unsigned int rBound = temp.find_last_of("/\\");
    unsigned int rBound2 = temp.find_first_of("DCR",rBound);
    AnsiString NewFileName = FileName.SubString(0,rBound2)+"Description"+".txt";
    TStringList * tts = new TStringList();
    if(FileExists(NewFileName))
    {
    tts->LoadFromFile(NewFileName);
    const std::string delimiter = "\t";

    std::string s1 = tts->Text.c_str();
    std::string s;
    ReplaceDotsToComma(s1,s);
    
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
    const std::string delimiterTab = "\t";
    const std::string delimiterSpace = " ";
    const int numberOfColls=3;

    TSignal B;
    TSignal Hall;
    TSignal Resistance;
    TSignal temp;

    for(int i=0;i<tts->Count;++i) // �� ���������� �����
    {
        if(tts->Strings[i].IsEmpty()) // ������ ������ ����������
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

void MagneticFieldDependence::SaveAllData(AnsiString FileName,bool isCombinedParams)
{
// ������, ��� �������� ������� -
// ��� ��� ��� ������� ���������� �������� ����� ������
// Com Dir � Rev
// �� ��� � � ��������� ��� �������.!!!!
ParamsType pt;

if(FileName[FileName.Length()]=='m')  pt=COMBINE;
if(FileName[FileName.Length()]=='v')  pt=REVERSE;
if(FileName[FileName.Length()]=='r')  pt=DIRECT;

saver->setParamsType(pt);
    saver->SaveData(CURRENT_DATA,&B,&HallEffect,&MagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(CURRENT_DATA,&B,&HallEffect,&MagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(FILTERED_DATA,&FilteredB,&FilteredHallEffect,&FilteredMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(FILTERED_DATA,&FilteredB,&FilteredHallEffect,&FilteredMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveData(EXTRAPOLATED_DATA,&ExtrapolatedB,&ExtrapolatedHallEffect,&ExtrapolatedMagnetoResistance,(isCombinedParams?POINTS_21:POINTS_11),FileName);
    saver->SaveData(EXTRAPOLATED_DATA,&ExtrapolatedB,&ExtrapolatedHallEffect,&ExtrapolatedMagnetoResistance,ALL_POINTS,FileName);

    saver->SaveSampleDescription(FileName);
}

//------------�������� ������-------------------------------------------------
template <class T>
int sigh(T in)
{
    if (in>=0)
    {
        return 1;
    }
    else 
        return -1;
}

void MagneticFieldDependence::averageData(TSignal & inY, TSignal &outY, FeatType featType, int centerOfSimmetry)
{
    /*
    ����������� ������ �� ������ ������ ���������� ���� ��� �������������� ���������� ����.
   
    */
    int size=inY.size();

    // ������ �������������� � ����������� �� ����� ���������.
    outY.resize(std::min(centerOfSimmetry + 1,size - centerOfSimmetry));
    
    
        switch(featType)
        {
        case ODD_FEAT: // �������� ��������
        /*
        �������� ����������
        �����
        f(-x)=-f(x)
        ����� ������� ���� ����� ���� ���������� �������� ���������� �������.
        � ����� ��������:

        (f(-x)+(-f(x)))/2.0
        �.�.
        (f(-x)-f(x))/2.0

        �� ��������� ������ �� ������.
        �� ���� ������ ���� �������� ���������������, � ������
        ������� ������� ������������� �������� -2 ��.
        ���������� ��� ���� ����� ���� ����� �������� � �������� �� ��������������� -
        ��� � ����� ��� ����, ������� ��� ��� �����.

        ���� �������� ������������ �������� �� �������� - ������ ����������� ������,
        �.�. ��-�� ��������������� ������� ��� �������� ���������� ���� ��������
        ������� �������� ���������� ����� �������� �� ������.
        ��� ����� ������� � ����, ��� ���� ������ ��� ����� - ����������� �� ���������.
        
        � ��������� ����� ��� ��������� ���������� ������, �� ��� ���� ���� ����
        � ������� - ��������� ��������� ��������.
        ������� ����� ����� ���� ��� �����, ������� ���������. �������� �������.


        � ��� � ����� ��� ���� ����� - � ��� ���� ������� �� ������ �����?
        �� ���� ������������� ���� ���������� � ��� ���������....

        ���, �� ����������� ������������ ����� B=0 � ���������� �������� �������,
        ������� ������������� ���� ������� 0.
        ������ - ��� ��� ������ ��� ��������?
        � ����� ������ ���� �� ����� ��� ����� �������� ������� ������ ����
        ������� � ����, �� �� ����� ���� ������� � ����, �� ��� ��� � �� ��������.

        �������� �������� � ����������� �������������� ������� ����������� ����������.


        */
        for(int i=0;i<size/2+1;++i)
            {
                /*
                ���� ����� ����� - ����� ���������.
                �� ��������� ������ ������������ ����� ������.
                ����� ������� ����������
                */

                if (centerOfSimmetry+i<size && centerOfSimmetry-i>=0) // ���� �� ���������� �� �������
                {
                    outY[i]=(inY[centerOfSimmetry+i]-inY[centerOfSimmetry-i])/2.0;     

                }
                
                else // ���� �������� ������� �� ����� ��� ������
                    // ������ ����� ��� ������������� ��� ���������� ������� ������������� ����������
                    // ��� �� ����� ���������. ����� ����������������.
                { 
                    /*
                        ����� ���������� ���.
                        ���������� ������� ����� ����� ����������������.
                        ������ �� 2+h ��
                        �������� �������������.
                        �������� ���������� ����.
                        ���� ������ ����� ����������. � ��� ������ � ��������.
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

            if( sigh(inY[0]*sigh(inY.back()))<0) // ����� ���� �� ����� ����� ������
                outY[i]=-sigh(inY[i])*fabs((inY[i]-inY[size-1-i]))/2.0;
            else
                outY[i]=sigh(inY[i])*fabs((inY[i]-inY[size-1-i]))/2.0;
*/
                //outY[i]=sigh(inY[size-1-i])*fabs((inY[i]-inY[size-1-i]))/2.0;
                //outY[i]=-sigh(inY[i])*fabs((inY[i]-inY[size-1-i]))/2.0;
                //outY[size-1-i]=-inY[i];
            }
            // ��������� �������� � �������� ���� - �������� ������.
      //  std::reverse(outY.begin(),outY.end());
            break;
        case EVEN_FEAT: // ������ ��������
        /*
        ���, ������ ���������� ��� � ��� �� �� ��� ������.
        ���� � ��� ������ ���������.
        ������������ ��� Y
        ��� ��������, ��� ��� ��������������� �������� �������� ���������� ���� �������� ������ ������� �����
        f(-B)=f(B)
        ��� ������� ������� ������ ��� ������� ������� � ��� ������� ������� ��������.
        �� ��� ������������� ��������, � ����������� ����� ������� ���������� ����������� - �� ��������� �� �� �����.
        ����� ��������� ������������ ����� �=0
        �.�. ����:
        1. ����� ����� �=0
        2. �������� � ������
        3. ��������� ������������ � �� ����� ��� ���������
        4. ����������� (��� ������� �������� ��������� �����, ��� ������� ������� ��� ����������������) ����� �� ��������� �������, ��� ����������
        P.S. �� ������ ������ ��������� ����������� �����, �� ����������� �����.
        5. ��������� �� ����� � ���� �������� ��������, �� ������� ������� (�������������) ��� ������ �������� �������� �� ��������� 
        � ������� ��������� ���������� ����.
        ��� ����� ������ ����� �������� - ������ ��������, ������������ �������� ������ ���������.
    

        */

        // � ������ B=0 �� �����-����� ������ - 
        // ������ ������� ������ �������, �� � ����� ����������� ������
        // � ��� � �� ������ - �� �����������, ��� ��������� ������ ������� � ������
        // 
        
        //ShowMessage(FloatToStr(inY[min])+" "+IntToStr((int)min));
            
        for(int i=0;i<size/2+1;++i) // ��������� ������ � ��� ���� ������.
        {
            if (centerOfSimmetry+i<size && centerOfSimmetry-i>=0) // ���� �� ���������� �� �������
            {
                outY[i]=(inY[centerOfSimmetry-i]+inY[centerOfSimmetry+i])/2.0;
                if(fabs(outY[i])<0.0001)
                     ShowMessage("������ �����!");     
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
    TSignal tempInX;
    TSignal tempInHall;
    TSignal tempInResistance;
    
    tempInX=(*getPointerB(dataKind));
    tempInHall=(*getPointerHall(dataKind));
    tempInResistance=(*getPointerMagnetoResistance(dataKind));
    
    if(tempInX.size()==0)
    {
        //"������� ������ ������ ������!!!");
        return;
    }
    
    averageData(tempInX,AveragedB,ODD_FEAT,indexOfElemClosestToValue(tempInX,0));
    averageData(tempInHall,AveragedHallEffect,ODD_FEAT,indexOfElemClosestToValue(tempInX,0));
    averageData(tempInResistance,AveragedMagnetoResistance,EVEN_FEAT,indexOfElemClosestToValue(tempInX,0));

    // extrapolateData(AVERAGED_DATA, 1, 1);


    TSignal * BToExtrapolate=& AveragedB;
    TSignal * HallToExtrapolate=& AveragedHallEffect;
    TSignal * ResistanceToExtrapolate=& AveragedMagnetoResistance;

    

    
    
    TSignal inBHall;
    TSignal inBMagnetoResistance;

    TSignal inHallEffect;
    TSignal inMagnetoResistance;

    /*
    ������������� ������ ������� �� ��������� �������� ������������� ��������.

    ��� ��� ���������� � �����.
    ���������������� ����� ���� �� �������������, ���� �� ����������� ���������.
    */
    for (size_t i =0 /*(2.5*BToExtrapolate->size())/4*/; i < BToExtrapolate->size(); ++i)
    {
        inBHall.push_back( (*BToExtrapolate)[i]);
        inBMagnetoResistance.push_back( (*BToExtrapolate)[i]);
        inHallEffect.push_back( (*HallToExtrapolate)[i]);
        inMagnetoResistance.push_back( (*ResistanceToExtrapolate)[i]);
    }
/*
    int ves=200;
    for (unsigned int i = 0; i<ves; ++i) 
    {
        inBHall.push_back(BToExtrapolate->back());
        inBMagnetoResistance.push_back(BToExtrapolate->back());
        inHallEffect.push_back(HallToExtrapolate->back());
        inMagnetoResistance.push_back(ResistanceToExtrapolate->back());
    } */
  
    unsigned int NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("���������� ����� ����� ����! � �� ���� ������ �� ����:)");
    return;
    }
    MyDataType h=(max_abs_elem(inBHall)- min_elem(inBHall))/ static_cast<MyDataType>(NumberOfPoints);
    PowPolinomRes=1;
    PowPolinomHall=1;
    
    TSignal koefMagnetoResistance(PowPolinomRes+1);
    TSignal koefHallEffect(PowPolinomHall+1);

    if(!curveFittingUniversal(&inBMagnetoResistance,&inMagnetoResistance, &koefMagnetoResistance,PowPolinomRes))
    return;
    if(!curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,PowPolinomHall))
    return;   

    TSignal newB;
    TSignal newHallEffect;
    TSignal newMagnetoResistance;

    

    newB.clear();
    newB.push_back(BToExtrapolate->back()-h); // ��������� ��������� ����.
    for (unsigned int i = 1; i<NumberOfPoints; ++i) {
        newB.push_back(newB[i-1]+h);
    }

    // ��������� ������������������ �����������.
    calculatePolinomByKoef(newB,koefMagnetoResistance,newMagnetoResistance);
    calculatePolinomByKoef(newB,koefHallEffect,newHallEffect);

    //----------� ��� ��� ������������ ����������� ������� � �������----

    unsigned int i=0;
    while(i<NumberOfPoints && newB[i]<=BToExtrapolate->back())
    ++i; // ������� ������� �� ������� ������������� ������������� ��������� ����.    

    for(unsigned int j=i;j<NumberOfPoints && newB[j]<=2.5;j++)
    {     // � ����� ���������� ������������������ ��������.
        BToExtrapolate->push_back(newB[j]);
        ResistanceToExtrapolate->push_back(newMagnetoResistance[j]);
        HallToExtrapolate->push_back(newHallEffect[j]);
    }

    // � ����� �� ���� ������-��?�_�
    //std::reverse(outY.begin(),outY.end());
    
}

void MagneticFieldDependence::GetEqualNumberOfPoints(TSignal & B,
TSignal & BHall,TSignal & BRes, TSignal & Hall,
TSignal & Res)
{
    MyDataType left, right;
    size_t length;

    TSignal tempB;
    TSignal tempHall;
    TSignal tempResistance;
    if(Hall.size()==0 || Res.size()==0 || B.size()==0)
    {
    return;
    }
    // ---------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (Hall.size()>Res.size())
    {
        left=Res[0];
        right=Res.back();
        length=Res.size();
    }
    else
    {
        left=Hall[0];
        right=Hall.back();
        length=Hall.size();
    }
    // ��� ������ ���������� ���������� �����, ����� ���������� � ������ ������.
    thiningSignal(BHall, Hall, tempB, tempHall, left, right, length);
    thiningSignal(BRes, Res, tempB, tempResistance, left, right, length);
    
    BHall.clear();
    Hall.clear();
    BRes.clear();
    Res.clear();
    B.clear();
    
    BHall=tempB;
    Hall=tempHall;
    BRes=tempB;
    Res=tempResistance;
    B=BHall;
}
//-------------------------------------------------------------------------
void MagneticFieldDependence::blockfilterData()
{
    /*
    ������� ������.
    ���� ������ ����������� �� ����� � ����������� ��������.
    ����� ���� ������������.
    �� ������ ������ �������� �� ����� ������.
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
    // ������� ���������� ���������� ����������
    clearFilteredParams();
    // ���������� ���������� �����
    GetEqualNumberOfPoints(B,BHall,BMagnetoResistance,
    HallEffect,MagnetoResistance);

    if( B[0]<-1.0 && B.back() >1.0) // ���� ��� ��������������� ������
    {
    featData(CURRENT_DATA);// ��� ���� ��������� 
    // � ����������� ����� ����������� ������.
	}

    filterDataHelper((*filterParamsHall),HALL_EFFECT);
    
    /* TStringList * tsl= new TStringList;
    for (int i=0; i<AveragedB.size(); ++i)
    {
        tsl->Add(FloatToStr(AveragedB[i]) );
    }
    tsl->SaveToFile("E:\\����\\�������� ������ ���������������\\Lcard\\MCTParamsExplorer\\AveragedB.txt");

    */
    filterDataHelper((*filterParamsResistance),MAGNETORESISTANCE);
    FilteredB=FilteredBHall;
    

    GetEqualNumberOfPoints(FilteredB,FilteredBHall,FilteredBMagnetoResistance,
    FilteredHallEffect,FilteredMagnetoResistance);
    
    /*tsl->Clear();
     //TStringList * tsl= new TStringList;
    for (int i=0; i<FilteredMagnetoResistance.size(); ++i)
    {
        tsl->Add(FloatToStr(FilteredB[i]) + "\t"+ FloatToStr(FilteredMagnetoResistance[i]));
    }
    tsl->SaveToFile("E:\\����\\�������� ������ ���������������\\Lcard\\MCTParamsExplorer\\filterOutPutAftFiltr2.txt");
*/

}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterData(FilterParams &fPHall, FilterParams &fPResistance)
{
    /*
    �������� ����������, ����� ��������� ����� ���������� ����������.
    */
    setFilterParamsHall(fPHall.SamplingFrequecy, fPHall.BandwidthFrequency, fPHall.AttenuationFrequency, fPHall.filterLength);
    setFilterParamsResistance(fPResistance.SamplingFrequecy, fPResistance.BandwidthFrequency, fPResistance.AttenuationFrequency, fPResistance.filterLength);
    filterData();

}
//-------------------------------------------------------------------------------
void MagneticFieldDependence::filterDataHelper(FilterParams &fP,
    SignalType dependenceType)
{
    if(HallEffect.size()!=B.size())
    {
        ShowMessage("���������� ����� ���������� ���� � ������� ����� �� ���������! filterdataHelper");
        return;
    }

    TSignal * inB;
    TSignal * inHallEffect;
    TSignal * inMagnetoResistance;
    unsigned int NumberOfPoints;

    if( B[0]<-1.0 && B.back() >1.0) // ���� ��� ��������������� ������
    {
        //featData(CURRENT_DATA); // ��� ���� ���������
        inB=&AveragedB; // ����������� ����� ����������� ������
        inHallEffect=&AveragedHallEffect;
        inMagnetoResistance=&AveragedMagnetoResistance;
        NumberOfPoints=AveragedB.size();
    }
    else // ���� ��� ������� ������ - ��������� ��� ����.
    {
        inB=&B;
        inHallEffect=&HallEffect;
        inMagnetoResistance=&MagnetoResistance;
        NumberOfPoints=HallEffect.size();
    }

    // � ��� ������� ����� ������������� ������ ��� ������������� ��������� �����.
    // ��� ����� ����, � ���� ��� ������ ��� �������������� ���������� ����?
    // �� ������������ ������������� ������������� �����.
    // ���� ��������� ��� �������� �� ���������� ����������������.
    TSignal tempInB(2*NumberOfPoints);
    TSignal tempInSignal(2*NumberOfPoints);

    TSignal tempOutB(2*NumberOfPoints);
    TSignal tempOutSignal(2*NumberOfPoints);

    switch(dependenceType)
    {
    case HALL_EFFECT:


    // ��������� ������ ��� �������.
    // ���������� ��� � ������������� ��������� ����.
    for (unsigned int i = 0; i < NumberOfPoints; ++i)
    {
    /*
    ������� ����������� ���� ���������.
    � ������� ����� ������������� ���������, ������������ �����
    B==0;
    � ���� ������ � ����, ��� ��� ������� �������? /(�_�)\

    ���������� ��� ������� � ������������� ��������� ����� - ��� �����������.
    ��� ������� ����������������, �.�. ��� ������������� - ��� �����������,
    ������ ��� ����������� ����������� ������ (�� ���� ������� ������ ��������
    �������������� ���������� ����.

    ��� �������������� ���������� ���� ��� ���������, ��������� ��� ���� - �� �����������.
    

    */
        tempInSignal[i]=-(*inHallEffect)[NumberOfPoints-i-1]+2*(*inHallEffect)[0];
        tempInB[i]=-(*inB)[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=(*inHallEffect)[i];
        tempInB[i+NumberOfPoints]=(*inB)[i];
    }
    break;
    case MAGNETORESISTANCE:

    

    for (unsigned int i = 0; i < NumberOfPoints; ++i)
    {
        tempInSignal[i]=(*inMagnetoResistance)[NumberOfPoints-i-1];   // ���
        //tempInSignal[i]=-Dependence[NumberOfPoints-i-1]+2*Dependence[0];  // �����
        tempInB[i]=-(*inB)[NumberOfPoints-i-1];
        tempInSignal[i+NumberOfPoints]=(*inMagnetoResistance)[i];
        tempInB[i+NumberOfPoints]=(*inB)[i];
    }
    break;

    default:
        break;
    }

    /*
    � ������ �������������� ���������� ���� ���� ������������� ������� ���������
    ������ ��� ������� ������������� �������� ��� �������������� ���������� ����.
    */
    if(tempInB[0]>1.0)
    {
        std::reverse(tempInB.begin(),tempInB.end());
        std::reverse(tempInSignal.begin(),tempInSignal.end());
    }
    // ��������� 
    TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,
                fP.filterLength,fP.SamplingFrequecy,fP.BandwidthFrequency,fP.AttenuationFrequency);


    // ������ ������ ������� ��������, � ����������� �� ����� �������.
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



    
/*
    unsigned int i=0;

    while(i<NumberOfPoints && tempOutB[i]<=0 ) ++i; // ���� ��� ���� ���������� �������������.

    // ����� ���������� ������������� ����� �������������� ������� �������.
    for(;i<NumberOfPoints;++i)
    {

    switch(dependenceType)
    {
    case HALL_EFFECT:
        FilteredHallEffect.push_back(tempOutSignal[i]);

        break;
    case MAGNETORESISTANCE:
        FilteredMagnetoResistance.push_back(tempOutSignal[i]);
        FilteredB.push_back(tempOutB[i]);
        break;
    default:
        break;
    }
    } */


}

//-------------------------------------------------------------------------------

bool MagneticFieldDependence::extrapolateData( DataKind dataKind, const int polinomPowForMagnetoResistance, const int polinomPowForHallEffect)
{
	/*
	��� �� ����� ����� ���� - ���� ����������� ���������������� ����.
	������� ��� ��������� �� ���������������� ������.
	� ����������� �� ���� ����� �����, ��� ����� ���� �������������, ���� ����������� ������.
	���� ����� ���-�� ����� �� ��� - �� �������.
	������������� �������� ������ �� ����� ������� - �� ������ ������ �� ��������� ���� ������.

	����� �������� ���������� ���� ����� 0, 0 ��� ����������� ����������.

	�������� ������ ��� ������������� � �������� ������� ������ ������� ( curveFittingUniversal ).
	��� ������� ������ ���� �� ���������, �� ��� ���� ������ ��� ������ �������� ���.

	��������� ���� ����������� �������� ����� (������, �������� ��� ����� ������������ ���-�� �� �������), ������� � ���������� �������� ��������.

	*/
    bool returnValue=true;

    TSignal * BToExtrapolate= NULL;
    TSignal * HallToExtrapolate= NULL;
    TSignal * ResistanceToExtrapolate= NULL;

    if (dataKind==FILTERED_DATA)
    {
        BToExtrapolate= & FilteredB;
        HallToExtrapolate= & FilteredHallEffect;
        ResistanceToExtrapolate= & FilteredMagnetoResistance;
    }
    if(dataKind ==AVERAGED_DATA)
    {
        BToExtrapolate= & AveragedB;
        HallToExtrapolate= & AveragedHallEffect;
        ResistanceToExtrapolate= & AveragedMagnetoResistance;
    }   
    // ���� ����� ���-�� ����� �� ��� - �� �������.
    if(BToExtrapolate==NULL)
    {
    	return false;
    } 

    TSignal inBHall;
    TSignal inBMagnetoResistance;

    TSignal inHallEffect;
    TSignal inMagnetoResistance;

    /*
    ������������� ������ ������� �� ��������� �������� ������������� ��������.

    ��� ��� ���������� � �����.
    ���������������� ����� ���� �� �������������, ���� �� ����������� ���������.
    */
    for (size_t i = (3*BToExtrapolate->size())/4; i < BToExtrapolate->size(); ++i)
    {
        inBHall.push_back( (*BToExtrapolate)[i]);
        inBMagnetoResistance.push_back( (*BToExtrapolate)[i]);
        inHallEffect.push_back( (*HallToExtrapolate)[i]);
        inMagnetoResistance.push_back( (*ResistanceToExtrapolate)[i]);
    }

    unsigned int NumberOfPoints=inBHall.size();
    if(NumberOfPoints==0)
    {
    ShowMessage("���������� ����� ����� ����! � �� ���� ������ �� ����:)");
    return false;
    }

    MyDataType h=2.5/NumberOfPoints;
    
    int ves = inBHall.size()/10;
    for(int i=0;i<ves;++i) // ����������� ��� ����� (0,0) ��� ������� �����.
    {
        inBHall.push_back(0);
        inHallEffect.push_back(0);
    }

    TSignal koefMagnetoResistance(polinomPowForMagnetoResistance+1);
    TSignal koefHallEffect(polinomPowForHallEffect+1);
    
    if(!curveFittingUniversal(&inBMagnetoResistance,&inMagnetoResistance, &koefMagnetoResistance,polinomPowForMagnetoResistance))
    return false;
    if(!curveFittingUniversal(&inBHall,&inHallEffect, &koefHallEffect,polinomPowForHallEffect))
    return false;

    
    TSignal newB;
    TSignal newHallEffect;
    TSignal newMagnetoResistance;

    newB.push_back(BToExtrapolate->back()-h); // ��������� ��������� ����.
    for (unsigned int i = 1; i<NumberOfPoints; ++i) {
        newB.push_back(newB[i-1]+h);
    }

    // ��������� ������������������ �����������.
    calculatePolinomByKoef(newB,koefMagnetoResistance,newMagnetoResistance);
    calculatePolinomByKoef(newB,koefHallEffect,newHallEffect);

    ExtrapolatedB.clear();
    ExtrapolatedMagnetoResistance.clear();
    ExtrapolatedHallEffect.clear();

    ExtrapolatedB=newB;    
    ExtrapolatedMagnetoResistance=newMagnetoResistance;
    ExtrapolatedHallEffect=newHallEffect;
    
    //----------� ��� ��� ������������ ����������� ������� � �������----

    // ���, � �� �����-�� ������ �� �������������� �������?
    // ���-�� ���� �� �������, �� �������� � �� �����.
    
    unsigned int i=0;
    while(i<NumberOfPoints && newB[i]<=BToExtrapolate->back())
    ++i; // ������� ������� �� ������� ������������� ������������� ��������� ����.

    for(unsigned int j=i;j<NumberOfPoints;j++)
    {     // � ����� ���������� ������������������ ��������.
        BToExtrapolate->push_back(newB[j]);
        ResistanceToExtrapolate->push_back(newMagnetoResistance[j]);
        HallToExtrapolate->push_back(newHallEffect[j]);
    }
    //------------------------------------------------------------------
    
    /* TStringList * tsl= new TStringList;
    for (int i=0; i<FilteredMagnetoResistance.size(); ++i)
    {
        tsl->Add(FloatToStr(FilteredB[i]) + "\t"+ FloatToStr(FilteredMagnetoResistance[i]));
    }
    tsl->SaveToFile("E:\\����\\�������� ������ ���������������\\Lcard\\MCTParamsExplorer\\filterOutPutAftExtr.txt");
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
	std::string strToReplaceWhich=","; // �� ��� ������
	std::string strToSearch=".";   // ��� ����
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
    TSignal * pointToX=0; // ��������� �� ��������� ������
    TSignal * pointToY=0;
	s->Clear(); // ������ ������.
    switch(pt) // ���������� ��� ��������.
    {
    case HALL_EFFECT:
        pointToX=getPointerB(dk);
        pointToY=getPointerHall(dk);
        break;
    case MAGNETORESISTANCE:
        pointToX=getPointerB(dk);
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
        //ShowMessage("������ ����, ������� ������.");
        return false;
    }
    /*
    ��-�� ������������ �������� � ������������ �������� ����� ���� �������
    ������� ���������� ������ ������������� ����������.
    */
    TSignal::iterator posX;
    TSignal::iterator posY;
    // ���� ����� � ������� ������ ��� �� ����� ������� - ������� ������� ����.
    long double PointsToShow=(500>pointToX->size()?pointToX->size():500);

    if (pointToX->size() != pointToY->size())
    {
    // ShowMessage("������ ���������� ����� � ��������! ���-�� �� ����� ���� �����");
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
    // � ����������� ������� ����� �������� �� ����������� � �������� ����-������ ���.      
}

//---------------------------------------------------------------------------------

void MagneticFieldDependence::getSplittedDataFromADC()
{
    TwoDimensionalContainer * tempData1=adc->getSplittedData(1);

    TwoDimensionalContainer & tempData(*tempData1);
    clearCurrentParams();
             
    B=tempData[2];
    HallEffect=tempData[0]; // ������������������ ���������� � �� �����.
    MagnetoResistance=tempData[1];

    adc->dataisntNeeded();
    // ��� ����� ������� �� ������� �������� - ��� ��������� ����� �� �������.
    // ��������� ���� ����������, �.�. ��� ���������� ������ �������� ����������� �������:).
    
    BHall=B;
    BMagnetoResistance=B;

    multiplyB(CURRENT_DATA);
    filterData();
    //extrapolateData(4,4); // ���������� �����, ������� ��������� ��� ������������� �� ���������.
    // � ����������� ������� ����� �������� �� ����������� � �������� ����-������ ���.
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


void MagneticFieldDependence::calculateEffectiveParamsFromSignals()
{
    if (AveragedB.size() != 0)
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
            if(B[i]==0)
                Rh_eff[i]=0;
            else
                Rh_eff[i]=SThickness*AveragedHallEffect[i]/I;
        }
    }
}

//--------------------------------------------------------------------------------

void MagneticFieldDependence::calculateTenzorFromEffectiveParams()
{
    if (s_eff.size() != 0)
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
    //cutData(AVERAGED_DATA);
    calculateEffectiveParamsFromSignals();
    calculateTenzorFromEffectiveParams();
}

//-------------------------------------------------------------------------
void  MagneticFieldDependence::cutData(DataKind dataKind)
{
    // � ���� ��� ������ �� ����������... ����� � � �����?))))
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
