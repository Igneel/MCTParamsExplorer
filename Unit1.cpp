//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

/*
TODO
������ ����������� ����
������������� "��������� ���������"
����������� ������ "������" - �.�. ������� ���������� �������� � ������ ������ �����
������� �������� ������������� ��������� �����

��������������� �����������

���-�, ������� ������:
1. ��� (������������� ��������, ���������, ������/������� ���������, ��������� ������).
2. ���������� ����������� (���������, ����������, �������������, ����������/����������, ������� � �.�.,
    ���������� ��������, ���������� �����������).
3.

���������� �� ������:
1. ����������� ��������� +
2. ����������            +
3. ��������� ��������    +
4. ��������� ���������
5. �������������         +

���������� �� ���������� ����������.
���������� ��������� ������: �������� � ���������.

��������� ������� ��� ������ � ������.

*/

// ��������, ����������� �������� ���-������,
// �� ������ ������ ��� � ������������ ����:)

int NumberOfChannels =3;
int BlockSize=1024;
int Frequency=200000;
double h=0.001;//0.02;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TForm1::LoadLa7(void)
{
;
}


void __fastcall TForm1::FormCreate(TObject *Sender)
{
   
// ������������� ��� ��������� �������� � �������� � ������ ������ ��,
    // ��� ������������ �������� ������ �� DMA

        // ��������� �������

        //-------------------------------------------
    float       MaxFreq;   //������������ �������
    // ���������� ������� ����������� � ������� ������
    //������ ������������ �������
    //������������� ��������
    uiFreq->TickStyle = tsNone;
    uiBlockSize->TickStyle = tsNone;
    uiBlockSize->Max = 65536;
    uiBlockSize->Min = 1024;
    uiBlockSize->Position=1024;
    uiBlockSize2->Text=1024;

    uiFreq->Max = MaxFreq;
    uiFreq->Min = 20000;
    uiFreq->LineSize = (MaxFreq - 20000)/20;
    uiFreq->Position = 200000;
    //----����� ������������ �������� ��� �������:---------------------------
    Chart1->LeftAxis->Maximum=10;
    Chart1->LeftAxis->Minimum=-10;
    //------------------------------------------------------------------------
    Chan1->ItemIndex=0;
    Chan2->ItemIndex=0;
    GainKoefFaradey->ItemIndex=0;
    //------------------------------------------------------------------------
    //-----------------------------------------------------------------
    

    // ��� ��������� ����������� ������ ����������

    // ��� ������ ���������� ������� �������
    Series5->AddXY(0,-10,"",clBlack);
    Series5->AddXY(0,1000,"",clBlack);

    GainKoefFoygt->ItemIndex=0;
}
//---------------------------------------------------------------------------

// ��� ����� ����� �������� - ����������� ��������� �������
void __fastcall TForm1::uiDriverNameChange(TObject *Sender)
{
;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiBaseChange(TObject *Sender)
{
    //ADCBase = BaseList[uiBase->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiDRQChange(TObject *Sender)
{
    //ADCDRQ = DRQList[uiDRQ->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiIRQChange(TObject *Sender)
{
    //ADCIRQ = IRQList[uiIRQ->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)// ����� �� ���������
{
    Form1->Close();
}
//----����� ����, ���������/�������� ����������� ��������, �� ����� ������ ���-
//---------------------------------------
void __fastcall TForm1::uiControlClick(TObject *Sender)
{
//bS = !bS;
/*
    if (bS)
    {
        
        uiIRQ->Enabled = false;
        uiDRQ->Enabled = false;
        uiBase->Enabled = false;
        uiDriverName->Enabled = false;
        uiFrenq->Enabled = false;
        uiFreq->Enabled = false;
        uiBlockSize->Enabled=false;
        uiBlockSize2->Enabled=false;
        Chan1->Enabled=false;
        Chan2->Enabled=false;
        CurrentRes->Enabled=0;
        CurrentHall->Enabled=0;
        ResCurveIndex->Enabled=0;
        HallCurveIndex->Enabled=0;
        GainKoefFaradey->Enabled=0;

        //-- ������-----------------------------------------------
        uiResFeat->Enabled=0;
        uiFFTRes->Enabled=0;
        uiHallFeat->Enabled=0;
        uiFFTHall->Enabled=0;
        uiFaradeyFeat->Enabled=0;
        uiFFTFaradey->Enabled=0;
        uiFoygtFeat->Enabled=0;
        uiFFTFoygt->Enabled=0;


        // ������� ������������ ������ ��� �������������� �����
        Edit1->Enabled=0;
        //FFTResCrop->Enabled=0;
        //FFTHallCrop->Enabled=0;
        //FFTFaradeyCrop->Enabled=0;
        //FFTFoygtCrop->Enabled=0;

        CurrentFaradey->Enabled=0;
        CurrentFoygt->Enabled=0;

        FaradeyCurveIndex->Enabled=0;
        FoygtCurveIndex->Enabled=0;

        GainKoefFoygt->Enabled=0;

        Start();
        uiControl->Caption = AnsiString("Stop");
        uiResControl->Caption = AnsiString("Stop");
        uiHallControl->Caption = AnsiString("Stop");
        uiFaradeyControl->Caption = AnsiString("Stop");
        uiFoygtControl->Caption = AnsiString("Stop");
        StatusBar->Panels->Items[1]->Text="���������� ���������";
    }
    else
    {  */
        GainKoefFaradey->Enabled=1;
        CurrentRes->Enabled=1;
        CurrentHall->Enabled=1;
        ResCurveIndex->Enabled=1;
        HallCurveIndex->Enabled=1;
        uiIRQ->Enabled = true;
        uiDRQ->Enabled = true;
        uiBase->Enabled = true;
        uiDriverName->Enabled = true;
        uiFrenq->Enabled =true;
        uiFreq->Enabled = true;
        uiBlockSize->Enabled=true;
        uiBlockSize2->Enabled=true;
        Chan1->Enabled=true;
        Chan2->Enabled=true;

        uiResFeat->Enabled=1;
        uiFFTRes->Enabled=1;
        uiHallFeat->Enabled=1;
        uiFFTHall->Enabled=1;
        uiFaradeyFeat->Enabled=1;
        uiFFTFaradey->Enabled=1;
        uiFoygtFeat->Enabled=1;
        uiFFTFoygt->Enabled=1;

        // ������� ������������ ������ ��� �������������� �����
        Edit1->Enabled=1;
        //FFTResCrop->Enabled=1;
        //FFTHallCrop->Enabled=1;
        //FFTFaradeyCrop->Enabled=1;
        //FFTFoygtCrop->Enabled=1;

        CurrentFaradey->Enabled=1;
        CurrentFoygt->Enabled=1;

        FaradeyCurveIndex->Enabled=1;
        FoygtCurveIndex->Enabled=1;
        GainKoefFoygt->Enabled=1;

        Stop();
        uiControl->Caption = AnsiString("Start");
        uiResControl->Caption = AnsiString("Start");
        uiHallControl->Caption = AnsiString("Start");
        uiFaradeyControl->Caption = AnsiString("Start");
        uiFoygtControl->Caption = AnsiString("Start");
        StatusBar->Panels->Items[1]->Text="������ � ������. �������� ������";

}

//---------------------------------------------------------------------------

void __fastcall TForm1::uiFrenqChange(TObject *Sender)
{
    uiFreq->Position=StrToInt(uiFrenq->Text);
    Frequency=uiFreq->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiBlockSize2Change(TObject *Sender)
{
    uiBlockSize->Position=StrToInt(uiBlockSize2->Text);
    BlockSize= uiBlockSize->Position;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::uiBlockSizeChange(TObject *Sender)
{
    uiBlockSize2->Text=uiBlockSize->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFreqChange(TObject *Sender)
{
uiFrenq->Text=uiFreq->Position;
}
//---------------------------------------------------------------------------
// �������� ��������������������
void __fastcall TForm1::uiResFeatClick(TObject *Sender)
{
    EvenFeat(SeriesRes1,0);
    EvenFeat(SeriesRes2,0);
    MidCurve(SeriesRes1,SeriesRes2,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::uiHallFeatClick(TObject *Sender)
{
    OddFeat(SeriesHall1,0);
    OddFeat(SeriesHall2,0);
    MidCurve(SeriesHall1,SeriesHall2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFaradeyFeatClick(TObject *Sender)
{
    EvenFeat(SeriesFaradey1,0);
    EvenFeat(SeriesFaradey2,0);
    MidCurve(SeriesFaradey1,SeriesFaradey2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFoygtFeatClick(TObject *Sender)
{
    FoygtFeat(SeriesFoygt1,SeriesFoygt2,0);
}
//---------------------------------------------------------------------------

enum FeatType {ODD_FEAT, EVEN_FEAT};
void FeatCurve(TLineSeries* a, long index, FeatType featType)
{
    int size=a->XValues->Count();
    if(size==0)
    {
        ShowMessage("������� ������ ������ ������!!!");
        return;
    }
    TChartValueList * b,*c;

    // ��������� ������ � ���������� ������������ ������� ������� �������
    // 0 ��� �, 1 ��� y

    if(!index)
    {
        c=a->YValues;
        b=a->XValues;
    }
    else
    {
        b=a->YValues;
        c=a->XValues;
    }
    for(int i=0;i<size/2;i++)
    {
        b->Value[i]=(b->Value[i]-b->Value[size-1-i])/2.0;
        b->Value[size-1-i]=-b->Value[i];
        switch(featType)
        {
        case ODD_FEAT: // �������� ��������
            c->Value[i]=(c->Value[i]-c->Value[size-1-i])/2.0;
            c->Value[size-1-i]=-c->Value[i];
            break;
        case EVEN_FEAT: // ������ ��������
            c->Value[i]=(c->Value[i]+c->Value[size-1-i])/2.0;
            c->Value[size-1-i]=c->Value[i];
            break;
        }

    }
}

//---------------------------------------------------------------------------
// �������� ��������
// ��������� ������ � ���������� ������������ ������� ������� �������
// 0 ��� �, 1 ��� y
void  OddFeat(TLineSeries* a, long index)
{
    FeatCurve(a,index,ODD_FEAT);
}
//-----------------------------------------------------------------------------
//������ ��������
// ��������� ������ � ���������� ������������ ������� ������� �������
// 0 ��� �, 1 ��� y
void  EvenFeat(TLineSeries* a, long index)
{
    FeatCurve(a,index,EVEN_FEAT);
}
//-----------------------------------------------------------------------------
//----------�������� ���������������� � ��������� ������-----------------------
void  FoygtFeat(TLineSeries* a,TLineSeries* b, long index)
{
    int size=a->XValues->Count();
    if(size==0 || b->XValues->Count()==0)
    {
        ShowMessage("������� ������ ������ ������!!!");
        return;
    }

    for(int i=0;i<size;i++)
    {
        a->XValues->Value[i]=(a->XValues->Value[i]-b->XValues->Value[size-1-i])/2;
        a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[size-1-i])/2;
    }
}
//-----------------------------------------------------------------------------
// ���������� ���� ������
// ������ - ��������������, ���� �� ������������
// ������� ��������� ��������� � ������ ������������ ������!
void  MidCurve(TLineSeries* a, TLineSeries* b, long index)
{

    int size=a->XValues->Count();

    if(size==0 || b->XValues->Count()==0)
    {
        ShowMessage("������� ������ ������ ������!!!");
        return;
    }
    if(a->XValues->Count()!=b->XValues->Count())
    {
        ShowMessage("������ ���������� ����� �� ��������!!!");
        return;
    }

    for(int i=0;i<size;i++)
    {
    a->XValues->Value[i]=(a->XValues->Value[i]+b->XValues->Value[i])/2;
    a->YValues->Value[i]=(a->YValues->Value[i]-b->YValues->Value[i])/2;
    }
}
//----------------------------------------------------------------------------

void __fastcall TForm1::bClearClick(TObject *Sender) // ������� ��:)
{
    Series1->Clear();
    Series2->Clear();
    Series3->Clear();
    SeriesRes1->Clear();
    Memo1->Clear();
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
void __fastcall TForm1::uiFFTResClick(TObject *Sender)
{

    Tr_Filter((ResCurveIndex->ItemIndex==0?SeriesRes1:SeriesRes2),SeriesFFTRes,Lfilter1->Text.ToInt(),Fd1->Text.ToInt(),
    Fp1->Text.ToInt(),Fz1->Text.ToInt());

}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTHallClick(TObject *Sender)
{
    Tr_Filter((HallCurveIndex->ItemIndex==0?SeriesHall1:SeriesHall2),SeriesFFTHall,Lfilter2->Text.ToInt(),Fd2->Text.ToInt(),
    Fp2->Text.ToInt(),Fz2->Text.ToInt());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTFaradeyClick(TObject *Sender)
{
    Tr_Filter((FaradeyCurveIndex->ItemIndex==0?SeriesFaradey1:SeriesFaradey2),SeriesFFTFaradey,Lfilter3->Text.ToInt(),Fd3->Text.ToInt(),
    Fp3->Text.ToInt(),Fz3->Text.ToInt());

}
//---------------------------------------------------------------------------

void __fastcall TForm1::uiFFTFoygtClick(TObject *Sender)
{
    Tr_Filter((FoygtCurveIndex->ItemIndex==0?SeriesFoygt1:SeriesFoygt2),SeriesFFTFoygt,Lfilter4->Text.ToInt(),Fd4->Text.ToInt(),
    Fp4->Text.ToInt(),Fz4->Text.ToInt());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{

    Series3->Clear();
    Series4->Clear();
    Series2->Clear();
    Series1->Clear();  // ���������
    Series5->Active=false;// ����� �� �����
    long double x=0;
    for(int i=0;i<1500;i++)  // ����� �������� �� ������
    {
    //Series2->AddXY(x,5*sin(2*x),"",clBlack);
    Series1->AddXY(x,x*x+rand()%3*sin(25*x)+rand()%2*sin(50*x),"",clRed);
    //Memo1->Lines->Add(FloatToStr(Series1->XValues->Last()) + "   " + FloatToStr(sin(x)+sin(25*x)+sin(50*x)));
    //Series4->AddXY(x,10*sin(x)+sin(50*x)+sin(100*x),"",clGreen);
    //Series4->AddXY(x,x*x,"",clBlack);
    x+=0.02+rand()%10/1000.;
    }


    // ������ �� ������� - ���������������
    // ������� - ��������
    // ����� - ������

}
//---------------------------------------------------------------------------
//-------------------�������� �����------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
    if(OpenDialog1->Execute())  // ���� �� ���-�� �������
    {
    TLineSeries * S=GetCurrentSeries2(1);  // �������� �������� ������

    S->Clear(); // ������� ������� ������, ���� �����:)

    TStringList *tts=new TStringList();  // ���� ����� ��������� �� �����
    int b;
    tts->LoadFromFile(OpenDialog1->Files->Strings[0]);// ���������
    for(int i=0;i<tts->Count;i++) // �� ���������� �����
    {
        if(tts->Strings[i].IsEmpty()) // ������ ������ ����������
            continue;
        
        for(b=1;b<tts->Strings[i].Length();b++) // � �������� ������� ���� ������
            if(tts->Strings[i].operator [](b)==' ' || tts->Strings[i].operator [](b)=='\t')
                break;
            S->AddXY(tts->Strings[i].SubString(1,b-1).ToDouble(), // ������ ����� �� ������� ��� �, ������ ����� �
            tts->Strings[i].SubString(b+1,tts->Strings[i].Length()).ToDouble(),
            "",clRed);
    }    
    }
}
//---------------------------------------------------------------------------

//---------������� ������ "���������" ��� ���-------------------------------
void __fastcall TForm1::Edit1Change(TObject *Sender)
{
    if(Edit1->Text=="")
        Edit1->Text=0;

    Series5->XValues->Value[0]=Edit1->Text.ToDouble();
    Series5->XValues->Value[1]=Edit1->Text.ToDouble();
    Series5->Repaint();              // ����������� �������
}

// ����� ��������� �������
TLineSeries * __fastcall TForm1::GetCurrentSeries2(int curve)
{
// ���������� ��� �������� �����.
    TLineSeries * SaveSeries;
    switch(Form1->PC->ActivePageIndex)
    {           // �������� ������ ������ ������ ��� ����������
        case 0:
                // ������ �������, ����� ������� ������� "FirstTab"
                SaveSeries=Form1->Series3;
            break;
        case 1:
            if(curve) // ���� �� ���������/��������� ����, �� � ����������� �� ����������� ������� ��������� ������
                SaveSeries=(ResCurveIndex->ItemIndex==0?Form1->SeriesRes1:Form1->SeriesRes2);
            else     
                SaveSeries=Form1->SeriesRes1;
            break;
        case 2:
            if(curve)
                SaveSeries=(HallCurveIndex->ItemIndex==0?Form1->SeriesHall1:Form1->SeriesHall2);
                // ������ �����
            else
                SaveSeries=Form1->SeriesHall1;
            break;
        case 3:
                // ���������������� � ��������� �������
            if(curve)
                SaveSeries=(FaradeyCurveIndex->ItemIndex==0?Form1->SeriesFaradey1:Form1->SeriesFaradey2);
            else
                SaveSeries=Form1->SeriesFaradey1;
            break;
                
        case 4:
                // ���������������� � ��������� ������
            if(curve)
                SaveSeries=(FoygtCurveIndex->ItemIndex==0?Form1->SeriesFoygt1:Form1->SeriesFoygt2);
            else
                SaveSeries=Form1->SeriesFoygt1;
            break;
            // � ������ ������ - ������������� ������������
        default:
            ShowMessage("�������� ������! �������� ��������� ���� ����������� ���������. ���������� � ������������!");
            return NULL;
    }
    return SaveSeries;  // ���������� ��������� �� �������� ������
}

 // ����� ��������� �������
 // ������������ ��� ���������� ����� � Unit2

TLineSeries * __fastcall TForm1::GetSelectedSeries(int index)
{
    TLineSeries * SaveSeries;
    switch(index)
    {
    case 0:
        SaveSeries=Series1;
        break;
    case 1:
        SaveSeries=Series2;
        break;
    case 2:
        SaveSeries=Series3;
        break;
    case 3:
        SaveSeries=Series4;
        break;
    case 4:
        SaveSeries=Series5;
        break;
    case 5:
        SaveSeries=SeriesRes1;
        break;
    case 6:
        SaveSeries=SeriesRes2;
        break;
    case 7:
        SaveSeries=SeriesFFTRes;
        break;
    case 8:
        SaveSeries=out1;
        break;
    case 9:
        SaveSeries=SeriesHall1;
        break;
    case 10:
        SaveSeries=SeriesHall2;
        break;
    case 11:
        SaveSeries=SeriesFFTHall;
        break;
    case 12:
        SaveSeries=out2;
        break;
    case 13:
        SaveSeries=SeriesFaradey1;
        break;
    case 14:
        SaveSeries=SeriesFaradey2;
        break;
    case 15:
        SaveSeries=SeriesFFTFaradey;
        break;
    case 16:
        SaveSeries=out3;
        break;
    case 17:
        SaveSeries=SeriesFoygt1;
        break;
    case 18:
        SaveSeries=SeriesFoygt2;
        break;
    case 19:
        SaveSeries=SeriesFFTFoygt;
        break;
    case 20:
        SaveSeries=out4;
        break;
    }

    return SaveSeries;
}

void __fastcall TForm1::N5Click(TObject *Sender)  // ����������
{   // � ����� ���������� �_�
    AllSeries[0]=PtrToInt(Series1);
    AllSeries[1]=PtrToInt(Series2);
    AllSeries[2]=PtrToInt(Series3);
    AllSeries[3]=PtrToInt(Series4);
    AllSeries[4]=PtrToInt(Series5);
    AllSeries[5]=PtrToInt(SeriesRes1);
    AllSeries[6]=PtrToInt(SeriesRes2);
    AllSeries[7]=PtrToInt(SeriesFFTRes);
    AllSeries[8]=PtrToInt(out1);
    AllSeries[9]=PtrToInt(SeriesHall1);
    AllSeries[10]=PtrToInt(SeriesHall2);
    AllSeries[11]=PtrToInt(SeriesFFTHall);
    AllSeries[12]=PtrToInt(out2);
    AllSeries[13]=PtrToInt(SeriesFaradey1);
    AllSeries[14]=PtrToInt(SeriesFaradey2);
    AllSeries[15]=PtrToInt(SeriesFFTFaradey);
    AllSeries[16]=PtrToInt(out3);
    AllSeries[17]=PtrToInt(SeriesFoygt1);
    AllSeries[18]=PtrToInt(SeriesFoygt2);
    AllSeries[19]=PtrToInt(SeriesFFTFoygt);
    AllSeries[20]=PtrToInt(out4);

    if(!SaveForm)
    {
    Application->CreateForm(__classid(TSaveForm), &SaveForm);
    SaveForm->Visible=true;
    }
    else
    SaveForm->BringToFront();

}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    const int SIZEM=200;
    double X[SIZEM][5]={0};
    double Xt[5][SIZEM]={0};
    if (Series3->XValues->Count()==0)
    {
        ShowMessage("������ ������!!!");
        return;
    }
    if (Series3->XValues->Count()>SIZEM)
    {
        ShowMessage("������� ����� ����� - ������� ������ �������!");
        return;
    }
    for(int i=0;i<Series3->XValues->Count();i++)
            {
            X[i][0]=Xt[0][i]=pow(Series3->XValues->Value[i],3);
            X[i][1]=Xt[1][i]=pow(Series3->XValues->Value[i],2);
            X[i][2]=Xt[2][i]=Series3->XValues->Value[i];
            X[i][3]=Xt[3][i]=1;
            X[i][4]=Xt[4][i]=-Series3->YValues->Value[i];
            }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
    SeriesRes1->Clear();
    SeriesRes2->Clear();
    SeriesFFTRes->Clear();
    out1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button6Click(TObject *Sender)
{
    SeriesHall1->Clear();  
    SeriesHall2->Clear();
    SeriesFFTHall->Clear();
    out2->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button7Click(TObject *Sender)
{
    SeriesFaradey1->Clear();
    SeriesFaradey2->Clear();
    SeriesFFTFaradey->Clear();
    out3->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button8Click(TObject *Sender)
{
    SeriesFoygt1->Clear();
    SeriesFoygt2->Clear();
    SeriesFFTFoygt->Clear();
    out4->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyPress(TObject *Sender, char &Key)
{
    if(Key=='.')
        Key=',';
    if(((int)Key<48 || (int)Key>57) && Key!=8 && Key!=',')
        Key=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImpulsKillerClick(TObject *Sender)
{
    double x,y;
    x=Interval1->Text.ToDouble();
    y=Interval2->Text.ToDouble();
    KillImpulps(SeriesRes1,x,y);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
    for(int i=0;i<SeriesRes1->XValues->Count();i++)
        SeriesRes1->XValues->Value[i]*=10;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button10Click(TObject *Sender)
{
    Series3->Active=!Series3->Active;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button12Click(TObject *Sender)
{
    // ���������� ���������� ����� �� ������� � ��� ����
    int length=SeriesRes1->XValues->Count();

    double *x=new double [length];
    double *y=new double [length];
    double *x1=new double [2*length];
    double *y1=new double [2*length];
     
    for(int i=0;i<length;i++)
     {
            x[i]=SeriesRes1->XValues->Value[i];
            y[i]=SeriesRes1->YValues->Value[i];
     }

     for(int i=0,j=0;i<length;i++,j+=2)
     {
            x1[j]=x[i];
            y1[j]=y[i];
            if (i==length-1)
            break;
            x1[j+1]=(x[i]+x[i+1])/2;
            y1[j+1]=(y[i]+y[i+1])/2;        
     }
     SeriesRes1->Clear();
     for(int i=0;i<2*length;i++)
     SeriesRes1->AddXY(x1[i],y1[i],"",clRed);

    delete [] x;
    delete [] y;
    delete [] x1;
    delete [] y1;
}
//---------------------------------------------------------------------------
void fillspace(double *xm,double *ym, int l,int r)
{

    if(abs(r-l)==1)
    {
    xm[l+1]=(xm[l]+xm[r])/2;
    ym[l+1]=(ym[l]+ym[r])/2;
    return;
    }
    xm[(l+r)/2]=(xm[l]+xm[r])/2;
    ym[(l+r)/2]=(ym[l]+ym[r])/2;
    fillspace(xm,ym,l,(l+r)/2);
    fillspace(xm,ym,(l+r)/2,r);
    }
    //---------------------------------------------------------------------------
    // ������� ����� ��������������� ���������
    void KillImpulps(TLineSeries * a,double x, double y)
    {

    int length=a->XValues->Count();
    if(length==0)
    {
    ShowMessage("������ ������!!!");
    return;
    }
    double *xm=new double [length];
    double *ym=new double [length];

    for(int i=0;i<length;i++)
     {
            xm[i]=a->XValues->Value[i];
            ym[i]=a->YValues->Value[i];
     }
    // ������� ���������
    //x=Interval1->Text.ToDouble();
    //y=Interval2->Text.ToDouble();
    double dx=fabs(xm[0]-x);// �������� ����� ��������� �����
    int i;
    double k=0;
    for(i=0;i<length;i++)
    {
            k=fabs(xm[i]-x);
            if(k>dx)  // ��� ������ ����� �� ���������� - ������� �� �����
            {
                 break;
            }
            dx=k;
    }
    int i1=i; // ���������� ������ ������
    dx=fabs(xm[i]-y);// ���� ������
    for(;i<length;i++)
    {
            k=fabs(xm[i]-y);
            if(k>dx)
            {
                    break;
            }
            dx=k;
    }
    int i2=i;
    int left=i1;
    int right=i2;

    fillspace(xm,ym,i1,i2);

     a->Clear();
     for(int i=0;i<length;i++)
     {
            a->AddXY(xm[i],ym[i],"",clRed);
                    }
    delete [] xm;
    delete [] ym;
}
// � ����������� �� �������� ������� ���������� ����� ��������� �������.
void __fastcall TForm1::N9Click(TObject *Sender)
{
    int a=PC->ActivePageIndex;
    switch (a)
    {
    case 1:
    Button5Click(Sender);
    case 2:
    Button6Click(Sender);
    case 3:
    Button7Click(Sender);
    case 4:
    Button8Click(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
    int len=SeriesRes1->XValues->Count();

    double *x= new double [len];
    for(int i=0;i<len;i++)
    x[i]=SeriesRes1->XValues->Value[i];

    double *y= new double [len];
    for(int i=0;i<len;i++)
    y[i]=SeriesRes1->YValues->Value[i];

    double *yf= new double [len];
    for(int i=0;i<len;i++)
    yf[i]=SeriesFFTRes->YValues->Value[i];

    for(int i=0;i<len;i++)
    {
    y[i]=fabs(fabs(y[i])-fabs(yf[i]));
    SeriesRes2->AddXY(x[i],y[i],"",clBlack);
    }

    double M=0,sko=0;

    for(int i=0;i<len;i++)
    {
    M+=y[i]/len;
    }
    for(int i=0;i<len;i++)
    {
    sko+=pow(y[i]-M,2);
    }
    sko/=len;
    sko=sqrt(sko);

    Memo2->Lines->Add("M=" + FloatToStr(M) + "\n");
    Memo2->Lines->Add("sko=" + FloatToStr(sko) + "\n");

    delete[] x;
    delete[] y;
    delete[] yf;

}
//---------------------------------------------------------------------------

