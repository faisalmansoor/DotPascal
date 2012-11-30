#ifndef KEYWORD_MAP_H
#define KEYWORD_MAP_H

#include <string>
#include <map>
#include "global.h"

using namespace std;
class KeyWordMap
{
private:
	map<string,int> kmap;
	int kcount;
public:	
	KeyWordMap()
	{
		kcount = 0;
		Add("PROGRAM",T_program);
		Add("BEGIN",T_begin);
		Add("END",T_end);
		Add("VAR",T_var);		
		Add("FUNCTION",T_function);
		Add("PROCEDURE",T_procedure);
		Add("INTEGER",T_integer);
		Add("REAL",T_real);
		Add("ARRAY",T_array);
		Add("OF",T_of);
		Add("IF",T_if);
		Add("THEN",T_then);
		Add("ELSE",T_else);
		Add("WHILE",T_while);
		Add("DO",T_do);
		Add("NOT",T_not);		
		Add("CASE",T_case);
		Add("FOR",T_for);
		Add("TO",T_to);
		Add("STEP",T_step);
		Add("DOWNTO",T_downto);		
		Add("DEFAULT",T_default);

	}

	void Add(char* str,int index)
	{
		kcount++;
		string s = str;
		kmap[str] = index;
	}
	int Length()
	{
		return kcount;
	}
	int Get(char* str)
	{
		int id;
		string s = str;
		id =  kmap[str];
		if ( id == 0)
			return -1;
		else
			return id;
	} 

};

#endif