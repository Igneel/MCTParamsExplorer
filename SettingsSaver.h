#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H 

#include <algorithm>
#include <vcl.h>

#include "UsedTypes.h"
#include "System.hpp"

class SettingsSaver
{
public:
	SettingsSaver();
	void Add(std::string Name,std::string Value);
	void Modify(std::string Name,std::string Value);
	void Delete(std::string Name);
	void Save(AnsiString FileName);
	~SettingsSaver();

private:

	std::string convertToString(std::pair<std::string, std::string> * in);

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