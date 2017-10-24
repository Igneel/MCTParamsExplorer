//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <System.IOUtils.hpp>
#include <System.SysUtils.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------




bool silentModeEnabled=false; // если флаг равен тру - то функции сохранения не спрашивают имя файла
// флаг используется для крупных расчетов))

bool isRoundNeeded = false; // ещё один флаг, показывает нужно ли округление при сохранении.
/*

TO DO:

1. Перед фильтрацией прикрутить нормальное достроение графика в отрицательную область магнитного поля.+
2. Попробовать использовать экстраполяцию ПОСЛЕ фильтрации.+
	Ну и как-то не очень наверное
	Нужно испробовать экстраполяцию по фильтрованным и нефильтрованным данным одновременно.

3. Подбор длины фильтра относительно величины отклонения точки (0,0) от истинного значения.
4. Коррекция смещения точки, проходящей через начало координат.
5. Апгрейд функций сохранения и загрузки данных.


На данный момент код, отвечающий за расчет параметров переписан с использованием классов.

*/

 // */

int NumberOfPoints=201;//11;  // количество точек
const int NumberOfNumbersAfterPoint=4; // параметр округления.

const int NumberOfCarrierTypes=3;
long double h; // величина шага


__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//------------------------------------------------------------------------------

// те самые объекты.
clMagneticFieldDependences *IdealParams=0;
clMagneticFieldDependences *ParamsWithNoise=0;
clMagneticFieldDependences *FilteredParams=0;
clMagneticFieldDependences *ExtrapolatedParams=0;

//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
/*
hDLL = LoadLibraryA("K:\\Дела\\Институт физики полупроводников\\La-7\\MctErrorEstimating\\filter.dll");
if (!hDLL) {
  ShowMessage("Невозможно загрузить filter.dll");
  return;
}



// пытаемся найти в таблице экспорта необходимую нам функцию
TrForMassiveFilter = (dll_func)GetProcAddress(hDLL, "_TrForMassiveFilter");
// обратите внимание на название функции

if (!TrForMassiveFilter) {
  ShowMessage("Невозможно найти функцию TrForMassiveFilter");
  return;
}

 */

srand(time(NULL));

g_Nz_par->Cells[0][1]="электроны";
g_Nz_par->Cells[0][2]="легкие дырки";
g_Nz_par->Cells[0][3]="тяжелые дырки";
g_Nz_par->Cells[1][0]="концентрация";
g_Nz_par->Cells[2][0]="подвижность";

}
//---------------------------------------------------------------------------


void ParamsKRT(void)
{

	h=Form1->g_hsize->Text.ToDouble(); // сохраняем значение шага
    NumberOfPoints=2/h+1;
	//--------------------------Классы------------------------------------------
	IdealParams=new clMagneticFieldDependences(NumberOfPoints,h,
	Form1->eMolarCompositionCadmium->Text.ToDouble(),
	Form1->eTemperature->Text.ToDouble(),Form1->eHeavyHoleConcentration->Text.ToDouble(),
	Form1->eAFactor->Text.ToDouble(),Form1->eKFactor->Text.ToDouble(),
	Form1->eSampleThickness->Text.ToDouble(),Form1->eCBRatio->Text.ToDouble(),
	Form1->eCurrentIntensity->Text.ToDouble(),NumberOfCarrierTypes);

	Form1->g_Nz_par->Cells[1][1]=FloatToStr(IdealParams->carrierParams->getConcentration(2));
	Form1->g_Nz_par->Cells[2][1]=FloatToStr(IdealParams->carrierParams->getMobility(2));
	Form1->g_Nz_par->Cells[1][2]=FloatToStr(IdealParams->carrierParams->getConcentration(1));
	Form1->g_Nz_par->Cells[2][2]=FloatToStr(IdealParams->carrierParams->getMobility(1));
	Form1->g_Nz_par->Cells[1][3]=FloatToStr(IdealParams->carrierParams->getConcentration(0));
	Form1->g_Nz_par->Cells[2][3]=FloatToStr(IdealParams->carrierParams->getMobility(0));
	//--------------------------Классы------------------------------------------

	//Form1->bCalculateCarrierParams->Enabled=true;
	Form1->BuildingPlots->Enabled=1;

	//--------------------------------------------------------------------------
}

// расчет тензоров проводимости
void __fastcall TForm1::bCalculateCarrierParamsClick(TObject *Sender)
{
	if(IdealParams!=0)
	delete IdealParams;
	/*

	Читаем концентрации и подвижности с формы.
	Читаем шаг.
	Заполняем магнитное поле.
	Рассчитываем компоненты тензора по значениям параметров плёнки.
	Выводим их на экран
	Рассчитываем эффективные значения.
	Получаем измеряемые сигналы.


	*/
	ParamsKRT();

	//------------------Классы--------------------------------------------------
	IdealParams->calculateMagneticFieldDependences();

	IdealParams->constructPlotFromTwoMassive(SXX,Series1,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,Series2,clRed);

	IdealParams->constructPlotFromTwoMassive(SXX,gSeriesIdealParamsSxx,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,gSeriesIdealParamsSxy,clRed);

	IdealParams->constructPlotFromTwoMassive(S_EFF,gSeriesIdealParamsS_eff,clRed);
	IdealParams->constructPlotFromTwoMassive(RH_EFF,gSeriesIdealParamsRh_eff,clRed);

	IdealParams->constructPlotFromTwoMassive(US,gSeriesIdealParamsUs,clRed);
	IdealParams->constructPlotFromTwoMassive(UY,gSeriesIdealParamsUy,clRed);

	//------------------Классы--------------------------------------------------
	bGaussianNoiseGenerator->Enabled=1;
}
//---------------------------------------------------------------------------
// округление результатов при сохранении, необходимо потому что если этого не сделать
// метод будет считать что мы с этой точностью измеряли - и мало что найдет
void TForm1::RoundM(long double * x,int length)
{
	int S=pow(10,NumberOfNumbersAfterPoint);
	for(int i=0;i<length;i++)
	{
		int n=(int)(x[i]*S)%10;
		if(n<5)
			x[i]=floorl(x[i]*S)/S;
		else
			x[i]=ceill(x[i]*S)/S;
	}
}


// генератор шума
void __fastcall TForm1::bGaussianNoiseGeneratorClick(TObject *Sender)
{
	/*
	К идеальным значениям сигнала добавляется шум.
	Выводится на экран.
	*/
	//--------------------Классы----------------------------------------------------
	Edit5->Text=FloatToStr(100.0/(long double)(StrToFloat(Edit6->Text)*
		fabs(IdealParams->getSignalUy()[NumberOfPoints-1]))); // задаем коэффициенты



	std::vector<long double> vz; // М СКО и СКО в %
	//long double vz[6]={0};  // М СКО и СКО в %
	if(ParamsWithNoise!=0)
		delete ParamsWithNoise;
	ParamsWithNoise= new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	if(RadioButton1->Checked)      // нормальный шум
	{
	ParamsWithNoise->modifySignals(ShumAdding,IdealParams->getSignalUs(),
	IdealParams->getSignalUy(),vz,Edit5->Text.ToDouble());
	}
	if(RadioButton2->Checked)  // дискретный шум
	{
		 ParamsWithNoise->modifySignals(QuantumShumAdding,IdealParams->getSignalB(),
		 IdealParams->getSignalUs(),IdealParams->getSignalUy(),
		 vz,Edit5->Text.ToDouble(),LabeledEdit1->Text.ToDouble(),LabeledEdit2->Text.ToInt());
	}
	if(RadioButton3->Checked) // оба
	{
		ParamsWithNoise->modifySignals(ShumAdding,IdealParams->getSignalUs(),
		IdealParams->getSignalUy(),vz,Edit5->Text.ToDouble());
		std::vector<long double> *B= new std::vector<long double>;
		std::vector<long double> *Us=new std::vector<long double>;
		std::vector<long double> *Uy =new std::vector<long double>;
		for(int i=0;i<ParamsWithNoise->getSignalB().size();++i)
		{
			B->push_back(ParamsWithNoise->getSignalB()[i]);
			Us->push_back(ParamsWithNoise->getSignalUs()[i]);
			Uy->push_back(ParamsWithNoise->getSignalUy()[i]);
		}
			ParamsWithNoise->modifySignals(QuantumShumAdding,*B,
			*Us,*Uy,
			 vz,Edit5->Text.ToDouble(),LabeledEdit1->Text.ToDouble(),LabeledEdit2->Text.ToInt());


		delete B;
		delete Us;
		delete Uy;
	}


	mDebug->Lines->Add(FloatToStr(vz[0]));
	Edit1->Text=FloatToStr(vz[1]); // СКО
	Edit3->Text=FloatToStr(vz[2]);

	mDebug->Lines->Add(FloatToStr(vz[3]));
	Edit2->Text=FloatToStr(vz[4]); // СКО
	Edit4->Text=FloatToStr(vz[5]);

	ParamsWithNoise->constructPlotFromTwoMassive(US,Series3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(UY,Series4,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(US,gSeriesParamsWithNoiseUs,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(UY,gSeriesParamsWithNoiseUy,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(S_EFF,gSeriesParamsWithNoiseS_eff,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(RH_EFF,gSeriesParamsWithNoiseRh_eff,clBlack);

	ParamsWithNoise->constructPlotFromTwoMassive(SXX,gSeriesParamsWithNoiseSxx,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(SXY,gSeriesParamsWithNoiseSxy,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(SXX,Series3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(SXY,Series4,clRed);

    // расчет СКО и мат. ожидания.

	long double sko1=Sko(IdealParams->getSxx(),ParamsWithNoise->getSxx(),NumberOfPoints);
	long double sko2=Sko(IdealParams->getSxy(),ParamsWithNoise->getSxy(),NumberOfPoints);
	long double* shx= new long double [NumberOfPoints];
	long double* shy= new long double [NumberOfPoints];
	//long double shx[NumberOfPoints]={0};
	//long double shy[NumberOfPoints]={0};
	for(int i=0;i<NumberOfPoints;i++)
	{
		shx[i]=ParamsWithNoise->getSxx()[i]-IdealParams->getSxx()[i];
		shy[i]=ParamsWithNoise->getSxy()[i]-IdealParams->getSxy()[i];
	}
	long double mx=Mo(shx,NumberOfPoints);
	long double my=Mo(shy,NumberOfPoints);
	Edit1->Text=FloatToStr(sko1);
	Edit2->Text=FloatToStr(sko2);
	Edit3->Text=FloatToStr(mx/Mo(IdealParams->getSxx(),NumberOfPoints)*100);
	Edit4->Text=FloatToStr(my/Mo(IdealParams->getSxy(),NumberOfPoints)*100);
	delete[] shx;
	delete[] shy;
    bFilteringPlots->Enabled=true;
//-----------------------------Классы_------------------------------------------
}
//---------------------------------------------------------------------------



// сохранение результатов
void __fastcall TForm1::bSaveAllPointsClick(TObject *Sender)
{
	chooseAndSaveData(ALL_POINTS);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::g_Nz_parKeyPress(TObject *Sender, wchar_t &Key)
{
	if(Key=='.') Key=',';
	if(Key=='e') Key='E';
	if(Key=='Е') Key='E';  // русскую Е заменяю на английскую
	if (Key=='е') Key='E';
	if ((Key>='0' && Key<='9') || Key==',' || Key=='\b' || Key=='E' ) ;
	else
		Key=0;
}
//---------------------------------------------------------------------------
// генератор белого шума, не знаю нужен ли он нам вообще)
void __fastcall TForm1::bWhiteNoiseGeneratorClick(TObject *Sender)
{
/*long double x[l];
long double y[l];
long double y1[l];
srand(time(NULL));
for(int i=0;i<l;i++)
{
y[i]=rand()%1000/250./Edit5->Text.ToDouble();
y1[i]=rand()%1000/250./Edit6->Text.ToDouble();
}
Memo1->Lines->Add(FloatToStr(Mo(y,l)));
Memo1->Lines->Add(FloatToStr(Mo(y1,l)));
for(int i=0;i<l;i++)
{
y[i]=Series1->YValues->Value[i]+y[i];
}
Series3->Clear() ;

for(int i=0;i<l;i++)
Series3->AddXY(B[i],y[i],"",clRed);




for(int i=0;i<l;i++)
{
y1[i]=Series2->YValues->Value[i]+y1[i];
}
Series4->Clear() ;

for(int i=0;i<l;i++)
Series4->AddXY(B[i],y1[i],"",clRed);

 Edit1->Text=FloatToStr(Sko(sxx,y,l));
 Edit2->Text=FloatToStr(Sko(sxy,y1,l));

 // СКО %  от математического ожидания
 Edit3->Text=FloatToStr(Edit1->Text.ToDouble()/Mo(sxx,l)*100);
 Edit4->Text=FloatToStr(Edit2->Text.ToDouble()/Mo(sxy,l)*100);
 */
}
//---------------------------------------------------------------------------
// вычисление собственной концентрации электронов
long double ownConcentrationOfElectrons(long double T, long double x)
{
	long double k=1.380648813E-23/1.60217646E-19; // постоянная больцмана в электрон-Вольтах
	long double Eg=-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2.0*x)*T;
	return  (5.585-3.82*x+1.753E-3*T-1.364E-3*x*T)*1E20*pow(Eg,3/4.)*pow(T,1.5)*exp(-Eg/2./k/T); // собственная концентрация
}


void __fastcall TForm1::BuildingPlotsClick(TObject *Sender)
{
	Form2->Show();
}
//---------------------------------------------------------------------------

void MyReplaceStr(UnicodeString * out,UnicodeString * in, UnicodeString * findStr, UnicodeString * replaceStr)
{
    // не хотет робить, надо думать с передачей параметров.
	unsigned int findIndex=0;
	std::wstring s=in->w_str();
	std::wstring strToReplaceWhich=replaceStr->w_str(); // на что меняем
	std::wstring strToSearch=findStr->w_str();   // что ищем
	findIndex=s.find(strToSearch,strToSearch.length());
	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),strToReplaceWhich.begin(),strToReplaceWhich.end());
	*out=*s.c_str();
}

void TForm1::automaticCalculationHelper(UnicodeString SaveFileName)
{
	/*
	В SaveFileName нужно передавать _Us_Uy_vseZnachenia_k_"+IntToStr(i) и т.п.
	*/

 	Application->ProcessMessages();

	long double sko_xx=StrToFloat(Edit3->Text);
	if(isRoundNeeded)
	RoundM(&sko_xx,1);
	long double sko_xy=StrToFloat(Edit4->Text);
	if(isRoundNeeded)
	RoundM(&sko_xy,1);
	UnicodeString standartName; // эталонное имя файла
	standartName = sg1->FileName;     // запоминаем имя
	sg1->FileName=standartName+"T_"+eTemperature->Text+"_"+
		SaveFileName+"_sko_p_xx"+Sysutils::FloatToStrF(sko_xx,ffFixed,5,3)+
			 "_sko_p_xy"+Sysutils::FloatToStrF(sko_xy,ffFixed,5,3)+".txt";

	bSaveAllPoints->Click();

	unsigned int findIndex=0;
	std::wstring s=sg1->FileName.w_str();
	std::wstring strToReplaceWhich=L"11"; // на что меняем
	std::wstring strToSearch=L"vse";   // что ищем
	findIndex=s.find(strToSearch,strToSearch.length());
	s.replace(s.begin()+findIndex,s.begin()+findIndex+strToSearch.length(),strToReplaceWhich.begin(),strToReplaceWhich.end());

	sg1->FileName=s.c_str();
    //bSaveElevenPoints->Click();

	sg1->FileName=standartName;
}


void __fastcall TForm1::bAutomaticCalculationClick(TObject *Sender)
{
	/*
	+1. Просим задать имя.
	+2. Идем по температуре с шагом.
	+3. Рассчитываем параметры пленки и тензоры проводимости
	+4. Сохраняем параметры плёнки.
	+4. Сохраняем идеальный график тензора все точки и 11 точек.
	+5. Сохраняем идеальный график измеряемых сигналов Us и Uy все точки и 11 точек
	+6. Сохраняем рассчитанные параметры плёнки (концентрации и подвижности НЗ).
	+7. Задаем длину фильтра.

	// для каждого значения шума.
	Генерируем зашумленный сигнал.
	+8. Сохраняем все точки и 11 точек зашумленного измеряемого сигнала.
	+9. Запускаем фильтрацию.
	+10. Сохраняем все точки фильтрованного сигнала.

	+12. Сохраняем все точки и 11 точек зашумленных компонент тензора.
	+15. Сохраняем все точки и 11 точек фильтрованных компонент тензора.
	+16. А также экстраполированных сигналов и тензоров.

	*/
	silentModeEnabled=true;
	int h=eStepByTemperature->Text.ToInt(); // шаг по температуре
	int T1=Edit7->Text.ToInt(); // начальная температура
	int Tmax=Edit8->Text.ToInt(); // конечная температура

	int koef=1;// начальный коэффициент шума
	int endkoef=1;  // конечный коэффициент

	int h_koef=4; // шаг по уровню шума
	//eFilterLength->Text=IntToStr(150); // задаём длину фильтра, внимание - это для симметричного графика!
	g_hsize->Text=FloatToStr(0.001); // шаг по магнитному полю., отныне честно идем только до 2 Тл.

	UnicodeString standartName; // эталонное имя файла
	UnicodeString fName;   // новое имя файла
	UnicodeString path;
	UnicodeString file;

	file = ExtractFileName(standartName);

	UnicodeString signalsFolder="signals\\";
	UnicodeString tenzorsFolder="tenzors\\";

	// если в диалоге имя было выбрано
	if (sg1->Execute()) {
		mDebug->Lines->Add(sg1->FileName); // выводим его в мемо

		standartName = sg1->FileName;     // запоминаем имя

		UnicodeString path = ExtractFilePath(standartName);


		CreateDir(path+signalsFolder);
		CreateDir(path+tenzorsFolder);

		for (int T=T1; T <= Tmax; T+=h)  // идем по температуре с заданным шагом
		{
			 eTemperature->Text=IntToStr(T);  // для красоты обновляем значение температуры
			 bCalculateCarrierParams->Click(); // нажимаем кнопку рассчитать

			 // задаём имя файла
			 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
			 sg1->FileName=standartName+fName;
			 bSaveFilmParams->Click(); // сохраняем параметры плёнки

			 rbIdealTenzorPlot->Checked=true; // выбираем идеальный график тензоров
			 // задаём имя файла
			 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia.txt";
			 sg1->FileName=standartName+fName;
			 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика тензора
			 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy.txt";
			 sg1->FileName=standartName+fName;
			 // сохраняем 11 точек идеального графика тензора
			 bSaveElevenPoints->Click();

			 rbIdealUPlot->Checked=true; // выбираем идеальный график измеряемых сигналов
			 // задаём имя файла
			 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia.txt";
			 sg1->FileName=standartName+fName;
			 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика сигналов
			 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy.txt";
			 sg1->FileName=standartName+fName;
			 // сохраняем 11 точек идеального графика сигналов
			 bSaveElevenPoints->Click();
			 //sg1->FileName=standartName;

			 int numberOfNoises=editNumberNoises->Text.ToInt();

			 for (int noise=0; noise < numberOfNoises; noise++) {
			 sg1->FileName=standartName+IntToStr(noise);

			 for (int i=koef; i <= endkoef; i+=h_koef) // после чего начинается игра с коэффициентами
			 {
				Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1]))); // задаем коэффициенты
				Edit6->Text=IntToStr(i);
				bGaussianNoiseGenerator->Click(); // генератор шума

				sg1->FileName=path+signalsFolder+file+IntToStr(noise);
				rbNoisyU->Checked=true; // выбираем зашумленный график измеряемого сигнала

				fName="Us_Uy_vse_k_"+IntToStr(i);
				automaticCalculationHelper(fName);
				// надо сохранять зашумленные результаты
				rbNoisyTenzor->Checked=true;
				sg1->FileName=path+tenzorsFolder+file+IntToStr(noise);
				fName="tenzor_vse_k_"+IntToStr(i);
				automaticCalculationHelper(fName);

				/*
				//bFilteringPlots->Click(); // запускаем фильтрацию
				// сохраняем результаты фильтрации (все точки)-------------------------
				rbFilteredUPlot->Checked=true; // выбираем фильтрованный график
				sg1->FileName=standartName;
				fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
				sg1->FileName=standartName+fName;
				bSaveAllPoints->Click();
				// сохранять 11 точек на данный момент смысла нет.
				//---------------------------------------------------------------------
				rbFilteredTenzor->Checked=true;// сохранять результаты фильтрованных тензоров
				sg1->FileName=standartName;
				fName="tenzor_filt_vseZnachenia_k_"+IntToStr(i);
				automaticCalculationHelper(fName);
				//--------------------------------------------------------------------
				rbExtrapolatedU->Checked=true;
				sg1->FileName=standartName;
				fName="Us_Uy_Extrapolated_vseZnachenia_k_"+IntToStr(i);
				automaticCalculationHelper(fName);
				//--------------------------------------------------------------------
				rbExtrapolatedTenzor->Checked=true;
				sg1->FileName=standartName;
				fName="tenzor_Extrapolated_vseZnachenia_k_"+IntToStr(i);
				automaticCalculationHelper(fName);
				//--------------------------------------------------------------------
				*/
			 }
			 }
		}

	}
	else
	{
		return;
	}


	silentModeEnabled=false;
}
//---------------------------------------------------------------------------
int countOfRepeats=0;
long double criteria=0.01;
void __fastcall TForm1::bFilteringPlotsClick(TObject *Sender)
{
	if(FilteredParams!=0) // считаем по новой?
	delete FilteredParams;// надо убрать старое.

	FilteredParams=new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
   	FilteredParams->modifySignals(TrForMassiveFilter,ParamsWithNoise->getSignalUs(),ParamsWithNoise->getSignalUy(),eFilterLength->Text.ToInt());


	// и экстраполируем.
	if(ExtrapolatedParams!=0)
	delete ExtrapolatedParams;

	ExtrapolatedParams=new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	// Два режима экстраполяции
	// По фильтрованным данным
	if(rbFilteredOnly->Checked)
	{
		FilteredParams->modifySignals(EXTRAPOLATE,ExtrapolatedParams);
	}
	// И по фильтрованным вместе с зашумленными, а потом по фильтрованным вместе с экстраполированными
	if(rbFilteredNoisyExtrapolated->Checked)
	{
		extrapolateNoiseFiltered(ParamsWithNoise,FilteredParams,ExtrapolatedParams);
	}
	//------------Внимание - костыль.
	//------Так как иногда экстраполируется весьма плохо - будем это контролить и перезапускать если что:)
	long double distance=0;

	for (int i = 0; i < NumberOfPoints; i++) {
		distance+=ExtrapolatedParams->getSignalUs()[i]+ExtrapolatedParams->getSignalUy()[i];
        distance-=IdealParams->getSignalUs()[i]+IdealParams->getSignalUy()[i];
	}
	mDebug->Lines->Add(FloatToStr(distance/2.0/NumberOfPoints));
	if(fabs(distance/2.0/NumberOfPoints) > criteria)
	{
		countOfRepeats++;
    if(countOfRepeats>3) criteria*=2;
		bFilteringPlots->Click();


		return;
	}

	countOfRepeats=0;
	criteria=0.01;
	//------Конец костыля.
    // Построение графиков.
	FilteredParams->constructPlotFromTwoMassive(US,gSeriesFilteredUs,clBlue);
	FilteredParams->constructPlotFromTwoMassive(UY,gSeriesFilteredUy,clBlue);

	FilteredParams->constructPlotFromTwoMassive(US,gSeriesFilteredParamsUs,clBlue);
	FilteredParams->constructPlotFromTwoMassive(UY,gSeriesFilteredParamsUy,clBlue);

	FilteredParams->constructPlotFromTwoMassive(S_EFF,gSeriesFilteredParamsS_eff,clBlue);
	FilteredParams->constructPlotFromTwoMassive(RH_EFF,gSeriesFilteredParamsRh_eff,clBlue);

	FilteredParams->constructPlotFromTwoMassive(SXX,gSeriesFilteredParamsSxx,clBlue);
	FilteredParams->constructPlotFromTwoMassive(SXY,gSeriesFilteredParamsSxy,clBlue);

	ExtrapolatedParams->constructPlotFromTwoMassive(US,gSeriesExtrapolatedUs,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(UY,gSeriesExtrapolatedUy,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(US,gSeriesExtrapolatedParamsUs,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(UY,gSeriesExtrapolatedParamsUy,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(S_EFF,gSeriesExtrapolatedParamsS_eff,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(RH_EFF,gSeriesExtrapolatedParamsRh_eff,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(SXX,gSeriesExtrapolatedParamsSxx,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(SXY,gSeriesExtrapolatedParamsSxy,clGreen);
}



void __fastcall TForm1::bSaveElevenPointsClick(TObject *Sender)
{
	chooseAndSaveData(SOME_POINTS);
}
//---------------------------------------------------------------------------

void LoadingDataFromFile(TLineSeries * Series1,TLineSeries * Series2)
{
	// работает с графиками, от структур свободна.
	TStringList * tsl=new TStringList();
	if (silentModeEnabled || Form1->sg1->Execute()) {
	tsl->LoadFromFile(Form1->sg1->FileName);
	Series1->Clear();
		  Series2->Clear();
	tsl->Text=ReplaceChar(tsl->Text,L'.',L','); // заменить все точки на запятые

	for(int i=0;i<tsl->Count;i++) // по количеству строк
		  {

		  if(tsl->Strings[i].IsEmpty()) // пустые строки пропускаем
		  continue;
		  String s = tsl->Strings[i];

		  String s1=wcstok(s.c_str(),L" \t");
		  String s2=wcstok(NULL,L" \t");
		  String s3=wcstok(NULL,L" \t");

		  Series1->AddXY(s1.ToDouble(), // первая часть до пробела это х, вторая после у
		  s2.ToDouble(),"",clRed);
		  Series2->AddXY(s1.ToDouble(), // первая часть до пробела это х, вторая после у
		  s3.ToDouble(),"",clRed);

		  }
	}
	delete tsl;
}

void __fastcall TForm1::bLoadingPlotsClick(TObject *Sender)
{

// NEED'S UP TO DATE!!!!

/*if (rbLeftPlot->Checked) // левый
{
	LoadingDataFromFile(Series1,Series2);
}
if (rbRightPlot->Checked) // правый
{
	LoadingDataFromFile(Series3,Series4);
	/*for (int i=0; i < NumberOfPoints; i++)
	{
		ParamsWithNoise.Us[i]=Series3->YValues->Value[i];
		ParamsWithNoise.Uy[i]=Series4->YValues->Value[i];
	}
}      */
if (rbFilteredUPlot->Checked) // фильтрованный
{
	LoadingDataFromFile(gSeriesFilteredUs,gSeriesFilteredUy);
}

if (rbIdealUPlot->Checked) // идеальный
{
	LoadingDataFromFile(gSeriesIdealParamsUs,gSeriesIdealParamsUy);    //------------------------------!
	int length=gSeriesIdealParamsUs->YValues->Count;
	long double *x=new long double[length];
	long double *y=new long double[length];

	long double *y1=new long double[length];


	 for (int i=0; i < length; i++) {
		 x[i]=gSeriesIdealParamsUs->XValues->Value[i];
		 y[i]=gSeriesIdealParamsUs->YValues->Value[i];
	 }

	  for (int i=0; i < length; i++) {
		 y1[i]=gSeriesIdealParamsUy->YValues->Value[i];
	 }

	 IdealParams->setB_Us_Uy(x,y,y1);

	 IdealParams->constructPlotFromTwoMassive(SXX,Series1,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,Series2,clRed);

	IdealParams->constructPlotFromTwoMassive(SXX,gSeriesIdealParamsSxx,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,gSeriesIdealParamsSxy,clRed);

	IdealParams->constructPlotFromTwoMassive(S_EFF,gSeriesIdealParamsS_eff,clRed);
	IdealParams->constructPlotFromTwoMassive(RH_EFF,gSeriesIdealParamsRh_eff,clRed);

	IdealParams->constructPlotFromTwoMassive(US,gSeriesIdealParamsUs,clRed);
	IdealParams->constructPlotFromTwoMassive(UY,gSeriesIdealParamsUy,clRed);


 /*
 for (int i=0; i <NumberOfPoints; i++) { //----------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ParamsWithNoise.Us[i]=LineSeries3->YValues->Value[i];
	ParamsWithNoise.Uy[i]=LineSeries9->YValues->Value[i];
 }             */


 delete[] x;
 delete[] y;
}

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// сохранение параметров КРТ
// поддерживает тихий режим
void __fastcall TForm1::bSaveFilmParamsClick(TObject *Sender)
{
	TStringList * tsl=new TStringList();
	for (int i = 0; i < NumberOfCarrierTypes; i++)
	{
		tsl->Add(g_Nz_par->Cells[1][i+1] +"\t"+g_Nz_par->Cells[2][i+1]);
	}
	tsl->Text=ReplaceChar(tsl->Text,L',',L'.'); // заменить все запятые на точки
	if (silentModeEnabled || Form1->sg1->Execute())
	{
		tsl->SaveToFile(Form1->sg1->FileName);
	}
	delete tsl;
}

void __fastcall TForm1::bTestingSomethingClick(TObject *Sender)
{
	if(ExtrapolatedParams!=0)
	delete ExtrapolatedParams;

	ExtrapolatedParams=new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	extrapolateNoiseFiltered(ParamsWithNoise,FilteredParams,ExtrapolatedParams);

	ExtrapolatedParams->constructPlotFromTwoMassive(US,gSeriesExtrapolatedUs,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(UY,gSeriesExtrapolatedUy,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(US,gSeriesExtrapolatedParamsUs,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(UY,gSeriesExtrapolatedParamsUy,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(S_EFF,gSeriesExtrapolatedParamsS_eff,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(RH_EFF,gSeriesExtrapolatedParamsRh_eff,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(SXX,gSeriesExtrapolatedParamsSxx,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(SXY,gSeriesExtrapolatedParamsSxy,clGreen);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete IdealParams;
	delete ParamsWithNoise;
	delete FilteredParams;
	delete ExtrapolatedParams;

	IdealParams=0;
	ParamsWithNoise=0;
	FilteredParams=0;
	ExtrapolatedParams=0;

   //	FreeLibrary(hDLL);
}
//---------------------------------------------------------------------------

void TForm1::chooseAndSaveData(FileSaveMode mode)
{
    //пока что работает с графиками
	TLineSeries* Saving1=0;
	TLineSeries* Saving2=0;

	// идеальные данные
	if (rbIdealUPlot->Checked)
	{
		Saving1=gSeriesIdealParamsUs;
		Saving2=gSeriesIdealParamsUy;
	}

	if (rbIdealTenzorPlot->Checked)
	{
		Saving1=gSeriesIdealParamsSxx;
		Saving2=gSeriesIdealParamsSxy;
	}

	// зашумленные данные
	if (rbNoisyU->Checked)
	{
		Saving1=gSeriesParamsWithNoiseUs;
		Saving2=gSeriesParamsWithNoiseUy;
	}

	if(rbNoisyTenzor->Checked)
	{
		Saving1=gSeriesParamsWithNoiseSxx;
		Saving2=gSeriesParamsWithNoiseSxy;
	}

	// фильтрованные данные
	if (rbFilteredUPlot->Checked)
	{
		 Saving1=gSeriesFilteredParamsUs;
		 Saving2=gSeriesFilteredParamsUy;
	}

	if (rbFilteredTenzor->Checked)
	{
		Saving1=gSeriesFilteredParamsSxx;
		Saving2=gSeriesFilteredParamsSxy;
	}
	// экстраполированные данные
	if (rbExtrapolatedU->Checked)
	{
		Saving1=gSeriesExtrapolatedParamsUs;
		Saving2=gSeriesExtrapolatedParamsUy;
	}

	if (rbExtrapolatedTenzor->Checked)
	{
		Saving1=gSeriesExtrapolatedParamsSxx;
		Saving2=gSeriesExtrapolatedParamsSxy;
	}

	if(!Saving1)
		return;

	int length=Saving1->XValues->Count;
	int length2=Saving2->XValues->Count;

	if(!(length || length2))
	{
	 //ShowMessage("График пуст! Первый параметр: "+ IntToStr(length) +"второй параметр: "+ IntToStr(length2));
	 return;
	}
	if(length!=length2)
	{
	 //ShowMessage("Разное количество точек на графиках!");
	 return;
	}

	TStringList * tsl=new TStringList();

	long double *savingXData=new long double [length];
	long double *savingY1Data=new long double [length];
	long double *savingY2Data=new long double [length];
	for (int i = 0; i < length; i++) {
		savingY1Data[i]=Saving1->YValue[i];
		savingY2Data[i]=Saving2->YValue[i];
		savingXData[i]=Saving1->XValue[i];
	}
    if(isRoundNeeded==true)
	{
		RoundM(savingXData,length);
		RoundM(savingY1Data,length);
		RoundM(savingY2Data,length);
	}

	if (mode==SOME_POINTS) {
        const int SomePointsCount=11;
		long double points[SomePointsCount]={0};
		long double shag=0.2;
		for (int i=1; i < SomePointsCount; i++) {
			points[i]=points[i-1]+shag;
		}

		for (int i = 0; i < SomePointsCount; i++) {
			int index=0;
			long double r=4;
			for(int k=0;k<length;k++)
			{
				if(fabs(fabs(savingXData[k])-fabs(points[i]))<=r)
				{
					r=fabs(savingXData[k]-points[i]);
					index=k;
				}
			}

			tsl->Add(FloatToStr(savingXData[index])+"\t"+FloatToStr(savingY1Data[index])+"\t"+FloatToStr(savingY2Data[index]));
		}
	}
	if(mode==ALL_POINTS)
	{
		// формируем строку.
		for(int i=0;i<length;i++)
		{

			tsl->Add(FloatToStrF(savingXData[i],ffExponent,18,18)+"\t"+
			 FloatToStrF(savingY1Data[i],ffExponent,18,18) +"\t"+
			 FloatToStrF(savingY2Data[i],ffExponent,18,18));
		}
	}
    delete [] savingXData;
	delete [] savingY1Data;
	delete [] savingY2Data;

	//tsl->Text=ReplaceChar(tsl->Text,L',',L'.'); // заменить все запятые на точки
	/*
	for(int i=1;i<tsl->Text.Length();++i)
	{
		if(tsl->Text[i]==L',')
			tsl->Text[i]=L'.';
	}     */

	// если включен тихий режим - имя уже должно быть известно
	if (silentModeEnabled || Form1->sg1->Execute())
	{
		tsl->SaveToFile(Form1->sg1->FileName);
	}

	delete tsl;

}





void __fastcall TForm1::Button1Click(TObject *Sender)
{
	/*
	+1. Просим задать имя.
	+2. Идем по температуре с шагом.
	+3. Рассчитываем параметры пленки и тензоры проводимости
	+4. Сохраняем параметры плёнки.
	+4. Сохраняем идеальный график тензора все точки и 11 точек.
	+5. Сохраняем идеальный график измеряемых сигналов Us и Uy все точки и 11 точек
	+6. Сохраняем рассчитанные параметры плёнки (концентрации и подвижности НЗ).
	+7. Задаем длину фильтра.

	// для каждого значения шума.
	Генерируем зашумленный сигнал.
	+8. Сохраняем все точки и 11 точек зашумленного измеряемого сигнала.
	+9. Запускаем фильтрацию.
	+10. Сохраняем все точки фильтрованного сигнала.

	+12. Сохраняем все точки и 11 точек зашумленных компонент тензора.
	+15. Сохраняем все точки и 11 точек фильтрованных компонент тензора.
	+16. А также экстраполированных сигналов и тензоров.

	*/
	silentModeEnabled=true;
	int h=eStepByTemperature->Text.ToInt(); // шаг по температуре
	int T1=Edit7->Text.ToInt(); // начальная температура
	int Tmax=Edit8->Text.ToInt(); // конечная температура

	std::vector<long double> mzr;
	mzr.push_back(1.312418E-03);
	mzr.push_back(3.749766E-04);
	mzr.push_back(1.406162E-04);
	mzr.push_back(8.199488E-05);

	int koef=0;// начальный коэффициент шума
	int endkoef=mzr.size()-1;  // конечный коэффициент

	int h_koef=1; // шаг по уровню шума
	//eFilterLength->Text=IntToStr(150); // задаём длину фильтра, внимание - это для симметричного графика!
	g_hsize->Text=FloatToStr(1.0E-05); // шаг по магнитному полю., отныне честно идем только до 2 Тл.

	UnicodeString standartName; // эталонное имя файла
	UnicodeString fName;   // новое имя файла

	// если в диалоге имя было выбрано
	if (sg1->Execute()) {
		mDebug->Lines->Add( sg1->FileName); // выводим его в мемо
	}
	else
	{
		return;
	}

	standartName = sg1->FileName;     // запоминаем имя

	for (int T=T1; T <= Tmax; T+=h)  // идем по температуре с заданным шагом
	{
		 eTemperature->Text=IntToStr(T);  // для красоты обновляем значение температуры
		 bCalculateCarrierParams->Click(); // нажимаем кнопку рассчитать

		 // задаём имя файла
		 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveFilmParams->Click(); // сохраняем параметры плёнки

         rbIdealTenzorPlot->Checked=true; // выбираем идеальный график тензоров
		 // задаём имя файла
		 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia.txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика тензора
		 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy.txt";
		 sg1->FileName=standartName+fName;
		 // сохраняем 11 точек идеального графика тензора
		 bSaveElevenPoints->Click();


		 rbIdealUPlot->Checked=true; // выбираем идеальный график измеряемых сигналов
		 // задаём имя файла
		 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia.txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика сигналов
		 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy.txt";
		 sg1->FileName=standartName+fName;
		 // сохраняем 11 точек идеального графика сигналов
		 bSaveElevenPoints->Click();
		 sg1->FileName=standartName;
		 for (int i=koef; i <= endkoef; i+=h_koef) // после чего начинается игра с коэффициентами
		 {
			LabeledEdit1->Text=FloatToStr(mzr[i]);
			//Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1]))); // задаем коэффициенты
			//Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1])));
			bGaussianNoiseGenerator->Click(); // генератор шума
			//bFilteringPlots->Click(); // запускаем фильтрацию

			sg1->FileName=standartName;
			rbNoisyU->Checked=true; // выбираем зашумленный график измеряемого сигнала
			fName="Us_Uy_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			// надо сохранять зашумленные результаты
			rbNoisyTenzor->Checked=true;
			sg1->FileName=standartName;
			fName="tenzor_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			// сохраняем результаты фильтрации (все точки)-------------------------
			rbFilteredUPlot->Checked=true; // выбираем фильтрованный график
			sg1->FileName=standartName;
			fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
			sg1->FileName=standartName+fName;
			bSaveAllPoints->Click();
			// сохранять 11 точек на данный момент смысла нет.
			//---------------------------------------------------------------------
			rbFilteredTenzor->Checked=true;// сохранять результаты фильтрованных тензоров
			sg1->FileName=standartName;
			fName="tenzor_filt_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			//--------------------------------------------------------------------
			rbExtrapolatedU->Checked=true;
			sg1->FileName=standartName;
			fName="Us_Uy_Extrapolated_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			//--------------------------------------------------------------------
			rbExtrapolatedTenzor->Checked=true;
			sg1->FileName=standartName;
            fName="tenzor_Extrapolated_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			//--------------------------------------------------------------------
		 }
	}
	silentModeEnabled=false;
}
//---------------------------------------------------------------------------


