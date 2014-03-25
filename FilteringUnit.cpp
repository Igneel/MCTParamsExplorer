//---------------------------------------------------------------------------
#include "FilteringUnit.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ВНИМАНИЕ!!! Напрямую не вызывать!!! Пользоваться трамплином!!!---------------
double Filter (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = length; //Длина фильтра 20
long double Fd = Fdisk; //Частота дискретизации входных данных 2000
long double Fs = Fpropysk; //Частота конца полосы пропускания  10
long double Fx = Fzatyh; //Частота начала полосы затухания    20

long double *H= new long double [N] ; //Импульсная характеристика фильтра
long double *H_id= new long double [N]; //Идеальная импульсная характеристика
long double *W= new long double [N]; //Весовая функция

//Расчет импульсной характеристики фильтра
long double Fc = (Fs + Fx) / (2 * Fd);

for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2*M_PI*Fc;
else H_id[i] = sinl(2*M_PI*Fc*i )/(M_PI*i);
// весовая функция Блекмена
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1));
H [i] = H_id[i] * W[i];
}

//Нормировка импульсной характеристики
long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1

//----------------------------------------------------------------
// печать коэффициентов фильтра
//for(int i=0;i<N;i++)
//Form1->Memo2->Lines->Add(FloatToStr(H[i]));
//----------------------------------------------------------------

//Фильтрация входных данных
for (int i=0; i<sizeIn; i++)
{
out[i]=0.;
for (int j=0; j<(i>N-1?N-1:i); j++)// та самая формула фильтра
out[i]+= H[j]*in[i-j];
}
delete [] H;
delete [] H_id;
delete [] W;
return (N-1)/2.0;
}

// ВНИМАНИЕ!!! Напрямую не вызывать!!! Пользоваться трамплином!!!---------------
double Filter (const std::vector<long double> &in, std::vector<long double> & out,
int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = length; //Длина фильтра
long double Fd = Fdisk; //Частота дискретизации входных данных 2000
long double Fs = Fpropysk; //Частота конца полосы пропускания  10
long double Fx = Fzatyh; //Частота начала полосы затухания    20

std::vector<long double> H(N);  //Импульсная характеристика фильтра
std::vector<long double> H_id(N); //Идеальная импульсная характеристика
std::vector<long double> W(N);   //Весовая функция

//Расчет импульсной характеристики фильтра
long double Fc = (Fs + Fx) / (2.0 * Fd);

for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2.0*M_PI*Fc;
else H_id[i] = sinl(2.0*M_PI*Fc*i )/(M_PI*i);
// весовая функция Блекмена
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1.0));
H [i] = H_id[i] * W[i];
}

//Нормировка импульсной характеристики
long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1

//Фильтрация входных данных
for (int i=0; i<sizeIn; i++)
{
out[i]=0.0;
for (int j=0; j<(i>N-1?N-1:i); j++)// та самая формула фильтра
out[i]+= H[j]*in[i-j];
}

return (N-1)/2.0;
}


// функция трамплин для фильтра.  ----------------------------------------------
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh)
{
if(!inS->YValues->Count())   // если пустой - ошибка, убегаем.
{
//ShowMessage("Пустой массив!");
return 0;
}
int size=inS->YValues->Count();  // получаем размер
double *in=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in[i]=inS->YValues->Value[i];
double *out=new double[size]; // выделяем память для выходных значений
double k=Filter(in,out,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
 k*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count();// вычисляем сдвиг фаз
//----------------------------------------------
//---------добавление для фильтрации магнитного поля
// внимание - она пока работает не корректно ибо фильтр наполняется только за
// его длину.
/*double *in2=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // выделяем память для выходных значений
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// вычисляем сдвиг фаз
*/

//----------------------------------------------
outS->Clear(); // чистим график, перед выводом

for(int i=0;i<size;i++) // выводим
{
outS->AddXY(inS->XValues->Value[i]-k,out[i],"",clBlue);
}
delete[] in;  // прибираемся
delete[] out;
return k;
}


// функция трамплин для фильтра.  ----------------------------------------------
long double TrForMassiveFilter(long double *inB,long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh)
{

    double *in=new double[lengthMassive];  // выделяем память
    for(int i=0;i<lengthMassive;i++)       // копируем
    {
    	in[i]=inY[i];
    }
    double *out=new double[lengthMassive]; // выделяем память для выходных значений
    double k=Filter(in,out,lengthMassive,lengthFilter,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
     k*=(inB[lengthMassive-1]-inB[0])/(double)lengthMassive;// вычисляем сдвиг фаз
 // если что тут максимум и минимум надо бы вычислять.

//----------------------------------------------
//---------добавление для фильтрации магнитного поля
// внимание - она пока работает не корректно ибо фильтр наполняется только за
// его длину.
/*double *in2=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // выделяем память для выходных значений
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// вычисляем сдвиг фаз
*/

//----------------------------------------------
for(int i=0;i<lengthMassive;i++) // выводим
{
	outB[i]=inB[i]-k;
	outY[i]=out[i];
}

    delete[] in;  // прибираемся
    delete[] out;
    return k;
}

template <class T>
inline T max_elem(const std::vector<T> & in)
{
    std::vector<T>::const_iterator pos;
    T max=*in.begin();
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(*pos>max)
        max=*pos;
    }
    return max;
}

template <class T>
inline T min_elem(const std::vector<T> & in)
{
    std::vector<T>::const_iterator pos;
    T min=*in.begin();
    for(pos=in.begin();pos!=in.end();++pos)
    {
        if(*pos<min)
        min=*pos;
    }
    return min;
}


// функция трамплин для фильтра.  ----------------------------------------------
long double TrForMassiveFilter(std::vector<long double> & inB,
std::vector<long double> & inY,std::vector<long double> & outB,
std::vector<long double> & outY,int lengthFilter,long double Fdisk,
long double Fpropysk,long double Fzatyh)
{
    int lengthMassive=inY.size();
    std::vector<long double> in(inY);
    std::vector<long double> out(lengthMassive);

    double k=Filter(in,out,lengthMassive,lengthFilter,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
    k*=(max_elem(inB)-min_elem(inB))/(double)lengthMassive;// вычисляем сдвиг фаз
    // разность максимума и минимума на длину массива
 
//----------------------------------------------
//---------добавление для фильтрации магнитного поля
// внимание - она пока работает не корректно ибо фильтр наполняется только за
// его длину.
/*double *in2=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // выделяем память для выходных значений
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// вычисляем сдвиг фаз
*/

//----------------------------------------------
    outB.resize(lengthMassive);
    outY=out;
    for(int i=0;i<lengthMassive;i++) // выводим
    {
	    outB[i]=inB[i]-k;
    }
    return k;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
Медианный фильтр.
Просто возвращает медиану массива.
*/
long double medianFilter(std::vector <long double> & in)
{
    std::sort(in.begin(),in.end());
    return in[in.size()/2];
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

