//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------


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
#include <math.h>
#include <TeEngine.hpp>
#include <Grids.hpp>
#include <oxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#pragma hdrstop

#include "Unit2.h"
#include "SignalShiftForm.h"
#include "ExtrapolateUnit.h"
#include "FilteringUnit.h"  
#include "MagneticFieldDependence.h"
#include "LCard.h"
#include "mobilityspectrum.h"


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
    TChart *Chart1;
    TLineSeries *Series2;
    TButton *uiControl;
    TEdit *uiBlockSize;
    TEdit *uiFrenq;
    TTabSheet *Resistance;
    TChart *ChartResist;
    TButton *uiResControl;
    TLineSeries *SeriesRes1;
    TRadioGroup *ResCurveIndex;
        TButton *bClear;
    TTabSheet *HallEffect;
    TTabSheet *Faradey;
    TTabSheet *Foygt;
    TLineSeries *SeriesRes2;
    TChart *ChartHall;
    TRadioGroup *HallCurveIndex;
    TButton *uiHallControl;
    TMemo *Memo3;
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
    TMemo *Memo5;
    TLineSeries *SeriesFFTFaradey;
    TButton *bFilterRes;
    TLineSeries *SeriesFFTRes;
    TLineSeries *Series1;
    TLineSeries *Series4;
    TButton *uiFFTHall;
    TButton *uiFFTFaradey;
    TButton *uiFFTFoygt;
    TLineSeries *SeriesFFTHall;
    TStatusBar *StatusBar;
    TLineSeries *SeriesFFTFoygt;
        TLineSeries *Series3;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
        TLineSeries *out1;
        TLineSeries *out2;
        TLineSeries *out3;
        TLineSeries *out4;
    TButton *bClearPlotsRes;
    TButton *bClearPlotHall;
        TButton *Button7;
        TButton *Button8;
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
        TMenuItem *N8;
        TMenuItem *N101;
        TMenuItem *N9;
        TMenuItem *N10;
        TButton *Button13;
    TMenuItem *N11;
    TCheckBox *CheckBox1;
    TButton *bApplyADCSettings;
    TBarSeries *Series5;
    TLabel *LabelChan1;
    TLabel *LabelChan2;
    TLabel *LabelChan3;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TComboBox *ComboBox1;
    TComboBox *ComboBox2;
    TComboBox *ComboBox3;
    TComboBox *ComboBox4;
    TComboBox *ComboBox5;
    TComboBox *ComboBox6;
    TLabel *Label3;
    TLabel *Label1;
    TLabel *Label5;
    TLabel *Label6;
    TButton *bUniteDependence;
    TMemo *Memo6;
    TLabeledEdit *uiShiftValue;
    TButton *bResShiftCurve;
    TRadioGroup *uiDataKind;
    TLabeledEdit *uiHallShiftValue;
    TRadioGroup *uiHallDataKind;
    TButton *bShiftHallCurve;
    TLabel *Label7;
    TComboBox *uiLeftBound;
    TComboBox *uiRightBound;
    TLabel *Label8;
    TComboBox *uiHallLeftBound;
    TComboBox *uiHallRightBound;
    TButton *uiCalculateTenzor;
    TGroupBox *GroupBox1;
    TLabel *Label35;
    TEdit *uiCurrent;
    TLabeledEdit *uiTemperature;
    TLabeledEdit *uiInventoryNumber;
    TLabeledEdit *uiSampleLength;
    TLabeledEdit *uiSampleWidth;
    TLabeledEdit *uiSampleThickness;
    TGroupBox *GroupBox2;
    TEdit *eLengthFilterRes;
    TEdit *eBandWidthFRes;
    TEdit *eAttenuationFRes;
    TLabeledEdit *PowPolinomRes;
    TEdit *eSamplingFRes;
    TLabel *Label21;
    TLabel *Label15;
    TLabel *Label20;
    TLabel *Label19;
    TGroupBox *GroupBox3;
    TLabeledEdit *PowPolinomHall;
    TEdit *eAttenuationFHall;
    TEdit *eBandWidthFHall;
    TEdit *eSamplingFHall;
    TEdit *eLengthFilterHall;
    TLabel *Label16;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
        TButton *Button2;
    TButton *Button1;
    TButton *bMobilitySpectrum;
        TMenuItem *N12;
        TChart *Chart2;
        TLineSeries *Series6;
        TChart *Chart3;
        TLineSeries *LineSeries1;
    TMenuItem *N13;
    TPopupMenu *PopupMenu1;
    TMenuItem *N14;
    TMenuItem *N15;
    TMenuItem *N16;
    TMenuItem *N17;
    TMemo *ErrorLog;
    TLabel *Label9;
    TLabel *Label11;
    TLabeledEdit *eMedianFilterSize;
    TLabeledEdit *uiSamplingFreq;
    TComboBox *ComboBox7;
    TComboBox *ComboBox8;
    TMenuItem *N18;
    TMenuItem *N19;
    TButton *Button4;
    TTabSheet *DensAndMobDetermination;
    TChart *Chart4;
    TChart *Chart5;
    TChart *Chart6;
    TStringGrid *MobSpecResults;
    TButton *btnMobilitySpectrum;
    TButton *btnMultiCarrierFit;
    TLabeledEdit *LabeledEdit1;
    TLabeledEdit *LabeledEdit2;
    TButton *btnSaveCarrierParams;
    TLineSeries *ChSpHoleComponent;
    TLineSeries *ChspElecComponent;
    TLineSeries *ChSxx_theor;
    TPointSeries *ChSxxExper;
    TLineSeries *ChSxy_theor;
    TPointSeries *ChSxyExper;
    TStringGrid *FitResults;
    TXMLDocument *XMLsettings;
    TProgressBar *ProgressBar1;
    TMenuItem *N20;
    TMenuItem *N21;
    TMenuItem *N22;
    TMenuItem *N23;
    TMenuItem *N24;
    TMenuItem *N25;
    TMenuItem *N26;
    TButton *Button3;
    TButton *Button5;
    TCheckBox *CheckBox2;
    TCheckBox *CheckBox3;
    TButton *SmartCalc;
        TButton *Button6;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall uiControlClick(TObject *Sender);
        void __fastcall bClearClick(TObject *Sender);
    void __fastcall bFilterResClick(TObject *Sender);
    void __fastcall N5Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);

    TLineSeries * __fastcall GetSelectedSeries(int curve);
        void __fastcall bClearPlotsResClick(TObject *Sender);
        void __fastcall bClearPlotHallClick(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
        void __fastcall ImpulsKillerClick(TObject *Sender);
        void __fastcall N9Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall N11Click(TObject *Sender);
    void __fastcall bApplyADCSettingsClick(TObject *Sender);
    void __fastcall bUniteDependenceClick(TObject *Sender);
    void __fastcall CurrentResChange(TObject *Sender);
    void __fastcall uiCalculateTenzorClick(TObject *Sender);
    void __fastcall eLengthFilterResChange(TObject *Sender);
    void __fastcall eLengthFilterHallChange(TObject *Sender);
    void __fastcall HallCurveIndexClick(TObject *Sender);
    void __fastcall ResCurveIndexClick(TObject *Sender);
    void __fastcall bResShiftCurveClick(TObject *Sender);
    void __fastcall bShiftHallCurveClick(TObject *Sender);
    void __fastcall ComboBox5Change(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
        void __fastcall Series1Click(TChartSeries *Sender, int ValueIndex,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall N12Click(TObject *Sender);
    void __fastcall N13Click(TObject *Sender);
    void __fastcall N17Click(TObject *Sender);
    void __fastcall N16Click(TObject *Sender);
    void __fastcall uiFrenqChange(TObject *Sender);
    void __fastcall N19Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall btnMultiCarrierFitClick(TObject *Sender);
    void __fastcall btnMobilitySpectrumClick(TObject *Sender);
    void __fastcall N25Click(TObject *Sender);
    void __fastcall N26Click(TObject *Sender);
    void __fastcall eMedianFilterSizeKeyPress(TObject *Sender, char &Key);
    void __fastcall XMLsettingsBeforeOpen(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall openFileWithSignal(AnsiString filename);
    void __fastcall calculateTenzor();
    void __fastcall N21Click(TObject *Sender);
    void __fastcall btnSaveCarrierParamsClick(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall SmartCalcClick(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
    

private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void UpdatePlots();
    MagneticFieldDependence * InitParams();
    MagneticFieldDependence ** ActiveParams();
    void DeleteActiveParams();
    void UpdateSampleDescription(TStringList *Names,TStringList *Values);
    void concatDependence();

    void saveSettings();
    void loadSettings();
    void TForm1::getAndDisplayMultiCarrierFitResults(MagneticFieldDependence * p);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;


int AllSeries[26];   

extern LCardADC *adc;

void Gist(std::vector<long double> & in);
void addPeak(TChartSeries *Sender,int ValueIndex);

void KillImpulps(TLineSeries * a,double Left, double Right);

//---------------------------------------------------------------------------
#endif
