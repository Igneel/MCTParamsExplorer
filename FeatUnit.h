#ifndef FeatUnitH
#define FeatUnitH

#include <Series.hpp>
#include <vcl.h>
#include "UsedTypes.h"
#include "commonFunctions.h"
enum FeatType {ODD_FEAT, EVEN_FEAT};
  /*
void averageData(TSignal & inY, TSignal &outY, FeatType featType);
void featData(DataKind dataKind);
 */
// Функции ниже уже не используются

void MidCurve(TLineSeries* a, TLineSeries* b, long index);
void FoygtFeat(TLineSeries* a,TLineSeries* b, long index);
void EvenFeat(TLineSeries* a, long index);
void OddFeat(TLineSeries* a, long index);
void FeatCurve(TLineSeries* a, long index, FeatType featType);

#endif