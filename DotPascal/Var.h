#ifndef VAR_H
#define VAR_H

#include<string>
#include<vector>
#include"global.h"

using namespace std;
class SymbolTable;	// Forward Declaration
typedef SymbolTable VarList;

class Var
{
private :
	int vClass;
	int type; // Is Array , Simple , Function , Proceure etc
	int subType; // Sub Type INT REAL
	int lb;
	int ub;
	string name;
	VarList* methodparams;	/* possible param varlist For Functions*/
	Var* resultType;
	int index;
	int scope;
	int pnum;	// Pos of parameter in the parameter list
	int dnum;   // Pos of parameter in the decl list
	bool neg;  // specifies weather unarray neg is applied on it or not
	bool lvalue;	
	vector<Bounds>* boundList; // array bound list 

	enum { 
			VarFunction,
		    VarProcedure,
		    VarArray,
			VarArrayIndex
		};
	
public:
	Var()
	{
		memset(this,0,sizeof Var );
		scope = 1;		
		boundList = NULL;
	}
	int GetTypeId() { return type; }
	void SetTypeId(int t) { type = t; }
	string GetName() { return name; }
	void SetName(string& s) { name = s; }
	VarList* GetParams() { return methodparams; }
	void SetParams(VarList* p) { methodparams = p; }
	int GetIndex() { return index; }
	void SetIndex(int i) { index = i; }
	void SetSubTypeId(int st) { subType = st;}
	int GetSubTypeId() { return subType; }
	void SetLb(int b) { lb = b; }
	int GetLb() { return lb; }
	void SetUb(int b) { ub = b; }
	int GetUb() { return ub; }	
	void SetResultType(Var* r) { resultType = r ; }
	Var* GetResultType() { return resultType ; }
	int GetScope()  { return scope; }
	void SetScope(int s ) { scope = s; }
	int GetClassId() { return vClass;}
	void SetClassId(int c) { vClass = c;}
	int GetParamIndex() { return pnum;}
	void SetParamIndex(int p) { pnum = p;}
	void SetDeclIndex(int d) { dnum = d;}
	int GetDeclIndex() { return dnum;}
	bool GetNeg() { return neg;}
	void SetNeg(bool s) { neg = s;}
	bool LValue() { return lvalue;}
	void LValue(bool b) { lvalue = b;}
	bool IsTemp() 
	{
		if( vClass == VAR_TEMP  || type == VAR_TEMP )
			return true;
		else
			return false;
	}
	 
	void AddBound(int lb,int ub) 
	{
		if( boundList == NULL )
		{
			boundList = new vector<Bounds>;
		}
		Bounds b; 
		b.lb = lb ;
		b.ub = ub;
		boundList->push_back(b);
	}
	bool CompareBoundList(Var* v)
	{
		Bounds b1,b2;
		if( boundList->size() != v->boundList->size() )
			return false;
		for( int i = 0 ;i < boundList->size() ; i++)
		{
			b1 = boundList->at(i);
			b2 = v->boundList->at(i);
			if( b1 == b2 )
				return false;			  
		}
		return true;
	}
	int GetDimension()
	{
		return boundList->size();
	}
	Bounds GetBound(int index)
	{
		return (*boundList)[index];
	}
	bool CheckBound(int index,int d)
	{
		if( d >= (*boundList)[index].lb || d <= (*boundList)[index].ub )
			return true;
		return false;		
	}
	vector<Bounds>* GetBoundList()
	{
		return boundList;
	}
	void  SetBoundList(vector<Bounds>* bl)
	{
		boundList = bl;
	}	
	int GetArrayLength()
	{
		Bounds b;
		int size = 1;
		for( int i = 0 ;i < boundList->size() ; i++)
		{
			b = (*boundList)[i];
			size*= (b.ub - b.lb + 1);
		}
		return size;
	}
	string ToString()
	{
		char temp[10];
		string s;
		s += "(Id= ";
		s += itoa(type,temp,10);
		s += " Name :";
		if( type == VAR_TEMP )
			s += "VAR_TEMP";
		else
			s += name;
		s += ")";
		return s;
	}	
};
#endif
