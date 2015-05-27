//---------------------------------------------------------------------------

#ifndef SignalShiftFormH
#define SignalShiftFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
    TLabeledEdit *uiShiftValue;
    TComboBox *uiLeftBound;
    TComboBox *uiRightBound;
    TRadioGroup *uiDataKind;
    TButton *bResShiftCurve;
    TLabel *Label7;
private:	// User declarations
public:		// User declarations
    __fastcall TForm4(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
