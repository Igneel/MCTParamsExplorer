#ifndef DependenceH
#define DependenceH

#include "UsedTypes.h"
#include "commonfunctions.h"

class Dependence
{
public:
	Dependence(TSignal x, TSignal y, bool isOdd);
	~Dependence();

	void setY(TSignal y);
	void setX(TSignal x);

	// весь сигнал можно получить вот так
	const TSignal * getY();
	const TSignal * getX();

	// координаты центра симметрии
	MyDataType getX0();
	MyDataType getY0();
	

	MyDataType getYbyX(MyDataType x);
	MyDataType getXbyY(MyDataType y);

	void Average();

private:

	// пересчет координат центра симметрии
	void SymmetryCenterUpdate();

	enum SymmetryType
	{
		EVEN_SYM, ODD_SYM		
	};

	TSignal x;
	TSignal y;

	MyDataType x0, y0;

	SymmetryType symmetry;
	
};

#endif