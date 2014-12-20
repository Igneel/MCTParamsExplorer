#ifndef UsedTypesH
#define UsedTypesH

#include <vector>
#include <utility>
#include <string>
#include <set>

typedef long double MyDataType;
typedef std::vector<MyDataType> DataTypeInContainer;
typedef std::vector<DataTypeInContainer> TwoDimensionalContainer;

typedef std::set<std::pair<std::string, DataTypeInContainer> >DataSet;

typedef std::vector< std::pair<int, int> > channelsInfo;
typedef std::vector< std::pair<std::string, std::string> > optionDescription;

enum DataKind {CURRENT_DATA=0, FILTERED_DATA=1, EXTRAPOLATED_DATA=2, AVERAGED_DATA=3};

enum SignalType {MAGNETIC_FIELD=0, HALL_EFFECT=1, MAGNETORESISTANCE=2, PHOTOCONDUCTIVITY_FARADEY=3, PHOTOCONDICTIVITY_FOYGHT=4,MAGNETIC_FIELD_F=5, SXX=6,SXY=7};
//enum PlotType {MAGNETIC_FIELD, HALL_EFFECT, MAGNETORESISTANCE,MAGNETIC_FIELD_F, SXX,SXY};
enum ParamsType {DIRECT=0,REVERSE=1,COMBINE=2};

const long double THEALMOSTZERO = 0.000001;




#endif

