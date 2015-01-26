
#include "Signal.h"

class MagneticField : public Signal
{
public:
	MagneticField(long double * array, size_t length);
	MagneticField(TSignalIt begin, TSignalIt end);
	~MagneticField();
	virtual void featTo(TSignal & s);
	virtual void averageTo(TSignal & s);
	virtual void filterTo(TSignal & s);
	
};