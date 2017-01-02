//---------------------------------------------------------------------------


#pragma hdrstop

#include "clMagneticFieldDependences.h"


void clMagneticFieldDependences::calculateTenzorFromCarrierParams()
{
  for(int i=0;i<NumberOfPoints;i++)
  {
    sxx[i]=sxy[i]=0;
    int NumberOfCarrierTypes=carrierParams->getNumberOfCarrierTypes();

    for(int j=0;j<NumberOfCarrierTypes;j++)
    {
      sxx[i]+=electronCharge*carrierParams->getConcentration(j)*carrierParams->getMobility(j)/(1.0+
      carrierParams->getMobility(j)*carrierParams->getMobility(j)*B[i]*B[i]);

      sxy[i]+=electronCharge*carrierParams->getConcentration(j)*
      carrierParams->getMobility(j)*carrierParams->getMobility(j)*
      B[i]/(1.0+carrierParams->getMobility(j)*
      carrierParams->getMobility(j)*B[i]*B[i]);
    }
  }

}

void clMagneticFieldDependences::calculateEffectiveParamsFromTenzor()
{
  for(int i=0;i<NumberOfPoints;i++)
  {
    s_eff[i]=(sxx[i]*sxx[i]+sxy[i]*sxy[i])/sxx[i];
    Rh_eff[i]=sxy[i]/(sxx[i]*sxx[i]+sxy[i]*sxy[i]);
  }

}

void clMagneticFieldDependences::calculateSignalsFromEffectiveParams()
{
  for(int i=0;i<NumberOfPoints;i++)
  {
    Us[i]=carrierParams->getCBRatio()/carrierParams->getThickness()*carrierParams->getCurrentIntensity()/s_eff[i];
    Uy[i]=Rh_eff[i]*carrierParams->getCurrentIntensity()/carrierParams->getThickness();
  }

}
void clMagneticFieldDependences::calculateEffectiveParamsFromSignals()
{
  for (int i = 0; i < NumberOfPoints ; i++)
  {
    if(fabs(Us[i])<THEALMOSTZERO)
      s_eff[i]=0;
    else
    {
      s_eff[i]=carrierParams->getCBRatio()/carrierParams->getThickness()*carrierParams->getCurrentIntensity()/Us[i];
    }
    Rh_eff[i]=carrierParams->getThickness()*Uy[i]/carrierParams->getCurrentIntensity();
  }

}

void clMagneticFieldDependences::calculateTenzorFromEffectiveParams()
{
  for (int i = 0; i < NumberOfPoints ; i++)
  {
    sxx[i]=s_eff[i]/
      (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1);
    sxy[i]=s_eff[i]*s_eff[i]*Rh_eff[i]/
       (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1);
  }
}















#pragma package(smart_init)
