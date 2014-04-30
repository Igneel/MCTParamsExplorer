#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H 

#include <algorithm>

#include "UsedTypes.h"
#include "System.hpp"

class SettingsSaver
{
public:
	SettingsSaver();
	void AddSetting(std::string Name,std::string Value);
	void ModifySetting(std::string Name,std::string Value);
	void DeleteSetting(std::string Name);
	void SaveSetting(AnsiString FileName);
	~SettingsSaver();

private:

	optionDescription settings;
	//---Нужна для поиска элементов.

	struct optionNamesCompare {
    // important: we need two overloads, because the comparison
    // needs to be done both ways to check for equality

	bool operator()(const std::string &a, const std::pair<std::string,std::string> &b)
	{
		if (a==b.first)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator()(const std::pair<std::string,std::string> &b,const std::string &a)
	{
		if (a==b.first)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	};

	
};

#endif