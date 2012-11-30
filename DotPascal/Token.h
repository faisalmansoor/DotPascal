#ifndef TOKEN_H
#define TOKEN_H

#include<string.h>
#include<string>
using namespace std;

#include "global.h"

struct Token
{
	int  Class;	
	int  Value;
	int  LineNo;
	string Name;				
	Token()
	{
		Class = 0;
		Value = 0;
		LineNo = -1;
	}
	void Set(int c,int v,int l)
	{
		Class = c;
		Value = v;
		LineNo = l;
	}	
	void Set(int c,int v)
	{
		Class = c;
		Value = v;
	}
	void Set(int c)
	{
		Class = c;
	}
	void Dump()
	{
		printf("%s\n",Name.c_str());
		printf("Value = %d\n",Value);
		printf("Class = %d\n",Class);	
	}
};
#endif
