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

#endif

