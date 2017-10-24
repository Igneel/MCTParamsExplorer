//---------------------------------------------------------------------------


#pragma hdrstop

#include "NoiseUnit.h"



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// сохранение результатов без шума
// преобразование Бокса-Мюллера
void BoxMull(long double *z,int l)
{
	long double x,y;
    long double s;
	int i;
	// вся эпичность этой ошибки потрясает%)
	// это ж надо было не подумать, что
	// если l - число нечетное, то мы вылезем за границу)
	for(i=0;i<l;i+=2)
	{
		do
		{
			x=(-1000+rand()%2000)/1000.0;
			y=(-1000+rand()%2000)/1000.0;
			// шум в пределах от -1 до 1
			s=x*x+y*y;
		}while (s>1.0 || s<=0.0);
		z[i]=x*sqrt(-2.0*log(s)/s);
		if((i+1)<l)
			z[i+1]=y*sqrt(-2.0*log(s)/s);
	}
}

void BoxMull(std::vector<long double> &z,int l)
{
	long double x,y;
    long double s;
	int i;
	// вся эпичность этой ошибки потрясает%)
	// это ж надо было не подумать, что
	// если l - число нечетное, то мы вылезем за границу)
	for(i=0;i<l;i+=2)
	{
		do
		{
			x=(-1000+rand()%2000)/1000.0;
			y=(-1000+rand()%2000)/1000.0;
			// шум в пределах от -1 до 1
			s=x*x+y*y;
		}while (s>1.0 || s<=0.0);
		z[i]=x*sqrt(-2.0*log(s)/s);
		if((i+1)<l)
			z[i+1]=y*sqrt(-2.0*log(s)/s);
	}
}


//--------------------------------------------------------------------------
// расчет среднего квадратичного отклонения
//--------------------------------------------------------------------------

long double Sko (const long double *x0,const long double *x,int l)
{
	long double z=0;
	for(int i=0;i<l;i++)
	z+= pow(fabs((fabs(x[i])-fabs(x0[i]))),2);
	z/=(long double)l;
	return sqrt(z);
}

long double Sko (std::vector<long double> const &x0,std::vector<long double> const &x,int l)
{
	long double z=0;
	for(int i=0;i<l;i++)
	z+= pow(fabs((fabs(x[i])-fabs(x0[i]))),2);
	z/=(long double)l;
	return sqrt(z);
}

//---------------------------------------------------
// расчет математического ожидания
//-------------------------------------------------

 long double Mo (const long double *x,int l)
 {
	 long double M=0;
	 for(int i=0;i<l;i++)
	 M+=x[i];
	 return M/l;
 }

 long double Mo (std::vector<long double> const &x,int l)
 {
	 long double M=0;
	 for(int i=0;i<l;i++)
	 M+=x[i];
	 return M/l;
 }

//---------------------------------------------------------------------------
// возвращает величину математического ожидания шума.
void ShumAdding(const long double *x,long double *out,long double *ret, long double koeff,const int l)
{


long double *y= new long double[l]; // шум для нашего графика

BoxMull(y,l);
for(int i=0;i<l;i++)
	out[i]=x[i]+y[i]/koeff;

if(fabs(out[l-1])>10000)
{
long double temp1=x[l-1];
long double temp2=out[l-1];
long double temp3=y[l-1];
long double temp4=koeff;
ShowMessage("Сейчас вылезет:)");
}
	// амплитуда шума определяется как 1/koef
  ret[0]=Mo(y,l); // математическое ожидание
  ret[1]=Sko(x,out,l);  // СКО
  ret[2]=ret[0]/Mo(x,l)*100; // СКО в %  от математического ожидания исходного графика
  delete[] y;
}


void ShumAdding(std::vector<long double> const &x,std::vector<long double> &out,
	std::vector<long double> &ret, long double koeff,const int l)
{
std::vector<long double> y;
y.resize(l);

BoxMull(y,l);
for(int i=0;i<l;i++)
	out[i]=x[i]+y[i]/koeff;

	// амплитуда шума определяется как 1/koef
	ret.push_back(Mo(y,l)); // математическое ожидание
	ret.push_back(Sko(x,out,l));// СКО
	ret.push_back(ret[0]/Mo(x,l)*100);// СКО в %  от математического ожидания исходного графика
}

// Генератор шума, с квантованием по уровню
// Также тут реализована функция, которая квантует сам сигнал по уровню.
void QuantumNoiseGenerator(std::vector<long double> &buf,long double mzr, int amplitude, int n)
{
/*
Стоит вопрос в частоте нулей, по сути при измерениях у нас нулей скажем 81%
Видимо тут надо сделать аналогично.
*/
	for (int i = 0; i < n; ++i) {
		int val=rand()%(50*amplitude+1);
		if(val>amplitude) val=0;
		int sign=rand()%2;
		long double resPoint=(sign==0?1:-1)*val*mzr;
		buf[i]=resPoint;
	}
}

void QuantumNoiseGenerator(long double * buf,long double mzr, int amplitude, int n)
{
	for (int i = 0; i < n; ++i) {
		int val=rand()%(amplitude+1);
		int sign=rand()%2;
		long double resPoint=(sign==0?1:-1)*val*mzr;
		buf[i]=resPoint;
	}
}
void SignalQuantifier(std::vector<long double> & signal,long double mzr,int n)
{
	 for (int i = 0; i < n; ++i) {
		int temp=floor(signal[i]/mzr+0.5);
		signal[i]=temp*mzr;
	}
}

void SignalQuantifier(long double * signal,long double mzr,int n)
{
     for (int i = 0; i < n; ++i) {
		int temp=floor(signal[i]/mzr+0.5);
		signal[i]=temp*mzr;
	}
}

void QuantumShumAdding(std::vector<long double> const &x,std::vector<long double> &out,
	std::vector<long double> &ret, long double koeff,const int l,long double mzr,
	int amplitude)
{
std::vector<long double> y;
y.resize(l);
std::vector<long double> ytemp;
ytemp.resize(l);
QuantumNoiseGenerator(y,mzr,amplitude,l);

// Ахтунг тут усреднение добавлено!   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /*
for (int i = 0; i < 299; ++i) {
	QuantumNoiseGenerator(ytemp,mzr,amplitude,l);
	for (int j = 0; j < l; ++j) {
		y[j]+=ytemp[j];
	}
	
} 

for (int j = 0; j < l; ++j) {
		y[j]/=300.0;
	}
          */
std::vector<long double> qx;
qx.resize(l);
for(int i=0;i<l;i++)
 {
	qx[i]=x[i];
 }

 SignalQuantifier(qx,mzr,l);
for(int i=0;i<l;i++)
	out[i]=qx[i]+y[i];

	ret.push_back(Mo(y,l)); // математическое ожидание
	ret.push_back(Sko(x,out,l));// СКО
	ret.push_back(ret[0]/Mo(x,l)*100);// СКО в %  от математического ожидания исходного графика
}

#pragma package(smart_init)
