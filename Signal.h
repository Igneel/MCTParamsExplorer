
#ifndef SIGNAL_H
#define SIGNAL_H 

#include "commonFunctions.h"

class Signal
{
public:
	Signal(long double * array, size_t length);
	Signal(TSignal::iterator begin, TSignal::iterator end);
	virtual ~Signal();
	virtual void setSignal();
	virtual std::pair<TSignal::iterator,TSignal::iterator> getSignal();
	virtual size_t length();
	
	

	/*
	Давайте решим какой будет логика вычислений здесь.
	Вариант 1.
	Результат будет сохраняться поверх имеющихся данных.
	*/
	virtual void feat();
	virtual void average();
	virtual void filter();
	virtual void extrapolate(TSignal & B);
	/*	
	Вариант 2.
	Передаем в качестве параметров вектор, куда будет сохраняться результат.
	*/
	virtual void featTo(TSignal & s);
	virtual void averageTo(TSignal & s);
	virtual void filterTo(TSignal & s);
	virtual void extrapolateTo(TSignal & s,TSignal & B);
	/*
	Вариант 3.
	Передаем в качестве параметров вектор который нужно обработать.
	Результат сохраняем сюда.
	*/
	virtual void featFrom(TSignalIt begin, TSignalIt end);
	virtual void averageFrom(TSignalIt begin, TSignalIt end);
	virtual void filterFrom(TSignalIt begin, TSignalIt end);
	virtual void extrapolateFrom(TSignalIt begin, TSignalIt end,TSignal & B);

	void setFilterParams(FilterParams * fP);
	void setExtrapolateDegree(int degree);

	virtual void calculateFrom(); // А пусть наш сигнал можно будет вычислить на основе входных данных
	virtual void calculateFrom(TSignalIt begin, TSignalIt end, TSignalIt begin2);
	virtual void calculateFrom(TSignalIt begin, TSignalIt end);
	virtual void multiply(long double koef);
	virtual void clear();

private:
	
	TSignal signal;
	FilterParams *filterParams;
	int ExtapolatePolinomDegree;
	
};


#endif
