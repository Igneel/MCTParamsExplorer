#ifndef LCardH
#define LCardH

//������������� ��������, ���������, ������/������� ���������, ��������� ������
class LCardADC
{
public:
LCardADC();
~LCardADC();

private:
bool DrivetInit();
bool SettingADCParams();

void StartMeasurement();
void StopMeasurement();

getData();
clearBuffer();

};

#endif