//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Project1.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("driver.cpp");
USEFORM("Unit2.cpp", SaveForm);
USEUNIT("\\VBOXSVR\MCTParamsExplorer\ExtrapolateUnit.cpp");
USEUNIT("\\VBOXSVR\MCTParamsExplorer\FilteringUnit.cpp");
USEUNIT("\\VBOXSVR\MCTParamsExplorer\LCard.cpp");
USEUNIT("\\VBOXSVR\MCTParamsExplorer\MagneticFieldDependence.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------


