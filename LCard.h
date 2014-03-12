#ifndef LCardH
#define LCardH

//инициализация драйвера, настройка, запуск/останов измерений, получение данных
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