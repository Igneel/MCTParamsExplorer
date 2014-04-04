//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------

#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Chart.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Series.hpp>
#include <StdCtrls.hpp>

#include "Unit2.h"
#include "ExtrapolateUnit.h"
#include "FilteringUnit.h"
#include <math.h>
#include "MagneticFieldDependence.h"
#include "LCard.h"
#include "FeatUnit.h"

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TPageControl *PC;
    TTabSheet *FirstTab;
    TLabel *Label2;
    TLabel *Label4;
    TLabel *Label6;
    TLabel *Label7;
    TChart *Chart1;
    TLineSeries *Series2;
    TMemo *Memo1;
    TButton *uiControl;
    TComboBox *Chan1;
    TComboBox *Chan2;
    TEdit *uiBlockSize2;
    TEdit *uiFrenq;
    TTabSheet *Resistance;
    TChart *ChartResist;
    TButton *uiResControl;
    TEdit *CurrentRes;
    TLabel *Label9;
    TLineSeries *SeriesRes1;
    TButton *uiResFeat;
    TMemo *Memo2;
    TRadioGroup *ResCurveIndex;
        TButton *bClear;
    TTabSheet *HallEffect;
    TTabSheet *Faradey;
    TTabSheet *Foygt;
    TLineSeries *SeriesRes2;
    TChart *ChartHall;
    TRadioGroup *HallCurveIndex;
    TButton *uiHallFeat;
    TButton *uiHallControl;
    TMemo *Memo3;
    TEdit *CurrentHall;
    TLabel *Label11;
    TLineSeries *SeriesHall1;
    TLineSeries *SeriesHall2;
    TChart *ChartFaradey;
    TLineSeries *SeriesFaradey1;
    TLineSeries *SeriesFaradey2;
    TLabel *Label12;
    TEdit *CurrentFaradey;
    TComboBox *GainKoefFaradey;
    TLabel *Label13;
    TRadioGroup *FaradeyCurveIndex;
    TButton *uiFaradeyControl;
    TButton *uiFaradeyFeat;
    TMemo *Memo4;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TChart *ChartFoygt;
    TLineSeries *SeriesFoygt1;
    TLineSeries *SeriesFoygt2;
    TComboBox *GainKoefFoygt;
    TLabel *Label10;
    TEdit *CurrentFoygt;
    TLabel *Label14;
    TRadioGroup *FoygtCurveIndex;
    TButton *uiFoygtControl;
    TButton *uiFoygtFeat;
    TMemo *Memo5;
    TLineSeries *SeriesFFTFaradey;
    TButton *bFilterRes;
    TLineSeries *SeriesFFTRes;
    TLineSeries *Series1;
    TLineSeries *Series4;
    TEdit *eLengthFilterRes;
    TButton *uiFFTHall;
    TButton *uiFFTFaradey;
    TButton *uiFFTFoygt;
    TLineSeries *SeriesFFTHall;
    TStatusBar *StatusBar;
    TLineSeries *SeriesFFTFoygt;
    TLabel *Label15;
        TLineSeries *Series3;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
    TMemo *Memo6;
        TLineSeries *out1;
        TLineSeries *out2;
        TLineSeries *out3;
        TLineSeries *out4;
        TButton *bTest;
    TEdit *eSamplingFRes;
    TEdit *eBandWidthFRes;
    TEdit *eAttenuationFRes;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
    TButton *bClearPlotsRes;
        TButton *Button6;
        TButton *Button7;
        TButton *Button8;
        TLabel *Label16;
    TEdit *eLengthFilterHall;
        TLabel *Label22;
    TEdit *eSamplingFHall;
    TEdit *eBandWidthFHall;
        TLabel *Label23;
        TLabel *Label24;
    TEdit *eAttenuationFHall;
        TLabel *Label17;
        TEdit *Lfilter3;
        TEdit *Fd3;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *Fp3;
        TEdit *Fz3;
        TLabel *Label27;
        TEdit *Fd4;
        TLabel *Label18;
        TEdit *Lfilter4;
        TLabel *Label28;
        TLabel *Label29;
        TEdit *Fp4;
        TEdit *Fz4;
        TLabel *Label30;
        TEdit *Interval1;
        TEdit *Interval2;
        TButton *ImpulsKiller;
    TButton *bMultuplyB;
        TMenuItem *N8;
        TMenuItem *N101;
        TButton *Button10;
        TButton *Button11;
        TMenuItem *N9;
        TMenuItem *N10;
        TButton *Button13;
    TMenuItem *N11;
    TLabeledEdit *PowPolinomRes;
    TLabeledEdit *PowPolinomHall;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall uiControlClick(TObject *Sender);
    void __fastcall uiResFeatClick(TObject *Sender);
        void __fastcall bClearClick(TObject *Sender);
    void __fastcall uiHallFeatClick(TObject *Sender);
    void __fastcall uiFaradeyFeatClick(TObject *Sender);
    void __fastcall uiFoygtFeatClick(TObject *Sender);
    void __fastcall bFilterResClick(TObject *Sender);
    void __fastcall N5Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);

    TLineSeries * __fastcall GetCurrentSeries2(int curve);
    TLineSeries * __fastcall GetSelectedSeries(int curve);
        void __fastcall bClearPlotsResClick(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
        void __fastcall ImpulsKillerClick(TObject *Sender);
        void __fastcall bMultuplyBClick(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall N9Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall bTestClick(TObject *Sender);
    void __fastcall N11Click(TObject *Sender);
    

private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void TForm1::UpdatePlots();

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;


extern int AllSeries[26];   

extern LCardADC *adc;

void KillImpulps(TLineSeries * a,double Left, double Right);

//---------------------------------------------------------------------------
#endif
