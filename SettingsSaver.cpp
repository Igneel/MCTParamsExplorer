#include "SettingsSaver.h"

SettingsSaver::SettingsSaver()
{

}
//---------------------------------------------------------------------
void SettingsSaver::Add(std::string Name,std::string Value)
{
	if(!std::binary_search(settings.begin(), settings.end(), Name,optionNamesCompare()))
	{
		settings.push_back(make_pair(Name,Value));
		std::sort(settings.begin(), settings.end());
	}
	else
	{
	Modify(Name,Value);	
	}
}


//---------------------------------------------------------------------
void SettingsSaver::Modify(std::string Name,std::string Value)
{
	std::pair<optionDescription::iterator,optionDescription::iterator> range;
	range=std::equal_range(settings.begin(), settings.end(), Name,optionNamesCompare() );
	if (range.first!=range.second)
	{
		range.first->second=Value;
	}
}
//---------------------------------------------------------------------
void SettingsSaver::Delete(std::string Name)
{
	std::pair<optionDescription::iterator,optionDescription::iterator> range;
	range=std::equal_range(settings.begin(), settings.end(), Name,optionNamesCompare() );
	if (range.first!=range.second)
	{
	   //	range.first->
	}
}
//---------------------------------------------------------------------
void SettingsSaver::Save(AnsiString FileName)
{
	TStringList *tsl= new TStringList();
	for (optionDescription::iterator i = settings.begin(); i != settings.end(); ++i)
	{
	tsl->Add(convertToString(i).c_str());	
	}
	tsl->SaveToFile(FileName);
	delete tsl;
}
//---------------------------------------------------------------------
SettingsSaver::~SettingsSaver()
{

}
//---------------------------------------------------------------------
std::string SettingsSaver::convertToString(std::pair<std::string, std::string> *in)
{
	return in->first + "\t" + in->second;	
}