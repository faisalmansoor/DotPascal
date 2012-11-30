#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include<map>
#include<string>
#include "Var.h"

using namespace std;

typedef map<int,Var*> IndexList;
typedef map<string,Var*> NameList;

class SymbolTable
{
private:
	 IndexList mvIndexList;
	 NameList  mvNameList;
	 int mvindex;
	 int mDeclCount;	 
	 int mResCount;

public:
	SymbolTable()
	{
		mvindex = 0;
		mDeclCount = 0;
	}

	void SetDeclCount(int count)
	{
		mDeclCount = count;
	}
	int GetDeclCount()
	{
		return mDeclCount;
	}
	
	void Add(Var* e)
    {
		int index = mvindex++;
		e->SetIndex(index);
		//mvNameList.insert( NameList::value_type(e->GetName(), e) );
		mvIndexList[index]  = e;
		mvNameList[e->GetName()] = e;

		Var* v = FindByIndex(0);

    }
	
	Var* FindByName(string& s)
    {
		Var* v;
		v = mvNameList[s];
		return v;
    }
	
	Var* FindByIndex(int i)
    {
		Var* v;
		v = mvIndexList[i];
		return v;
    }
	
	int Length()
    {
		return mvindex;		// number of items
    }
	void Clear()
	{
		Var* v;
		for( int i = 0; i< mvindex ; i++)
		{
			v = mvIndexList[i];
			mvNameList[v->GetName()] = NULL;
			mvIndexList[i] = NULL;
		}
		mvindex = 0;
	}
	void AdjustIndex(int size)
	{
		for( int i = 0 ; i< mvindex ; i++)
		{
			mvIndexList[i + size] = mvIndexList[i];
			mvIndexList[i] = 0;
		}
	}
	void ResCount(int r) {mResCount = r;}
	int ResCount() {return mResCount;}

	void Dump(const char* str = "",FILE *fp = stdout)
	{
		fprintf(fp,"****%s****\n",str);
		Var* v,*vn;
		for( int i = 0 ;i < mvindex ; i++)
		{
			v = mvIndexList[i];
			vn = mvNameList[v->GetName()];

			fprintf(fp,"%d : %s \n",i,vn->GetName().c_str());
		}
		fprintf(fp,"\n\n");

	}
};
#endif