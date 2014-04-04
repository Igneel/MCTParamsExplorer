
#include "FilterParams.h"

FilterParams::FilterParams()
{
    SamplingFrequecy=10000;
    BandwidthFrequency=15;
    AttenuationFrequency=25;
    filterLength=50;
}

FilterParams::FilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    setFilterParams(samplingFrequecy, bandwidthFrequency, attenuationFrequency, length);
}

FilterParams::FilterParams(String samplingFrequecy,String bandwidthFrequency,String attenuationFrequency, String length)
{
    setFilterParams(StrToFloat(samplingFrequecy),StrToFloat(bandwidthFrequency),StrToFloat(attenuationFrequency),StrToInt(length));
}

void FilterParams::setFilterParams(MyDataType samplingFrequecy,MyDataType bandwidthFrequency,MyDataType attenuationFrequency, int length)
{
    SamplingFrequecy=samplingFrequecy;
    BandwidthFrequency=bandwidthFrequency;
    AttenuationFrequency=attenuationFrequency;
    filterLength=length;    
}