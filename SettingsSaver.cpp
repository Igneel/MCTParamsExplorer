#include "SettingsSaver.h"

SettingsSaver::SettingsSaver()
{

}
//---------------------------------------------------------------------
void SettingsSaver::AddSetting(std::string Name,std::string Value)
{
	settings.push_back(make_pair(Name,Value));
	std::sort(settings.begin(), settings.end());
}


//---------------------------------------------------------------------
void SettingsSaver::ModifySetting(std::string Name,std::string Value)
{
	std::binary_search(settings.begin(), settings.end(), Name,optionNamesCompare() );
}
//---------------------------------------------------------------------
void SettingsSaver::DeleteSetting(std::string Name)
{

}
//---------------------------------------------------------------------
void SettingsSaver::SaveSetting(AnsiString FileName)
{

}
//---------------------------------------------------------------------
SettingsSaver::~SettingsSaver()
{

}
//---------------------------------------------------------------------