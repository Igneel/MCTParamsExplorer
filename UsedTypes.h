#ifndef UsedTypesH
#define UsedTypesH

#include <vector>
#include <utility>
#include <string>

typedef long double MyDataType;
typedef std::vector<MyDataType> DataTypeInContainer;
typedef std::vector<DataTypeInContainer> TwoDimensionalContainer;

typedef std::vector< std::pair<int, int> > channelsInfo;
typedef std::vector< std::pair<std::string, std::string> > optionDescription;

enum DataKind {CURRENT_DATA=0, FILTERED_DATA=1, EXTRAPOLATED_DATA=2, AVERAGED_DATA=3};

enum ParamsType {DIRECT=0,REVERSE=1,COMBINE=2};

const long double THEALMOSTZERO = 0.000001;




#endif

