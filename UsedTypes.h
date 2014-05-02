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

enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA, AVERAGED_DATA};

const long double THEALMOSTZERO = 0.000001;


#endif

