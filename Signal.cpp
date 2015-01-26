
#include "Signal.h"

Signal::Signal(long double * array, size_t length)
{
	for (int i = 0; i < length; ++i)
	{
		signal.push_back(array[i]);
	}
}

Signal::Signal(TSignalIt begin, TSignalIt end)
{
	signal(begin,end);
}

virtual Signal::~Signal()
{
	signal.clear();
	filterParams->~FilterParams();
}

virtual void Signal::setSignal(TSignalIt begin, TSignalIt end)
{
	clear();
	signal(begin, end);
}

virtual std::pair<TSignal::iterator,TSignal::iterator> Signal::getSignal()
{
	return make_pair(signal.begin(), signal.end());
}

virtual size_t Signal::length()
{
	return signal.size();
}

virtual void Signal::feat()
{
	featTo(signal);
}

virtual void Signal::average()
{
	averageTo(signal);
}

virtual void Signal::filter()
{
	filterTo(signal);
}

virtual void Signal::extrapolate(TSignal & B)
{
	extrapolateTo(signal,B);
}

virtual void Signal::featFrom(TSignalIt begin, TSignalIt end)
{
	setSignal(begin,end);
	feat();
}

virtual void averageFrom(TSignalIt begin, TSignalIt end)
{
	setSignal(begin,end);
	average();
}

virtual void filterFrom(TSignalIt begin, TSignalIt end)
{
	setSignal(begin,end);
	filter();
}

virtual void extrapolateFrom(TSignalIt begin, TSignalIt end,TSignal & B)
{
	setSignal(begin,end);
	extrapolate(B);
}

void Signal::setFilterParams(long double samplingFrequecy, long double bandwidthFrequency, long double attenuationFrequency, long double length )
{
	filterParamsHall->setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
}

void Signal::setExtrapolateDegree(int degree)
{
	ExtapolatePolinomDegree=degree;
}

virtual void Signal::multiply(long double koef)
{
	for (std::vector<>::iterator i = signal.begin(); i != signal.end(); ++i)
	{
		*i*=koef;
	}
}

virtual void Signal::clear()
{
	signal.clear();
}
