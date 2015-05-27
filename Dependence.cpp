#include "Dependence.h"
#include <deque.h>

Dependence::Dependence(TSignal x1, TSignal y1, bool isOdd)
{
	setY(y1);
	setX(x1);
	SymmetryCenterUpdate();
	if (isOdd)
	{
		symmetry=ODD_SYM;
	}
	else
	{
		symmetry=EVEN_SYM;
	}
}
Dependence::~Dependence()
{
}

void Dependence::setY(TSignal y1)
{
	y=y1;
}
void Dependence::setX(TSignal x1)
{
	x=x1;
}

MyDataType Dependence::getYbyX(MyDataType xp)
{
	size_t ind=indexOfElemClosestToValue(x,xp);
	if (y.size()>ind)
	{
		return y[ind];
	}
	return 123456789.0;	
}
MyDataType Dependence::getXbyY(MyDataType yp)
{
	size_t ind=indexOfElemClosestToValue(y,yp);
	if (x.size()>ind)
	{
		return x[ind];
	}
	return 123456789.0;	
}

void Dependence::Average()
{	
	/*
	Находим центр.
	Идем от него, зная индекс и значения.
	Если дельта х примерно одинакова - усредняем как обычно.
	Если какая-нибудь больше другой - делаем линейную интерполяцию, создаем точку, усредняем с ней

	Для хранения промежуточного результата используем дек.
	*/
	std::deque <long double> tx, ty;

	long double eps = 10E-3;
	SymmetryCenterUpdate();
	int i0 = indexOfElemClosestToValue(x,x0);

	tx.push_back(x0);
	ty.push_back(y0);

	for (int i = i0, j=i0; i < x.size() && j>=0; ++i, --j)
	{		
		if( fabs(x[i] + x[j] -2.0*x0)<eps) // если точки почти одинаковы, просто усредняем
		{
			if (symmetry == EVEN_SYM)
			{
				ty.push_back((y[i]+y[j])/2.0);
				//ty.push_front((y[i]+y[j])/2.0);
				tx.push_back((x[i]-x[j])/2.0);
				//tx.push_front(-(x[i]-x[j])/2.0);
			}
			if (symmetry == ODD_SYM)
			{
				ty.push_back((y[i]-y[j])/2.0);
				//ty.push_front((y[i]-y[j])/2.0);
				tx.push_back((x[i]-x[j])/2.0);
				//tx.push_front(-(x[i]-x[j])/2.0);
				//y[i]=y[j]=(y[i]-y[j])/2.0;
			}
		}
		else // если точки разные, придется интерполировать
		{
			/*
			если линейно то
			y1=k*x1+b
			y2=k*x2+b

			b=y1-k*x1

			y2=k*x2+y1-k*x1
			k(x2-x1)=y1-y2

			k = (y1-y2)/(x2-x1);
			b = y1-k*x1;

			*/

			long double k;
			long double b;
			long double yn;



			if (fabs(x[i])>fabs(x[j]))
			{
				if(i-1 < 0) break;

				k = (y[i]-y[i-1])/(x[i]-x[i-1]);
				b = y[i]-k*x[i];
				yn = -k*x[j]+b;

				if (symmetry == EVEN_SYM)
				{
					ty.push_back((yn+y[j])/2.0);
					//ty.push_front((yn+y[j])/2.0);
					tx.push_back(x[j]);
					//tx.push_front(-x[j]);
					//y[i]=y[j]=(yn+y[j])/2.0;
				}
				if (symmetry == ODD_SYM)
				{
					ty.push_back((yn-y[j])/2.0);
					//ty.push_front((yn-y[j])/2.0);
					tx.push_back(x[j]);
					//tx.push_front(-x[j]);
					//y[i]=y[j]=(yn-y[j])/2.0;
				}
			}
			else
			{
				if(j-1 < 0) break;
				k = (y[j]-y[j-1])/(x[j]-x[j-1]);
				b = y[j]-k*x[j];
				yn = -k*x[i]+b;

				if (symmetry == EVEN_SYM)
				{
					ty.push_back((y[i]+yn)/2.0);
					//ty.push_front((y[i]+yn)/2.0);
					tx.push_back(x[i]);
					//tx.push_front(-x[i]);
					//y[i]=y[j]=(y[i]+yn)/2.0;
				}
				if (symmetry == ODD_SYM)
				{
					ty.push_back((y[i]-yn)/2.0);
					//ty.push_front((y[i]-yn)/2.0);
					tx.push_back(x[i]);
					//tx.push_front(-x[i]);
					//y[i]=y[j]=(y[i]-yn)/2.0;
				}
			}
		}

	}

	x.clear();
	y.clear();

	for (int i = 0; i < tx.size(); ++i)
	{
		x.push_back(tx[i]);
		y.push_back(ty[i]);
	}


/*

	for (int i = 0; i < x.size(); ++i)
		{
			MyDataType x = this->x[i];
			size_t i1 = indexOfElemClosestToValueFromLeft(y,getYbyX(x-x0));
			size_t i2 = indexOfElemClosestToValueFromRight(y,getYbyX(-x+x0));
			if (symmetry == EVEN_SYM)
			{		
				y[i1] = y[i2] = (getYbyX(x-x0)+getYbyX(-x+x0))/2.0;
			}
			if (symmetry == ODD_SYM)
			{
				y[i1] = y[i2] = (getYbyX(x-x0)-getYbyX(-x+x0))/2.0;
			}
		}	// её надо отлаживать.
*/	
}
// пересчет координат центра симметрии
void Dependence::SymmetryCenterUpdate()
{
	if (symmetry==EVEN_SYM)
	{
		// Если это магнитосопротивление - то находим минимум - это и будет центром симметрии.
		MyDataType min = min_abs_elem(y);		
		y0=min;
		x0= getXbyY(min);
	}
	if (symmetry==ODD_SYM)
	{
		// а тут пока просто B==0
		y0=getYbyX(0);
		x0=getXbyY(y0);
	}
}

// весь сигнал можно получить вот так
const TSignal * Dependence::getY()
{
	return &y;
}
const TSignal * Dependence::getX()
{
	return &x;
}

// координаты центра симметрии
MyDataType Dependence::getX0()
{
	return x0;
}
MyDataType Dependence::getY0()
{
	return y0;
}