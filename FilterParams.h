#ifndef FILTERPARAMSH
#define FILTERPARAMSH

#include "UsedTypes.h"
#include <system.hpp>
#include <SysUtils.hpp>

class FilterParams
{
public:
	FilterParams();
	FilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);
	FilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length);
	void setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length);

	MyDataType SamplingFrequecy;
	MyDataType BandwidthFrequency;
	MyDataType AttenuationFrequency;
	int filterLength;
};

#endif