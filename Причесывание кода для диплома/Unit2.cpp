//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit1.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
const int size=300-77+1;
long double ph=StrToFloat(Form1->g_Nz_par->Cells[1][3]);
long double T=77;
long double x=0.1;
long double n[size]={0};
long double Tn[size]={0};
for(int i=0;i<size;i++,T+=1)
{
n[i]=-pow(ownConcentrationOfElectrons(T,x),2)/ph;
Tn[i]=T;
Series1->AddXY(Tn[i],n[i],"",clBlack);
}




;
}
//---------------------------------------------------------------------------
