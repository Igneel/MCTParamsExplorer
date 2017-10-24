//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <cmath>
#include <string>
//---------------------------------------------------------------------------
#include "Unit2.h"
#include "ExtrapolateUnit.h"
#include "FilteringUnit.h"
#include "NoiseUnit.h"
#include "clMagneticFieldDependences.h"

//--------------------Подключение библиотеки фильтрации.---------------------

//#include "filter.h"

//---------------------------------------------------------------------------


class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PG;
	TTabSheet *TabSheet1;
	TTabSheet *Графики;
	TLabeledEdit *eKFactor;
	TLabeledEdit *eHeavyHoleConcentration;
	TLabeledEdit *eAFactor;
	TLabeledEdit *eSampleThickness;
	TLabeledEdit *eCurrentIntensity;
	TLabeledEdit *eTemperature;
	TLabeledEdit *eMolarCompositionCadmium;
	TLabeledEdit *eCBRatio;
	TButton *bWhiteNoiseGenerator;
	TButton *bGaussianNoiseGenerator;
	TButton *bCalculateCarrierParams;
	TMemo *mDebug;
	TButton *BuildingPlots;
	TStringGrid *g_Nz_par;
	TEdit *g_hsize;
	TLabel *Label1;
	TChart *Chart1;
	TLineSeries *Series1;
	TChart *Chart2;
	TLineSeries *Series2;
	TChart *Chart4;
	TLineSeries *Series4;
	TChart *Chart3;
	TLineSeries *Series3;
	TButton *bSaveAllPoints;
	TEdit *Edit5;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *Edit6;
	TEdit *Edit1;
	TLabel *Label2;
	TLabel *Label4;
	TLabel *Label7;
	TEdit *Edit2;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *Edit4;
	TEdit *Edit3;
	TChart *Chart5;
	TLineSeries *gSeriesIdealParamsSxx;
	TLineSeries *gSeriesParamsWithNoiseSxx;
	TChart *Chart6;
	TLineSeries *gSeriesIdealParamsS_eff;
	TLineSeries *gSeriesParamsWithNoiseS_eff;
	TChart *Chart7;
	TLineSeries *gSeriesIdealParamsUs;
	TLineSeries *gSeriesParamsWithNoiseUs;
	TChart *Chart8;
	TLineSeries *gSeriesIdealParamsSxy;
	TLineSeries *gSeriesParamsWithNoiseSxy;
	TChart *Chart9;
	TLineSeries *gSeriesIdealParamsRh_eff;
	TLineSeries *gSeriesParamsWithNoiseRh_eff;
	TChart *Chart10;
	TLineSeries *gSeriesIdealParamsUy;
	TLineSeries *gSeriesParamsWithNoiseUy;
	TButton *bAutomaticCalculation;
	TEdit *eStepByTemperature;
	TLabel *Label11;
	TButton *bFilteringPlots;
	TLineSeries *gSeriesFilteredUs;
	TLineSeries *gSeriesFilteredUy;
	TButton *bSaveElevenPoints;
	TGroupBox *gbSavingPlot;
	TRadioButton *rbFilteredUPlot;
	TButton *bLoadingPlots;
	TEdit *eFilterLength;
	TRadioButton *rbIdealUPlot;
	TButton *bSaveFilmParams;
	TLabel *Label12;
	TButton *bTestingSomething;
	TLineSeries *Series9;
	TLineSeries *gSeriesExtrapolatedUs;
	TLineSeries *gSeriesExtrapolatedUy;
	TLineSeries *gSeriesFilteredParamsSxx;
	TLineSeries *gSeriesExtrapolatedParamsSxx;
	TLineSeries *gSeriesFilteredParamsSxy;
	TLineSeries *gSeriesExtrapolatedParamsSxy;
	TLineSeries *gSeriesExtrapolatedParamsS_eff;
	TLineSeries *gSeriesFilteredParamsS_eff;
	TLineSeries *gSeriesExtrapolatedParamsRh_eff;
	TLineSeries *gSeriesExtrapolatedParamsUs;
	TLineSeries *gSeriesExtrapolatedParamsUy;
	TLineSeries *gSeriesFilteredParamsRh_eff;
	TLineSeries *gSeriesFilteredParamsUs;
	TLineSeries *gSeriesFilteredParamsUy;
	TRadioButton *rbIdealTenzorPlot;
	TRadioButton *rbFilteredTenzor;
	TRadioButton *rbExtrapolatedU;
	TRadioButton *rbExtrapolatedTenzor;
	TRadioButton *rbNoisyU;
	TRadioButton *rbNoisyTenzor;
	TGroupBox *gbExtrapolatingMode;
	TRadioButton *rbFilteredOnly;
	TRadioButton *rbFilteredNoisyExtrapolated;
	TGroupBox *GroupBox1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TLabeledEdit *LabeledEdit1;
	TLabeledEdit *LabeledEdit2;
	TSaveDialog *sg1;
	TEdit *Edit7;
	TEdit *Edit8;
	TLabel *Label13;
	TButton *Button1;
	TRadioButton *RadioButton3;
	TLabeledEdit *editNumberNoises;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bCalculateCarrierParamsClick(TObject *Sender);
	void __fastcall bGaussianNoiseGeneratorClick(TObject *Sender);
	void __fastcall bSaveAllPointsClick(TObject *Sender);
	void __fastcall g_Nz_parKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall bWhiteNoiseGeneratorClick(TObject *Sender);
	void __fastcall BuildingPlotsClick(TObject *Sender);
	void __fastcall bAutomaticCalculationClick(TObject *Sender);
	void __fastcall bFilteringPlotsClick(TObject *Sender);
	void __fastcall bSaveElevenPointsClick(TObject *Sender);
	void __fastcall bLoadingPlotsClick(TObject *Sender);
	void __fastcall bSaveFilmParamsClick(TObject *Sender);
	void __fastcall bTestingSomethingClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button1Click(TObject *Sender);







private:	// User declarations
public:		// User declarations
	void automaticCalculationHelper(UnicodeString SaveFileName);
    void RoundM(long double * x,int length);
	void chooseAndSaveData(FileSaveMode mode);
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
long double ownConcentrationOfElectrons(long double T, long double x);

/*HMODULE hDLL = NULL;
// определяем тип "указатель на функцию"
typedef double __cdecl (*dll_func)(long double *inB,long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh);

dll_func TrForMassiveFilter = NULL; */

void ParamsKRT(void);
//---------------------------------------------------------------------------
#endif
