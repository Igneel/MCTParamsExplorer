#ifndef FeatUnitH
#define FeatUnitH

#include <Series.hpp>
#include <vcl.h>
enum FeatType {ODD_FEAT, EVEN_FEAT};

void MidCurve(TLineSeries* a, TLineSeries* b, long index);
void FoygtFeat(TLineSeries* a,TLineSeries* b, long index);
void EvenFeat(TLineSeries* a, long index);
void OddFeat(TLineSeries* a, long index);
void FeatCurve(TLineSeries* a, long index, FeatType featType);

#endif