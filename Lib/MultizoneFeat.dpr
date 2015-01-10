library MultizoneFeat;

{ Important note about DLL memory management: ShareMem must be the
  first unit in your library's USES clause AND your project's (select
  Project-View Source) USES clause if your DLL exports any procedures or
  functions that pass strings as parameters or function results. This
  applies to all strings passed to and from your DLL--even those that
  are nested in records and classes. ShareMem is the interface unit to
  the BORLNDMM.DLL shared memory manager, which must be deployed along
  with your DLL. To avoid using BORLNDMM.DLL, pass string information
  using PChar or ShortString parameters. }

uses
  Windows,
  SysUtils,
  Forms,
  Dialogs,
  Grids,
  StdCtrls,
  ExtCtrls,
  Math,
  Series,
  Chart,
  Optim1,
  Base;

// const



  type
  SeriesArray = array[1..9] of TBarSeries;
    EditArray = array[1..36] of TEdit;

var

  fil, fil_hall8, Data_File, Config_File :text;
  B_spektr,Gxx_sp,Gxx_MC,Gxy_MC,
  Gxy_sp,Xr,Lv,Xv,Mv,Vpr                 :Dat1;
  NPoint_hm                       :Integer;


  GraphON_hall,
  GraphOn_Farad                          :Boolean;
  Pnl_Pr_Hall8: TPanel;

 { Series7 :TLineSeries;
  Series8 :TLineSeries;
  Series9 :TLineSeries;
  Series10 :TLineSeries;}
  d1                                     :DataValue;
  mmoFeatFunctionValue:TMemo;
   mmoFeatMuElectronValue:TMemo;
   MemoMnog3:TMemo;
   MemoMnog4:TMemo;
   mmoConcentrationElectrons:TMemo;
   MemoMnog6:TMemo;
   MemoMnog7:TMemo;

   EditMassive : array [1..6] of TEdit;



////////////////////////////////////////////////////////////////////////////
///////////////// НАЧАЛО "ХОЛЛ. МНОГОЗОННАЯ ПОДГОНКА" //////////////////////
////////////////////////////////////////////////////////////////////////////

       {
procedure FileOpen_hall8(Memo6:TMemo; OpenDialog3:TOpenDialog);
var i:Integer;
begin
  {
  Читалось всё в переменные
   read(Data_File,Max_Value[1],Max_Value[2],Max_Value[3],Max_Value[4],
         Max_Value[5],Max_Value[6]);
         read(Data_File,Min_Value[1],Min_Value[2],Min_Value[3],Min_Value[4],
         Min_Value[5],Min_Value[6]);
         for i:=0 to NPoint-1 do
         readln(Data_File,Magfield_spektr[i],GxxExp[i],GxyExp[i]);
         Memo6.Lines.LoadFromFile(OpenDialog3.FileName);
  }
{end;   }



  {
procedure Graphik_hall8;
var i:Integer;
begin

  if GraphON_hall then
  begin
   Series7.Clear;
   Series8.Clear;
   Series9.Clear;
   Series10.Clear;
   mmoFeatFunctionValue.Lines.Add(FloatToStr(func_hall8));
   mmoFeatMuElectronValue.Lines.Add(FloatToStr(DATA[1]));
   MemoMnog3.Lines.Add(FloatToStr(DATA[2]));
   MemoMnog4.Lines.Add(FloatToStr(DATA[3]));
   mmoConcentrationElectrons.Lines.Add(FloatToStr(DATA[4]));
   MemoMnog6.Lines.Add(FloatToStr(DATA[5]));
   MemoMnog7.Lines.Add(FloatToStr(DATA[6]));
   for i:=0 to NPoint-1  do
   begin
     Series7.AddXY(MagField_spektr[i], Gxx[i]);
     Series8.AddXY(MagField_spektr[i], GxxExp[i]);
     Series9.AddXY(MagField_spektr[i], Gxy[i]);
     Series10.AddXY(MagField_spektr[i], GxyExp[i]);
   end;
   
   Application.ProcessMessages(); // обрабатывает все сообщения поступившие
                                  // приложению
  end;
end;
       }
procedure Optimiz_hall8(VesGxx,VesGxy:Extended);
begin

  //Ves1:=VesGxx;
  //Ves2:=VesGxy; 
  Randomize;
  N_Data:=6;
 // Registerfunc(func_hall8);
 // Registergraph(Graphik_hall8);
  BegRand(1000);   // в данном случае c 1000 гараздо лучше ищет
  hook;
  // Graphik_hall8;
end;

{
procedure ReplacementOfSeparator;   // Убирает запятые из файла
var j,position,size_m:Integer;
    MiniString, MaxiString:string;

begin
  MiniString:='';
  MaxiString:='';
{$I-}
  //  Reset(Data_File);
{$I+}
{  if IOResult<>0 then
   begin
    ShowMessage('Ошибка открытия файла');
    Exit;
   end
  else
    begin
     while not Eof(Data_File) do // Убираем запятые, если они есть,
                            // и заменяем их на '.'
      begin
        Readln(Data_File,  ministring);
        size_m:=SizeOf(MiniString);
        for j:=0 to size_m+1 do
        begin
          position:=Pos(',',MiniString);
          if position<>0 then
          MiniString[position]:='.';
        end;

        MaxiString:=MaxiString+MiniString+#13#10;
      end;
      CloseFile(Data_File);
    end;

  Rewrite(Data_File);    // Переписываем файл без запятых
  Write(Data_File, MaxiString);
  CloseFile(Data_File);
end;    }
   {
procedure Button10Click(Memo6:TMemo;OpenDialog3:TOpenDialog);
begin
  if OpenDialog3.Execute then
  begin
    AssignFile(Data_File,OpenDialog3.FileName);
    ReplacementOfSeparator;
    FileOpen_hall8(Memo6, OpenDialog3);
  end;
end;
   }
procedure btnFeatOnceClick(VesGxx,VesGxy:Extended);
begin    // однократная подгонка.
  GraphON_hall:=True;
  Optimiz_hall8(VesGxx,VesGxy);
end;

procedure btnFeatMultiClick(VesGxx,VesGxy:Extended);
var  i,l:Integer; fmin:Extended;
begin
   
  // тут у нас работа с графическими компонентами - можно и убрать
  {
  for l:=1 to 7 do
  SerArr[l]:=Form4.FindComponent('Series'+IntToStr(l)) as TBarSeries;
  for l:=1 to 28 do
  EditArr[l]:=Form4.FindComponent('Edit'+IntToStr(l)) as TEdit;
}

  fmin:=10e8;
  GraphON_hall:=False;
   {
  for l:=1 to 7 do
  begin
    TBarSeries(FindComponent('Series'+IntToStr(l))).Clear;
    TPointSeries(FindComponent('Series'+IntToStr(l))).Clear;
  end;
    }
   
  for l:=1 to 100 do
  begin

    Optimiz_hall8(VesGxx,VesGxy);
    for i:=1 to 6 do
    d1[i,l]:=Data[i];
    d1[7,l]:=func_hall8(Data,MagField_spektr,Gxx,Gxy,GxxExp,GxyExp,NPoint,Ves1,Ves2);
    // Разброс значений ***********************//
    // По точкам.
    {
    Series7.AddXY(d1[7,l], l);
    Series1.AddXY(DATA[1], l);
    Series2.AddXY(DATA[2], l);
    Series3.AddXY(DATA[3], l);
    Series4.AddXY(DATA[4], l);
    Series5.AddXY(DATA[5], l);
    Series6.AddXY(DATA[6], l);    }
    //***************************************//

    if  d1[7,l]<fmin then
    begin
      fmin:=d1[7,l];
      VesGxx:= Round(fmin);
      for i:=1 to 6 do
      begin
        EditMassive[i].Text:=FloatToStr(Data[i]);
      //------------------------------------------------------------------------
      end;
    end;
    
    
  end;
 //Gistogram(d1,SerArr,7,100);
 //Statistic(d1,EditArr,7,100);
 //GraphON_hall:=True;
 //ShowGraphics(EditArr, 6, Graphik_hall8);
 
end;

function Interval(StringGrid3:TStringGrid):Boolean;   // рассчитывает подвижность и концентрацию
var i:Integer;             // 3-х типов носителей в интервале (1%; 150%)
    Stg3_pusto:Boolean;
    max,min:Extended;
begin
  min:=-0.5;
  max:=0.5;
  Stg3_pusto:=False;
  for i:=1 to 3 do
      if (StringGrid3.Cells[1,i]='') or (StringGrid3.Cells[2,i]='') then
         Stg3_pusto:=True;
      if Stg3_pusto then
      begin
        Interval:=True;
        Exit;
      end
      else
      begin
        Max_Value[1]:=RoundTo(StrToFloat(StringGrid3.Cells[2,3])+
        StrToFloat(StringGrid3.Cells[2,3])*min,-6);
        Max_Value[2]:=RoundTo(StrToFloat(StringGrid3.Cells[2,2])+
        StrToFloat(StringGrid3.Cells[2,2])*max,-3);
        Max_Value[3]:=RoundTo(StrToFloat(StringGrid3.Cells[2,1])+
        StrToFloat(StringGrid3.Cells[2,1])*max,-3);
        Max_Value[4]:=RoundTo(StrToFloat(StringGrid3.Cells[1,3])+
        StrToFloat(StringGrid3.Cells[1,3])*min,14);
        Max_Value[5]:=RoundTo(StrToFloat(StringGrid3.Cells[1,2])+
        StrToFloat(StringGrid3.Cells[1,2])*max,15);
        Max_Value[6]:=RoundTo(StrToFloat(StringGrid3.Cells[1,1])+
        StrToFloat(StringGrid3.Cells[1,1])*max,19);
        Min_Value[1]:=RoundTo(StrToFloat(StringGrid3.Cells[2,3])+
        StrToFloat(StringGrid3.Cells[2,3])*max,-3);
        Min_Value[2]:=RoundTo(StrToFloat(StringGrid3.Cells[2,2])+
        StrToFloat(StringGrid3.Cells[2,2])*min,-6);
        Min_Value[3]:=RoundTo(StrToFloat(StringGrid3.Cells[2,1])+
        StrToFloat(StringGrid3.Cells[2,1])*min,-6);
        Min_Value[4]:=RoundTo(StrToFloat(StringGrid3.Cells[1,3])+
        StrToFloat(StringGrid3.Cells[1,3])*max,15);
        Min_Value[5]:=RoundTo(StrToFloat(StringGrid3.Cells[1,2])+
        StrToFloat(StringGrid3.Cells[1,2])*min,14);
        Min_Value[6]:=RoundTo(StrToFloat(StringGrid3.Cells[1,1])+
        StrToFloat(StringGrid3.Cells[1,1])*min,18);
        Interval:=False;
      end;
      
end;
 {
procedure Button12Click(Sender: TObject);
var i:Integer;
begin
  if not Interval then // если таблица не пуста и интервалы рассчитаны, то
   begin
    Memo6.Lines.Clear;
    Memo6.Lines.Add(IntToStr(NPoint));
    Memo6.Lines.Add(
          FloatToStr(Max_Value[1])
    +'	'+FloatToStr(Max_Value[2])
    +'	'+FloatToStr(Max_Value[3])
    +'	'+FloatToStr(Max_Value[4])
    +'	'+FloatToStr(Max_Value[5])
    +'	'+FloatToStr(Max_Value[6])
    );
    Memo6.Lines.Add(
          FloatToStr(Min_Value[1])
    +'	'+FloatToStr(Min_Value[2])
    +'	'+FloatToStr(Min_Value[3])
    +'	'+FloatToStr(Min_Value[4])
    +'	'+FloatToStr(Min_Value[5])
    +'	'+FloatToStr(Min_Value[6])
    );
    for i:=0 to NPoint-1 do
    Memo6.Lines.Add(
          FloatToStr(Magfield_spektr[i])
    +'	'+FloatToStr(GxxExp[i])
    +'	'+FloatToStr(GxyExp[i])
    );
    for i:=1 to StringGrid3.RowCount-1 do
    Memo6.Lines.Add(StringGrid3.cells[1,i]+'	'+StringGrid3.cells[2,i]);
    OpenDialog3.FileName:=GetCurrentDir+'\Из спектра подвижности.hs';
    Label42.Caption:=OpenDialog3.FileName;
    Memo6.Lines.SaveToFile(OpenDialog3.FileName);
    btnFeatOnce.Enabled:=True;
    btnFeatMulti.Enabled:=True;
    N29.Enabled:=True;
    Button27.Enabled:=True;
    Button27.Click;
   end
  else
  begin
    btnFeatOnce.Enabled:=False;
    btnFeatMulti.Enabled:=False;
    N29.Enabled:=False;
    Button27.Enabled:=False;
  end;
  
end;
     }

procedure WriteHall8Res;
var i:Integer;
begin

  begin
     Writeln(Data_File,mmoFeatFunctionValue.Lines[mmoFeatFunctionValue.Lines.Count-1]);
     Writeln(Data_File,mmoFeatMuElectronValue.Lines[mmoFeatMuElectronValue.Lines.Count-1]
     +' '+MemoMnog3.Lines[MemoMnog3.Lines.Count-1]
     +' '+MemoMnog4.Lines[MemoMnog4.Lines.Count-1]
     +' '+mmoConcentrationElectrons.Lines[mmoConcentrationElectrons.Lines.Count-1]
     +' '+MemoMnog6.Lines[MemoMnog6.Lines.Count-1]
     +' '+MemoMnog7.Lines[MemoMnog7.Lines.Count-1]);
     for i:=0 to NPoint-1 do
     Writeln(Data_File,FloatToStr(MagField_Spektr[i])+' '+
     FloatToStr(GxxExp[i])+' '+FloatToStr(GxyExp[i])+' '+
     FloatToStr(Gxx[i])+' '+FloatToStr(Gxy[i]));

     //----------------------------------------------
      {
     // Сохраняем средние значения параметров:
     Writeln(Data_File,'Средние значения параметров');
     Writeln(Data_File, Edit8.Text
     +' '+Edit9.Text
     +' '+Edit10.Text
     +' '+Edit11.Text
     +' '+Edit12.Text
     +' '+Edit13.Text);

     // Сохраняем СКО:
     Writeln(Data_File,'СКО');
     Writeln(Data_File, Edit15.Text
     +' '+Edit16.Text
     +' '+Edit17.Text
     +' '+Edit18.Text
     +' '+Edit19.Text
     +' '+Edit20.Text);

     // Сохраняем СКО %:
     Writeln(Data_File,'СКО, %');
     Writeln(Data_File, Edit22.Text
     +' '+Edit23.Text
     +' '+Edit24.Text
     +' '+Edit25.Text
     +' '+Edit26.Text
     +' '+Edit27.Text);

     Writeln(Data_File,'Средние значения целевой функции');
     Writeln(Data_File, Edit7.Text
     +' '+Edit14.Text
     +' '+Edit21.Text
     +' '+Edit28.Text);

     Writeln(Data_File,'Гистограммы.');
     Writeln(Data_File,'Подвижность электронов.');
     with(Form4) do
     begin  
       for i:=0 to Series1.XValues.Count-1 do
       begin
              Writeln(Data_File,FloatToStr(Series1.XValues[i])
              +' '+ FloatToStr(Series1.YValues[i]));
       end;
     end;

     Writeln(Data_File,'Подвижность легких дырок.');
     with(Form4) do
     begin  
       for i:=0 to Series2.XValues.Count-1 do
       begin
              Writeln(Data_File,FloatToStr(Series2.XValues[i])
              +' '+ FloatToStr(Series2.YValues[i]));
       end;
     end;

     Writeln(Data_File,'Подвижность тяжелых дырок.');
     with(Form4) do
     begin  
       for i:=0 to Series3.XValues.Count-1 do
       begin
              Writeln(Data_File,FloatToStr(Series3.XValues[i])
              +' '+ FloatToStr(Series3.YValues[i]));
       end;
     end;

     Writeln(Data_File,'Концентрация электронов.');
     with(Form4) do
     begin  
       for i:=0 to Series4.XValues.Count-1 do
       begin
              Writeln(Data_File,FloatToStr(Series4.XValues[i])
              +' '+ FloatToStr(Series4.YValues[i]));
       end;
     end;

     Writeln(Data_File,'Концентрация легких дырок.');
     with(Form4) do
     begin  
       for i:=0 to Series5.XValues.Count-1 do
       begin
              Writeln(Data_File,FloatToStr(Series5.XValues[i])
              +' '+ FloatToStr(Series5.YValues[i]));
       end;
     end;

     Writeln(Data_File,'Концентрация тяжелых дырок.');
     with(Form4) do
     begin  
       for i:=0 to Series6.XValues.Count-1 do
       begin
              Writeln(Data_File,FloatToStr(Series6.XValues[i])
              +' '+ FloatToStr(Series6.YValues[i]));
       end;
     end;
        }
  end; 
end;
   {
procedure btnSaveFeatResultsClick(Sender: TObject);
begin
  if SDHall8Res.Execute then
  begin
    AssignFile(Data_File, SDHall8Res.FileName);
    WriteHall8Res;
  end;
end;
    }

    {
procedure LoadHall8Res;
var i:Integer;
    f:Extended;
begin


    Reset(Data_File);


  if IOResult<>0 then
   begin
    ShowMessage('Ошибка открытия файла');
    Exit;
   end
  else
 begin
  try
    Readln(Data_File,f);
    Readln(Data_File,Data[1],Data[2],Data[3],Data[4],Data[5],Data[6]);
    for i:=0 to NPoint-1 do
    Readln(Data_File,Magfield_Spektr[i],GxxExp[i],GxyExp[i],Gxx[i],Gxy[i]);
    Series7.Clear;
    Series8.Clear;
    Series9.Clear;
    Series10.Clear;
    mmoFeatFunctionValue.Lines.Add(FloatToStr(f));
    mmoFeatMuElectronValue.Lines.Add(FloatToStr(DATA[1]));
    MemoMnog3.Lines.Add(FloatToStr(DATA[2]));
    MemoMnog4.Lines.Add(FloatToStr(DATA[3]));
    mmoConcentrationElectrons.Lines.Add(FloatToStr(DATA[4]));
    MemoMnog6.Lines.Add(FloatToStr(DATA[5]));
    MemoMnog7.Lines.Add(FloatToStr(DATA[6]));
   for i:=0 to NPoint-1  do
   begin
     Series7.AddXY(MagField_spektr[i], Gxx[i]);
     Series8.AddXY(MagField_spektr[i], GxxExp[i]);
     Series9.AddXY(MagField_spektr[i], Gxy[i]);
     Series10.AddXY(MagField_spektr[i], GxyExp[i]);
   end;
  except
    ShowMessage('Ошибка загрузки данных');
    Rewrite(Data_File);
  end;
  CloseFile(Data_File);
 end; 
end; }
   {
procedure btnLoadFeatResultsClick(Sender: TObject);
begin
  if ODHall8Res.Execute then
  begin
    AssignFile(Data_File, ODHall8Res.FileName);
    ReplacementOfSeparator;
    LoadHall8Res;
  end;
end;


       }

// расчет среднего и СКО
// вот сюда указатель на массив надо давать.
procedure Statistic(mass:DataValue; ResulitsForStatistic:array of Extended; m,n:Integer);
var  ll,l:integer;
     Xsr, S:Extended;
begin
  for ll:=1 to m do
  begin
     Xsr:=0;
     S:=0;
    for l:=1 to n do
    Xsr:=Xsr+mass[ll,l];
    Xsr:=Xsr/n;
    for l:=1 to n do
    S:=S+Sqr(mass[ll,l]-Xsr);
    S:=Sqrt(s/(n-1));

    ResulitsForStatistic[ll+m]:= Xsr;
    ResulitsForStatistic[ll+2*m]:= S;
    ResulitsForStatistic[ll+3*m]:= (S/abs(Xsr))*100;
    
  end;
end;

////////////////////////////////////////////////////////////////////////////
///////////////// КОНЕЦ "ХОЛЛ. МНОГОЗОННАЯ ПОДГОНКА" //////////////////////
////////////////////////////////////////////////////////////////////////////

{
Итак, картина следующая
На вход подаем зависимости:

Минимальные и максимальные значения. Т.е. коридор.
Дальше данные тензоров.

read(Data_File,Max_Value[1],Max_Value[2],Max_Value[3],Max_Value[4],
         Max_Value[5],Max_Value[6]);
         read(Data_File,Min_Value[1],Min_Value[2],Min_Value[3],Min_Value[4],
         Min_Value[5],Min_Value[6]);
         for i:=0 to NPoint-1 do
         readln(Data_File,Magfield_spektr[i],GxxExp[i],GxyExp[i]);
         Memo6.Lines.LoadFromFile(OpenDialog3.FileName);

На выходе читаем данные из:

Минимальные значения функции и значения параметров соотв. минимальному значению функции.

Графики тензоров (экспериментальные и теоретические).

mmoFeatFunctionValue.Lines.Add(FloatToStr(func_hall8));
   mmoFeatMuElectronValue.Lines.Add(FloatToStr(DATA[1]));
   MemoMnog3.Lines.Add(FloatToStr(DATA[2]));
   MemoMnog4.Lines.Add(FloatToStr(DATA[3]));
   mmoConcentrationElectrons.Lines.Add(FloatToStr(DATA[4]));
   MemoMnog6.Lines.Add(FloatToStr(DATA[5]));
   MemoMnog7.Lines.Add(FloatToStr(DATA[6]));
   for i:=0 to NPoint-1  do
   begin
     Series7.AddXY(MagField_spektr[i], Gxx[i]);
     Series8.AddXY(MagField_spektr[i], GxxExp[i]);
     Series9.AddXY(MagField_spektr[i], Gxy[i]);
     Series10.AddXY(MagField_spektr[i], GxyExp[i]);

     Надо найти способ выудить отсюда ещё средние значения.
     СКО и СКО в %

}

// Возвращаем по традиции количество точек?
// Веса, нижняя и верхняя границы, Спектр, две компоненты тензора
function RunMultizoneFeat (VesGxx,VesGxy:Extended;LowBound,UpBound:Bounds;
  MagSpectr:Data_spektr;GxxIn,GxyIn:Data_spektr;
  outResulitsForStatistic:array of Extended; outSizeStatistic:Integer;
  outGxx,outGxy:Data_spektr; outSizeDataSpectr:Integer;
  outMinValue,outMiddleValue,outSKOValue,outSKOinPercentValue:DataValue):Integer ; stdcall;
var i:Integer;
ResulitsForStatistic: array [0..35] of Extended;

  numberOfCarrierTypes: Extended;
  
  Fres:Extended;
begin

  //ShowMessage('Добро пожаловать!');
  // Это надо получить из управляющей программы:
  NPoint:=11; // Это тоже потом будем передавать.
  Ves1:=VesGxx;
  Ves2:=VesGxy;
  // Эти переменные объявлены в optim1 и их тоже надо получить.

  //ShowMessage('Готовимся копировать граничные условия!');
  for i:=1 to 6 do
  begin
    Max_Value[i]:=UpBound[i];
    Min_Value[i]:=LowBound[i];
  end;  

  //ShowMessage('Копируем компоненты тензора.');
  // ну и разумеется получить сами точки.
  for i:=0 to NPoint-1 do
  begin
    Magfield_spektr[i]:=MagSpectr[i];
    GxxExp[i]:=GxxIn[i];
    GxyExp[i]:=GxyIn[i];
  end;


  // После того как все данные получены начинаем выполнение.
  btnFeatMultiClick(Ves1,Ves2);


  // программа отработала
  // Так, где лежат результаты?

  Fres:=func_hall8(Data,MagField_spektr,Gxx,Gxy,GxxExp,GxyExp,NPoint,Ves1,Ves2); // минимальное значение функции
   {

   // Кстати всё это хранится в d1
  DATA[1]; // подвижность электронов
  DATA[2]; // подвижность легких дырок
  DATA[3]; // подвижность тяжелых дырок
  DATA[4]; // концентрация электронов
  DATA[5]; // концентрация легких дырок
  DATA[6]; // концентрация тяжелых дырок

  for i:=0 to NPoint-1  do
  begin
    MagField_spektr[i], Gxx[i]);
    MagField_spektr[i], GxxExp[i]);
    MagField_spektr[i], Gxy[i]);
    MagField_spektr[i], GxyExp[i]);
  end;
       }
  // эта функция должна посчитать нам статистику.
  outSizeStatistic:=36;
  outSizeDataSpectr:=MaxPoints;
  for i:=0 to NPoint-1 do
  begin
    outGxx[i]:=Gxx[i];
    outGxy[i]:=Gxy[i];
  end;
  Statistic(d1, ResulitsForStatistic, 7,100);
  for i:=0 to 35 do
  begin
    outResulitsForStatistic[i]:=ResulitsForStatistic[i];
  end;
  
  Result:=0;

end;  

exports

RunMultizoneFeat;

// Это для освобождения памяти.
procedure ProcDLL(Reason: Integer);
begin
  if Reason = DLL_PROCESS_DETACH then
  begin
    
  end;
end;
begin
   DllProc:=@ProcDLL;


  
end.
