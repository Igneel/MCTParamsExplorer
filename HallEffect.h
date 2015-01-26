
#include "Signal.h"

class THallEffect : public Signal
{
public:
	THallEffect(long double * array, size_t length);
	THallEffect(TSignalIt begin, TSignalIt end);
	~THallEffect();
	virtual void featTo(TSignal & s);
	virtual void averageTo(TSignal & s);
	virtual void filterTo(TSignal & s);
	virtual void extrapolateTo(TSignal & s,TSignal & B);
	
};