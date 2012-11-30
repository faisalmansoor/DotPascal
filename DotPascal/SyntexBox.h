#ifndef SYNTEX_BOX_H
#define SYNTEX_BOX_H

#define LIMIT 10

#include "global.h"
#include "TokenStream.h"
#include "Token.h"
#include "Error.h"
#include "ErrManager.h"
#include "SymbolTable.h"
#include "Var.h"
#include "ILGenerator.h"

#include<stack>
#include<list>
#include<string>
#include<stdio.h>
using namespace std;

typedef SymbolTable VarList;

class SyntexBox
{
private:
	TokenStream& mTStr;
	Token*		 mpTok;

	FILE* mFpDmp;

	string temp;
	FILE*  treeFp;
	string code;

	string mClassName;

	int mScope;
	// 0 means class
	// 1 means function
	int mCurrST;
	
	int mArraySimple; // 0 Array
					  // 1 SImpple

	int mIndScope;

	int mLastParamListSize; 
	
	/// Stacks////////////////////////////
	stack<Var*> mSStack; // Sementic Stack
	stack<int> mParamCountStack;  // Stack For No of parametaers for the func call
	
	// holds symbol table for current function and procedure
	SymbolTable mLocalST; 
		

	//Symbol table for class
	SymbolTable mSTRoot; 
		

	vector<Var*> mParamList;

	ILGenerator& mEmit;

	/////////////////////////
	void InitSStack()
	{
		/// Adding Write
		Var* fv = new Var;
		fv->SetTypeId(VAR_PROCEDURE);
		fv->SetName(string("WRITE"));
		mSTRoot.Add(fv);

		/// Adding READ
		fv = new Var;
		fv->SetTypeId(VAR_FUNCTION);
		fv->SetName(string("READ"));
		VarList* vl = new VarList;
		fv->SetParams(vl);
		Var* rt = new Var;
		rt->SetTypeId(VAR_RETURN_VAL);
		rt->SetSubTypeId(VAR_INT);
		fv->SetResultType(rt);
		mSTRoot.Add(fv);

		/// Adding READINT
		fv = new Var;
		fv->SetTypeId(VAR_FUNCTION);
		fv->SetName(string("READINT"));
		vl = new VarList;
		fv->SetParams(vl);
		rt = new Var;
		rt->SetTypeId(VAR_RETURN_VAL);
		rt->SetSubTypeId(VAR_INT);
		fv->SetResultType(rt);
		mSTRoot.Add(fv);

		/// Adding READREAL
		fv = new Var;
		fv->SetTypeId(VAR_FUNCTION);
		fv->SetName(string("READREAL"));
		vl = new VarList;
		fv->SetParams(vl);
		rt = new Var;
		rt->SetTypeId(VAR_RETURN_VAL);
		rt->SetSubTypeId(VAR_REAL);
		fv->SetResultType(rt);
		mSTRoot.Add(fv);

		mSTRoot.ResCount(4);
	}

	/////////////////////////	
	void GenResFunc()
	{
		//READ
		Gen(".method public hidebysig static int32  READ() cil managed ");
		Gen("{");		
		Gen("call       string [mscorlib]System.Console::ReadLine()");
		Gen("call       int32 [mscorlib]System.Int32::Parse(string)");		
		Gen("ret");
		Gen("}// end of method ReadInt");

		// READ INT
		Gen(".method public hidebysig static int32  READINT() cil managed ");
		Gen("{");		
		Gen("call       string [mscorlib]System.Console::ReadLine()");
		Gen("call       int32 [mscorlib]System.Int32::Parse(string)");		
		Gen("ret");
		Gen("}// end of method ReadInt");

		//READREAL
		Gen(".method public hidebysig static float64  READREAL() cil managed");
		Gen("{");		
		Gen("call       string [mscorlib]System.Console::ReadLine()");
		Gen("call       float64 [mscorlib]System.Double::Parse(string)");		
		Gen("ret");
		Gen("} // end of method ReadReal");

	}

	///////////////////////////////////////
	void GenConstructor()
	{
		//// .ctor
		Gen(".method public hidebysig specialname rtspecialname ");
		Gen("instance void  .ctor() cil managed");
		Gen("{");
		Gen(".maxstack  1");
		Gen("ldarg.0");
		Gen("call       instance void [mscorlib]System.Object::.ctor()");
		Gen("ret");
		Gen("} // end of method .ctor");
		
		//// .cctor

		Gen(".method private hidebysig specialname rtspecialname static ");
		Gen("void  .cctor() cil managed");
		Gen("{");
		

		int size = mSTRoot.Length();
		Var* v;

		// start from 2 because first entry is write
		for( int i = mSTRoot.ResCount() ;i < size ; i++)
		{ 
			v = mSTRoot.FindByIndex(i);

			if( v->GetTypeId() == VAR_ARRAY )
			{
				char t[LIMIT];
				code = "ldc.i4." ;
				code += itoa(v->GetArrayLength(),t,10);
				Gen(code);
				code = "newarr     [mscorlib]System.";
				
				if( v->GetSubTypeId() == VAR_INT )
					code += "Int32";
				else if( v->GetSubTypeId() == VAR_REAL )
					code += "Double";
				else
					Err("Internal Compiler Error in ' GenConstructor' no sub type assigned to array");

				Gen(code);
				Gen("stsfld     " + GetStringType(v->GetSubTypeId() ) 
					+ "[] " + mClassName + "::" + v->GetName() );
			
			}			
		}	
		
		Gen("ret");
		Gen("} // end of method .cctor");
	}	

	void GenLocalArrayInit()
	{
		int size = mLocalST.Length();
		Var* v;
				
		mIndScope++;
		for( int i = 0 ;i < size ; i++)
		{ 
			v = mLocalST.FindByIndex(i);

			if(v->GetClassId() == VAR_LOCAL )
			{
				
				
				if( v->GetTypeId() == VAR_ARRAY )
				{
					char t[LIMIT];
					Load(VAR_INT,v->GetArrayLength());					
					code = "newarr     [mscorlib]System.";
					if( v->GetSubTypeId() == VAR_INT )
						code += "Int32";
					else if( v->GetSubTypeId() == VAR_REAL )
						code += "Double";
					else
						Err("Internal Compiler Error in ' GenConstructor' no sub type assigned to array");
					
					Gen(code);				
					Gen(string("stloc.") + itoa(v->GetDeclIndex(),t,10) );
				}			
			}
		}			
		mIndScope--;
	}
	
	void Gen(const char* str)
	{
		string s;
		for( int i = 0 ; i < mIndScope ;i++)			
		{
			s += " ";
		}
		s += str;		
		mEmit.Add(s);				
	}
	void GenComment(string& str)
	{
		string s = "";
		for( int i = 0 ; i < mIndScope ;i++)			
		{
			s += " ";
		}
		
		str = "//" + str;
		s += str;
		mEmit.Add(s);
	}
	void GenComment(const char* str)
	{
		string s = "";
		for( int i = 0 ; i < mIndScope ;i++)			
		{
			s += " ";
		}
		
		s += "//" ;
		s += str ;
		s += " ";
		mEmit.Add(s);
	}	
	void Gen(string& str)
	{
		string s = "";
		for( int i = 0 ; i < mIndScope ;i++)			
		{
			s += " ";
		}		
		s += str;		
		mEmit.Add(s);
	}

	string GetStringType( int t )
	{
		switch( t )
		{
		case VAR_INT:
			return string("int32");
		case VAR_REAL:
			return string("float64");
		default:
			Err("Unhandles Type");
		}
		return "";
	}
	string GetStringOp(int op)
	{
		switch(op)
		{
		case OP_plus:
			return "add";
		case OP_minus:
			return "sub";
		case OP_or:
			return "or";
		case OP_mul:
			return "mul";
		case OP_div:
			return "div";
		case OP_mod:
			return "rem";
		case OP_and:
			return "and";
		}
		return "";
	}
		
	/* recognize and translate a relational "equal" */
	void RelEQ()
	{
		A();
		SimpleExpression();
		RelTypeCheckAndLoad();
		Gen("ceq");
		
		Var* v = new Var;	 	
		v->SetTypeId(VAR_SIMPLE);
		v->SetClassId(VAR_TEMP);
		v->SetSubTypeId(VAR_INT);
		mSStack.push(v);

	}
	
	/* recognize and translate a relational "not equal" */
	void RelNEQ()
	{
		A();
		SimpleExpression();			
		RelTypeCheckAndLoad();
		Gen("ceq");
		Load(VAR_INT,0);
		Gen("ceq");

		Var* v = new Var;
		v->SetTypeId(VAR_SIMPLE);
		v->SetClassId(VAR_TEMP);
		v->SetSubTypeId(VAR_INT);
		mSStack.push(v);

	}
	
	/* recognize and translate a relational "less than" */
	void RelLT()
	{
		A();
		SimpleExpression();	
		RelTypeCheckAndLoad();
		Gen("clt");

		Var* v = new Var;
		v->SetTypeId(VAR_SIMPLE);
		v->SetClassId(VAR_TEMP);
		v->SetSubTypeId(VAR_INT);
		mSStack.push(v);

	}
	
	/* recognize and translate a relational "greater than" */
	void RelGT()
	{
		A();
		SimpleExpression();
		RelTypeCheckAndLoad();
		Gen("cgt");

		Var* v = new Var;
		v->SetTypeId(VAR_SIMPLE);
		v->SetClassId(VAR_TEMP);
		v->SetSubTypeId(VAR_INT);
		mSStack.push(v);

	}	
	
	void RelGEQ()
	{
		string ltrue,lfalse;
		ltrue  = NewLabel();
		lfalse =  NewLabel();

		A();
		SimpleExpression();	
		RelTypeCheckAndLoad();
		Gen("blt.s  " + lfalse);
		Load(VAR_INT,1);
		Gen("br  " + ltrue);
		Gen(lfalse + ":");
		Load(VAR_INT,0);
		Gen(ltrue + ":");

		Var* v = new Var;
		v->SetTypeId(VAR_SIMPLE);
		v->SetClassId(VAR_TEMP);
		v->SetSubTypeId(VAR_INT);
		mSStack.push(v);

	}
    	
	void RelLEQ()
	{
		string ltrue,lfalse;
		ltrue  = NewLabel();
		lfalse =  NewLabel();

		A();
		SimpleExpression();	
		RelTypeCheckAndLoad();
		Gen("bgt.s  " + lfalse);
		Load(VAR_INT,1);
		Gen("br  " + ltrue);
		Gen(lfalse + ":");
		Load(VAR_INT,0);
		Gen(ltrue + ":");
		
		Var* v = new Var;
		v->SetTypeId(VAR_SIMPLE);
		v->SetClassId(VAR_TEMP);
		v->SetSubTypeId(VAR_INT);
		mSStack.push(v);

	}
	/*
	resolve type of top 2 stack elements and load them
	*/
	void RelTypeCheckAndLoad()
	{
		Var*v1,*v2;

		v2 = mSStack.top();
		mSStack.pop();

		v1 = mSStack.top();
		mSStack.pop();

		switch( CmpType(v1,v2) )
		{
		case 1:  // both int nothing req			
			break;
		case 2: // int real					
			break;
		case 3: // real int
			Gen("conv.r8");			
			break;
		case 4: // real,real			
			break;
		case 0:
			Err("Unhandles Type");
		}
		
	}
	
	void Call(Var* fv)
	{
		Var* r;
		Var* v;
		VarList* plist;
		
		if ( fv->GetTypeId() == VAR_PROCEDURE )
		{
			code = "call  void " +
					mClassName  + "::" +
					fv->GetName() + "(" ;

			plist = fv->GetParams();

			int size = plist->Length();

			for( int i =0 ; i < size ; i++)
			{
				if( i != 0)
					code += ",";
				v = plist->FindByIndex(i);
				
				if( v->GetTypeId() == VAR_ARRAY )
				{
					code += GetStringType(v->GetSubTypeId());				
					code += "[] " ;
				}
				else
					code += GetStringType(v->GetSubTypeId()) + " ";							
				
			}
			code += ")" ;
		}			
		
		else if( fv->GetTypeId() == VAR_FUNCTION )
		{
			r = fv->GetResultType();
			code = "call  ";
			code += GetStringType(r->GetSubTypeId());
			code +=  " " + mClassName  + "::" +
					fv->GetName() + "(" ;

			plist = fv->GetParams();

			int size = plist->Length();

			for( int i =0 ; i < size ; i++)
			{
				if( i != 0)
					code += ",";
				v = plist->FindByIndex(i);
				if( v->GetTypeId() == VAR_ARRAY )
				{
					code += GetStringType(v->GetSubTypeId());				
					code += "[] " ;
				}
				else
					code += GetStringType(v->GetSubTypeId()) + " ";				
			}
			code += ")" ;
		}
		Gen(code);
	}
	void LoadReserved(Var* fv,Var* v)
	{			
		if( fv->GetName() == "WRITE")
		{
			if( v->GetSubTypeId() == VAR_STRING )
				code = "call       void [mscorlib]System.Console::Write(string)";
			else if( v->GetSubTypeId() == VAR_INT )
				code = "call       void [mscorlib]System.Console::Write(int32)";
			else if( v->GetSubTypeId() == VAR_REAL )
				code = "call       void [mscorlib]System.Console::Write(float64)";
			else
				Err("Constant Type Unknown");
		}
		Gen(code);
	}
	bool IsReserved(Var* v)
	{
		return ( v->GetName() == "WRITE" );
	}
	Var* FindInST(Var* iv)
	{	
		Var* v =NULL;
		if( mScope == 1 )
		{
			v = mLocalST.FindByName(iv->GetName());				
			if( v == NULL )
			{
				v = mSTRoot.FindByName(iv->GetName());					
			}
		}
		else
			v = mSTRoot.FindByName(iv->GetName());			
		return v;
	}

	Var* FindInST(Token* t)
	{
		Var* v =NULL;
		if( mScope == 1 )
		{
			v = mLocalST.FindByName(t->Name);				
			if( v == NULL )
			{
				v = mSTRoot.FindByName(t->Name);					
			}
		}
		else
			v = mSTRoot.FindByName(t->Name);			

		return v;
	}
	
public:
	//////////////////////////////////////////////////
	void DumpSymenticStack()
	{
		stack<Var*> temp = mSStack;
		printf("*******************************\n"); 
		while( temp.size() > 0 )
		{
			printf("%s\n",temp.top()->ToString().c_str());
			temp.pop();
		}		
	}
	///////////////////////////////////////////////////
	string NewLabel()
	{
		char t[LIMIT];
		string l = "L";
		static int labelCount = 0;
		l += itoa(labelCount++,t,10);
		return l;
	}
	/*
	Returns 1 if int int
			2 if int real
			3 if real int
			4 if real real
			0 otherwise
	*/		
	int CmpType(Var* v1,Var* v2)
	{
		if( v1->GetSubTypeId() == VAR_INT && v2->GetSubTypeId() == VAR_INT )
			return 1;
		if( v1->GetSubTypeId() == VAR_INT && v2->GetSubTypeId() == VAR_REAL )
			return 2;
		if( v1->GetSubTypeId() == VAR_REAL && v2->GetSubTypeId() == VAR_INT )
			return 3;
		if( v1->GetSubTypeId() == VAR_REAL && v2->GetSubTypeId() == VAR_REAL )
			return 4;
		return 0;
	}
	/// Gen 
	void GenFuncDecl(Var* fv)
	{
		Var* r;
		Var* v;
		VarList* plist;
		code = ".method public hidebysig static " ;
		if( fv->GetTypeId() == VAR_FUNCTION) 
		{
			r = fv->GetResultType();
			code += GetStringType(r->GetSubTypeId());
			code += " ";
			code += fv->GetName();
			code += "(";

			plist = fv->GetParams();

			int size = plist->Length();

			for( int i =0 ; i < size ; i++)
			{
				if( i != 0)
					code += ",";
				v = plist->FindByIndex(i);
				
				code += GetStringType(v->GetSubTypeId());
				code += " ";
				code += v->GetName();				
			}

			code += ") cil managed";
		} 
		else if( fv->GetTypeId() == VAR_PROCEDURE )
		{			
			code += " void ";
			code += fv->GetName();
			code += "(";

			plist = fv->GetParams();

			int size = plist->Length();

			for( int i =0 ; i < size ; i++)
			{
				if( i != 0)
					code += ",";

				v = plist->FindByIndex(i);
				if ( v->GetTypeId() == VAR_SIMPLE)
				{					
					code += GetStringType(v->GetSubTypeId());
					code += " ";
					code += v->GetName();				
				}
				else if ( v->GetTypeId() == VAR_ARRAY)
				{					
					code += GetStringType(v->GetSubTypeId());
					code += "[] ";
					code += v->GetName();				
				}
				
			}

			code += ") cil managed";
		}
		Gen(code);
	}

	void LoadElement(Var* v)
	{
		code = "ldelem." ;
		if( v->GetSubTypeId() == VAR_INT )
			code += "i4";
		else if( v->GetSubTypeId() == VAR_REAL )
			code += "r8";
		else
			Err("Internal Compiler Error in ' LoadElement' no sub type assigned to array");
		Gen(code);
	}

	void Load(int type,string& name)
	{		
		if( type == VAR_INT )
		{
			code = "ldc.i4.s ";
			code += name;
		}	
		else if( type == VAR_REAL )
		{
			code = "ldc.r8.s ";
			code += name;
		}
		else
			Err("Load constant");
		
		Gen( code );					
	}

	void Load(int type,int c)
	{
		char t[LIMIT];
		if( type == VAR_INT )
		{
			code = "ldc.i4.s " ;
			code += itoa(c,t,10);
		}
		else if( type == VAR_REAL )
		{
			code = "ldc.r8.s " ;
			code += itoa(c,t,10);
		}
		else
			Err("Load constant");
		
		Gen( code );					
	}
	//// Loading 
	void Load(Var* v )
	{
		if ( AdjustStackType(v) == false )
			return;
		
		switch( v->GetClassId() )
		{
		case VAR_STATIC:
			if( v->GetTypeId() == VAR_SIMPLE )
			{
				
				Gen("ldsfld     " + GetStringType(v->GetSubTypeId() ) 
					+ " " + mClassName + "::" + v->GetName() );
			}
			else if( v->GetTypeId() == VAR_ARRAY )
			{				
				Gen("ldsfld     " + GetStringType(v->GetSubTypeId() ) 
					+ "[] " + mClassName + "::" + v->GetName() );
			}			

			break;
		case VAR_CONSTANT:
			switch( v->GetSubTypeId() )
			{
			case VAR_INT:
				code = "ldc.i4.s " + v->GetName();
				Gen( code );
				break;
			case VAR_REAL:
				code = "ldc.r8 " + v->GetName();
				Gen( code );
				break;
			case VAR_STRING:
				{
					string s = v->GetName();
					s = s.substr(1,s.length()-2);					

					code = "ldstr    \"" +  s + "\"";
					Gen( code );
					break;
				}
			}
			break;
		case VAR_TEMP: 	// Already Loaded			

			break;
		case VAR_LOCAL:		
			{
			 char t[LIMIT];
			 if( v->GetDeclIndex() == 0 )
				 code = string("ldloc.") + itoa(v->GetDeclIndex(),t,10);
			 else
				 code = string("ldloc.s\tV_") + itoa(v->GetDeclIndex(),t,10);

			 Gen(code);					
			}
		    break;
		case VAR_RETURN_VAL:
			{
			 char t[LIMIT];
			 if( v->GetDeclIndex() == 0 )
				 code = string(" ldloc.") + itoa(v->GetDeclIndex(),t,10);
			 else
				 code = string(" ldloc.s\tV_") + itoa(v->GetDeclIndex(),t,10);
			 Gen(code);					
			}	
			break;		
		case VAR_PARAM:
			{
				char t[LIMIT];
				code = string("ldarg.") + itoa(v->GetParamIndex(),t,10);
				Gen(code);
			}
		}
		// if negative then
		if( v->GetNeg() == true )
			Gen("neg");
		
	}
	bool AdjustStackType(Var* v)
	{
		if( v->GetTypeId() == VAR_FUNCTION     || 
		    v->GetTypeId() == VAR_PROCEDURE    
		  )
		return false; 

		if( v->GetSubTypeId() == VAR_STRING )
			return true;

		if( mSStack.empty() == false )
		{
			Var* sv = mSStack.top();
			if( sv->LValue() == true || sv->GetSubTypeId() == VAR_STRING )
				return true;

			if(!(sv->GetTypeId() == VAR_FUNCTION || 
				 sv->GetTypeId() == VAR_PROCEDURE 
				 )
			  )
			{			
				
				switch( CmpType(sv,v) )
				{
				case 1:  // both int nothing req
					break;
				case 2: // int real
					{
						// convert and push a temp on stack
						Gen("conv.r8"); 
						Var* v = new Var;
						v->SetClassId(sv->GetClassId());
						v->SetTypeId(VAR_TEMP);
						v->SetSubTypeId(VAR_REAL);						
						v->SetName(sv->GetName());
						mSStack.pop();
						mSStack.push(v);
					}
					break;
				case 3: // real int					
					break;
				case 4: // real,real
					break;
				case 0:
					Err("Unhandles Type");
				}	
			}
		}
		return true;
	}

	//// Stroring	
	void Store(Var* v,bool bsubscript = false)
	{
		char t[LIMIT];
		if( v->GetTypeId() == VAR_ARRAY )
		{
			if( bsubscript == true)
			{				
				if( v->GetSubTypeId() == VAR_INT )
					Gen("stelem.i4");
				else if( v->GetSubTypeId() == VAR_REAL )
					Gen("stelem.r8");
				else
					Err("Internal compiler error 'Store Array'");
				return;
			}
			else
			{
				switch( v->GetClassId() )
				{
				case VAR_STATIC: // class scope
					Gen("stsfld     " + GetStringType(v->GetSubTypeId() ) 
						+ "[] " + mClassName + "::" + v->GetName() );
					break;
				case VAR_LOCAL:
					Gen(string("stloc.") + itoa(v->GetDeclIndex(),t,10) );
					break;
				case VAR_PARAM:
					Gen(string("starg.s\t") + v->GetName());
					break;
				case VAR_RETURN_VAL:
					Gen(string("stloc.") + itoa(v->GetDeclIndex(),t,10) );
					break;		
				}
			}
			return;
		}
		switch( v->GetClassId() )
		{
		case VAR_STATIC: // class scope
			Gen("stsfld     " + GetStringType(v->GetSubTypeId() ) 
					+ " " + mClassName + "::" + v->GetName() );
			break;
		case VAR_LOCAL:
			Gen(string("stloc.") + itoa(v->GetDeclIndex(),t,10) );
			break;
		case VAR_PARAM:
			Gen(string("starg.s\t") + v->GetName());
			break;
		case VAR_RETURN_VAL:
			Gen(string("stloc.") + itoa(v->GetDeclIndex(),t,10) );
			break;		
		}
	}
	/////
	void EmitStaticFields()
	{
		int size = mSTRoot.Length();
		Var* v;

		// start from 2 because first entry is write
		for( int i = mSTRoot.ResCount() ;i < size ; i++)
		{ 
			v = mSTRoot.FindByIndex(i);

			if ( v->GetTypeId() == VAR_SIMPLE)
			{				
				Gen(".field private static " + 
					GetStringType(v->GetSubTypeId())  +
					" "  + v->GetName() 
					);
			}
			else if( v->GetTypeId() == VAR_ARRAY )
			{
				Gen(".field private static " + 
					GetStringType(v->GetSubTypeId())  +
					"[] "  + v->GetName() 
					);
			}
			else
				Err("Unknown Type");
		}
	}
	void EmitFields()
	{
		Var* v; 
		Var* retVar = NULL;
		char temp[LIMIT];
		int varcount = 0;
		code = ".locals init (";

		 int size = mLocalST.Length();

		 for( int i =0 ;i < size ; i++)
		 {			 
			 v = mLocalST.FindByIndex(i);
			 if(v->GetClassId() == VAR_LOCAL )
			 {
				  if( varcount != 0 )
					 code += ",";

				  if ( v->GetTypeId() == VAR_SIMPLE)
				  {				
				   code += GetStringType(v->GetSubTypeId()) + " V_" ;
				   code += itoa(varcount,temp,10) ;			 
				  }
				  else if( v->GetTypeId() == VAR_ARRAY )
				  {
				   code += GetStringType(v->GetSubTypeId()) + "[] V_" ;
				   code += itoa(varcount,temp,10) ;			 
				  }
				  varcount++;
			 }
			 /// If we have a VAR_RETURN_VAL then we are 
			 /// parsing declarations for functions 
			 else if( v->GetClassId() == VAR_RETURN_VAL)
			 {
				 retVar = v;
			 }			
		 }
		 if( retVar != NULL )
		 {
		 	retVar->SetDeclIndex(varcount);
			if( varcount != 0 )
				 code += ",";
				  				 
			 code += GetStringType(retVar->GetSubTypeId()) + " V_" ;
			 code += itoa(varcount,temp,10) ;			 
			 varcount++;			 			 
		 }
		 code += ")";
		 if( i > 0 )
			  Gen(code);
	}


	/////Utility Token FUnc
	int isEq (Token* tok,int c)
	{
		return ( tok->Class == c );
	}
	int isTok (int c)
	{
		return ( mpTok->Class == c );
	}
	int token()
	{
		return mpTok->Class;
	}
	SyntexBox(TokenStream& ts,ILGenerator& gen):mTStr(ts),mEmit(gen)
	{			
		mFpDmp = NULL;
	}
	///////////////////////////////////////////////////////
	int start()
	{
		mIndScope = 0;		
		mLastParamListSize = 0;
		
	    InitSStack(); // Add Res Func in ST

		A();
		Program();
		mEmit.Flush();
		return 1;
	}
	void A()
	{
		mpTok = mTStr.GetNextToken();
		if( mpTok != NULL)
		{
			temp =  mpTok->Name;
			//MakeTree(temp.c_str());
		}
	}
	// 1
	void Program()
	{
		switch( token() )
		{
		case T_program:
			mScope = 0; // Currently in class scope
			A();
			if( isTok(T_identifier) )
			{
				/*Class Head*/ 
				Gen(".assembly DotPascal{}");
				code = ".class " + mpTok->Name + "\n{" ;
				Gen(code);
				mClassName = mpTok->Name;
				mIndScope++;
				////

				A();
				if( isTok(T_semicolon) )
				{
					A();
					Declerations();
					EmitStaticFields();
					GenConstructor();
					GenResFunc();
					/*fclose(treeFp);
					exit(0);*/
					mScope = 1; // Now in Function scope					
					SubDecl();
					mScope = 0; // Now in class scope					

					/*Code For Main Head*/
					Gen(".method public hidebysig static void  Main() cil managed");
					Gen("{");
					Gen(".entrypoint\n");
					/////////////////
					CompoundStatement();
					/*Code For Main Tail*/					
					Gen(" ret");
					Gen("}");					
					/////////////////

					if( isTok(T_endmarker) )
					{
						mIndScope--;
						/*Class Tail*/
						Gen("}");
						A();
						if( isTok( T_eof ) )
						{
							printf("Ended Succesfully !!!!\n");
							if( mFpDmp != NULL )
								mSTRoot.Dump("Global Symbol Table",mFpDmp);

						}
						return;
					}
					else
					{
						Err("Unexpected End Of File");
					}
				}
				else
					Err(": Expected");
			}
			else
				Err("keyword 'program' expected");
			return;
		default: 
			Err();
			return;
		}		
	}
	// 2
	void IdentifierList()
	{		
		switch(token())
		{
		case T_identifier:
			{
				//// Push All identifiers on the Q
				Var* v = new Var;
				v->SetName(mpTok->Name);
				mParamList.push_back(v);
				
				// 1			
				A();
				IdentifierListTail();			
			}
			return;
		default:
			Err();
		}
	}
	// 3,4
	void IdentifierListTail()
	{		
		switch(token())
		{
		case T_comma:
			A();
			if( isTok(T_identifier) )
			{
				// 1
				//// Push All identifiers on the stack
				Var* v = new Var;
				v->SetName(mpTok->Name);
				mParamList.push_back(v);
	
				/////////////////////
				A();
				IdentifierListTail();
			}
			return ;
			
		case T_colon:			
			return ;
		case T_array:
			return;
		default:
			Err();
		}
	}
	// 5,6
	void Declerations()
	{		
		switch(token())
		{
		case T_var:
			VarDecl();
			return ;		
		case T_procedure:
		case T_function:
		case T_begin:
			return;
		default:
			Err();
			return ;
		}
	}
	// 7
	void VarDecl()
	{
		switch(token())
		{
		case T_var:
			A();
			DeclList(false);
			return;
		default:
			Err();
			return;
		}
	}
	// 8
	void DeclList(bool c) // c == true -> const
	{		
		Var* vt = new Var;
		Var* v; 
		switch( token() )
		{
		case T_identifier:
			IdentifierList();
			if( isTok( T_colon) )
			{
				A();
				Type(vt);
				//// Pop All The entries on the Param Q
				//// And assign them type and 
				////enter in current symbol
				//// Table

				int size = mParamList.size();
				for( int i = 0 ; i < size ;i++)
				{
					v = mParamList[i];
					v->SetScope(mScope); // Set Var Scope
					v->SetDeclIndex(i);					
					switch( vt->GetTypeId() )
					{
					case VAR_SIMPLE:
						v->SetTypeId(VAR_SIMPLE);
						v->SetSubTypeId(vt->GetSubTypeId());
						break;
					case VAR_ARRAY:
						v->SetTypeId(VAR_ARRAY);
						v->SetBoundList(vt->GetBoundList());
						v->SetSubTypeId(vt->GetSubTypeId());
						break;						
					}
					switch( mScope )
					{
					case 0: // if in class
						{
							//mSTRoot.Dump();
							Var* tv;
							tv = mSTRoot.FindByName( v->GetName() );
							if( tv != NULL )
							{
								code = "The program already contains a definition for '" +
									    v->GetName() + "'";
								Err(code.c_str());
							}
						}
						v->SetClassId(VAR_STATIC);
						mSTRoot.Add(v);
						break;
					case 1:  // if in a procedure or function
						{
							mLocalST.Dump();
							Var* tv;
							tv = mLocalST.FindByName( v->GetName() );
							if( tv != NULL )
							{
								code = "The function already contains a definition for '" +
									    v->GetName() + "'";
								Err(code.c_str());
							}
						}
						v->SetClassId(VAR_LOCAL);
						mLocalST.Add(v);
						break;
					}
					
				}				
				mParamList.clear();			

				if ( isTok(T_semicolon) )
				{
					A();
					DeclList(false);
					return;														
				}							
			}
			Err(" ':' Expected");
			break;
		case T_function:
		case T_procedure:
		case T_begin:
			return;
		default:
			Err();
		}
	}
	
	// 10,11
	void Type(Var* v)
	{
		switch(token())
		{
		case T_integer:
		case T_real:
			StandardType(v);
			v->SetTypeId(VAR_SIMPLE);
			return;
		case T_array:
			ArrayType(v);
			v->SetTypeId(VAR_ARRAY);
			return;
		default:
			Err("Unhandles Type");
		}
	}
	// 12,13
	void StandardType(Var* v)
	{		
		switch(token())
		{
		case T_integer:			
			v->SetSubTypeId(VAR_INT);
			A();
			return ;
		case T_real:			
			v->SetSubTypeId(VAR_REAL);
			A();
			return;
		default:
			Err("Unhandled Type");
		}
	}
	// 14
	void ArrayType(Var* v)
	{
		switch( token())
		{
		case T_array:
			A();
			v->SetTypeId(VAR_ARRAY);
			if( isTok(T_leftbracket) )
			{
				A();
				ArrayLimits(v);
				
				if( token() == T_rightbracket )
				{
					A();
					if( token() == T_of )
					{
						A();
						StandardType(v);
						return;
					}
					else
						Err("Expected 'of'");
				}
				else
					Err(" ']' Expected ");

				
			}				
			else
				Err("Expected '['");
			return;
		default:
			Err();
			
		}
	}
	// 14.1
	void ArrayLimits(Var* v)
	{
		int ub,lb;
		switch( token() )
		{
			
		case T_constant:
			if( isTok(T_constant) && mpTok->Value == CONST_integer )
			{
				lb = atoi( mpTok->Name.c_str() );
				A();
				if( isTok(T_doubledot) )
				{
					A();
					if( isTok(T_constant) && mpTok->Value == CONST_integer )
					{
						ub = atoi( mpTok->Name.c_str());
						v->AddBound(lb,ub);
						A();
						if( isTok(T_comma) )
						{
							A();
							ArrayLimits(v);
							return;
						}
						else if( isTok(T_rightbracket) )
							return;
						else return;
					}
					else
						Err("Expected Constant");
				}
				else
					Err("Expected '..'");
			}
			else 
				Err("Expected Constant");
			break;
		case T_rightbracket:
			return;
		default:
			Err();
		}
	}
	
	// 15,15b
	void SubDecl()
	{
		switch( token() )
		{
			
		case T_procedure:
		case T_function:
			SubProgramDecl();
			SubDecl();
			return;
		case T_begin:
			return;
		default:
			Err();
		}
	}
	// 16
	void SubProgramDecl()
	{
		Var* fv = NULL;
		switch(token())
		{
		case T_function:
		case T_procedure:
			SubHead(fv);		
			Gen("{");
			Declerations();
			EmitFields();
			GenLocalArrayInit();
			CompoundStatement();
			// if function then load return val
			if( fv->GetTypeId() ==  VAR_FUNCTION )
			{
				Load( mLocalST.FindByName( fv->GetName() ) );
			}
			///////
			Gen(" ret\n }");
			if( mFpDmp != NULL )
				mLocalST.Dump(fv->GetName().c_str(),mFpDmp);
			mLocalST.Clear();			
			return;
		default:
			Err();
			return;
		}
	}
	// 17,18
	void SubHead(Var*& argFv)
	{	
		VarList *vl = new VarList;
		switch( token() )
		{
		case T_function:
			A();
			if( isTok(T_identifier) )
			{
				////
				Var* fv = new Var;
				argFv = fv;
				fv->SetTypeId(VAR_FUNCTION);
				fv->SetName(mpTok->Name);			

				////////////////////////////////
				// Create a variable of the name of function and 
				// for return value and add that variable in the 
				// symbol table
				// its type will be set later whwn return type will
				// be seen
				////////////////////////////////
				Var* v = new Var;
				v->SetName(mpTok->Name);
				mLocalST.Add(v);
				/////////////////////////

								
				A();
				Arguments();

				//// Now parmQ must contain the param list
				//// Pop All The entries on the Param Q
				
				int size = mParamList.size();
				for( int i = 0 ; i < size ;i++)
				{
					Var* v = mParamList[i]; 
					vl->Add(v);
					v->SetClassId(VAR_PARAM);
					v->SetParamIndex(i);
					mLocalST.Add(v);
				}				
				fv->SetParams (vl);
				////////////////////////////////
				////Insert in Root ST
				mSTRoot.Add(fv);
				////////////////////////////////
				///Clear Param List
				mParamList.clear();
				mLastParamListSize = 0;

				if( isTok(T_colon) )
				{
					Var* rv = new Var;
					A();
					StandardType(rv);
					fv->SetResultType(rv);

					//// Now return type is seen so 
					//// set the return type of function return
					// variable

					Var* v = mLocalST.FindByName(fv->GetName());
					v->SetClassId(VAR_RETURN_VAL);
					v->SetTypeId(rv->GetTypeId());
					v->SetSubTypeId(rv->GetSubTypeId());
					//////////////////////////////
					

					if( isTok(T_semicolon) )
					{
						GenFuncDecl(fv);
						A();
						return;
					}
					else
						Err("Expected ';'");
				}
				else
					Err("Expected ':'");
			}
			else
				Err("Expected Identifier");			
			
			break;
		case T_procedure:
			A();
			if( isTok(T_identifier) )
			{
				////
				Var* fv = new Var;
				argFv = fv;
				fv->SetClassId(VAR_STATIC);
				fv->SetTypeId(VAR_PROCEDURE);
				fv->SetName(mpTok->Name);
				////

				A();
				Arguments();

				//// Now parmQ must contain the param list
				//// Pop All The entries on the Param Q
				
				int size = mParamList.size();
				for( int i = 0; i < size ;i++)
				{
					Var* v = mParamList[i];
					vl->Add(v);
					v->SetClassId(VAR_PARAM);
					v->SetParamIndex(i);
					mLocalST.Add(v);
				}
				fv->SetParams (vl);
				////Insert in Root ST
				mSTRoot.Add(fv);
				////////////////////////////////
				///Clear Param List
				mParamList.clear();
				mLastParamListSize = 0;

				if ( isTok(T_semicolon) )
				{
					GenFuncDecl(fv);
					A();
					return;
				}
			}
			Err();
			break;
		}
	}
	// 19,20
	void Arguments()
	{
		switch( token() )
		{
		case T_leftparen:
			A();
			ParamList();
			if ( isTok(T_rightparen) )
			{
				A();
				return;
			}
			Err();
			break;
		case T_colon:
		case T_semicolon:
			return;
		default:
			Err();
			return;
		}
	}
	// 21
	void ParamList()
	{		
		Var *type = new Var;
		switch( token() )
		{
		case T_identifier:
			IdentifierList();
			if( isTok(T_colon) )
			{
				A();
				Type(type);

				//// Travserse mParmList  And assign them type 
				int size = mParamList.size();
				for( int i = mLastParamListSize ; i < size ;i++)
				{
					Var* v = mParamList[i];
					v->SetTypeId(type->GetTypeId());
					v->SetSubTypeId(type->GetSubTypeId());
					v->SetBoundList(type->GetBoundList());
				}
				mLastParamListSize = size;

				ParamListTail();
				return;
			}
			Err();			
			break;
		case T_rightparen:
			return;
		default:
			Err();
		}
	}
	// 22,23
	void ParamListTail()
	{
		Var* type = new Var;
		switch( token() )
		{
		case T_semicolon:
			A();
			IdentifierList();
			if ( isTok(T_colon) )
			{
				A();
				Type(type);
				
				//// Travserse mParmList  And assign them type 
				int size = mParamList.size();
				for( int i = mLastParamListSize ; i < size ;i++)
				{
					Var* v;
					v = mParamList[i];
					v->SetTypeId(type->GetTypeId());
					v->SetSubTypeId(type->GetSubTypeId());									
					v->SetBoundList(type->GetBoundList());
				}
				mLastParamListSize = size;
				ParamListTail();
				return;
			}
			Err();
			break;
		case T_rightparen:
			return;
		default:
			Err();
		}
	}
	// 24
	void CompoundStatement()
	{
		//Increase Indent
		mIndScope++;
		switch( token() )
		{
		case T_begin:
			A();
			StatementList();
			if ( isTok(T_end) )
			{
				mIndScope--;
				A();
				return;
			}
			Err();
			break;
		default:
			Err();
		}
	}
	//27,27b
	void StatementList()
	{
		switch(token())
		{
		case T_identifier:
		case T_begin:
		case T_if:
		case T_case:
		case T_while:
		case T_for:
 			Statement();
			StatementList();
			return;
		case T_end:
			return;
		default:
			Err("Unknown Token " );
		}

	}
	// 30,31,32,33,34,35
	void Statement()
	{
		switch( token() )
		{
		case T_identifier:
			BasicStatement();
			if( isTok( T_semicolon ))
			{
				A();
				return;
			}
			break;
		case T_begin:
			CompoundStatement();
			return;
		case T_if:
			IfStatement();
			return;
		case T_case:
			CaseStatement();
			return;
		case T_while:
			WhileStatement();
			return;
		case T_for:
			ForStatement();
			return;
		case T_end:
			StatementList();
			return;
		default:
			Err();
		}
	}
	// 36
	void BasicStatement()
	{
		Var* v;
		switch( token() )
		{
		case T_identifier:			
			
			v = FindInST(mpTok);

			if( v == NULL )
			{
				code = "'" ;
				code +=  mpTok->Name + "' : undeclared identifier";
				Err( code.c_str() );
			}			
			// this is l value
			v->LValue(true);

			mSStack.push(v);			
			////

			A();
			BasicStatementTail(v);
			// Now this is not an L value any more
			v->LValue(false);
			DumpSymenticStack();


			/// After the finish of statement if there is any
			/// temporary left on the top of the stack 
			/// clean it
			if( mSStack.size() > 0 )
			{
				Var* v = mSStack.top();
				if( v->GetTypeId() == VAR_TEMP)
					Gen("pop");
			}		 
			
			
			return;
		default:
			Err();
		}
	}

	// 37a,37b
	void BasicStatementTail(Var* fv)
	{		
		int scount = 0;
		bool bsubscript = false;
		switch( token() )
		{
		case T_leftbracket:
		case T_assignop:

			if( isTok(T_leftbracket) )
			{
				if( fv->GetTypeId() == VAR_ARRAY )
				    Load(fv);
			}
			
			Subscript(bsubscript,scount);
			
			DumpSymenticStack();
						

			if( fv->GetTypeId() != VAR_SIMPLE && 
				fv->GetTypeId() != VAR_ARRAY  )
			{
				Err("Cant Assign To Procedure Vaiable");
			}

			////
			if( isTok(T_assignop) )
			{
				A();
				Expression();
				
				////  Stack Contains Two Variables on top
				Var* from;
				Var* to;				
				from = mSStack.top();
				mSStack.pop();

				to = mSStack.top();
				mSStack.pop();

				if( from->GetTypeId() == VAR_ARRAY &&
					to->GetTypeId() == VAR_ARRAY  
					)
				{
					if( from->GetDimension() != to->GetDimension() )
						Err("Dimension Mismatch");
				}
				else if( from->GetTypeId() == VAR_ARRAY  &&
					(to->GetTypeId() == VAR_SIMPLE       ||
					 to->IsTemp() == true			     || 
					 from->GetDimension() == scount) 	 &&
					 bsubscript == false 
				  )
				  Err("Cant Assign array variable to simple variable");
				
				else if( to->GetTypeId() == VAR_ARRAY   &&
					(from->GetTypeId() == VAR_SIMPLE    ||
					from->IsTemp() == true)				&&			  
					scount == 0 	 							
					)
					Err("Cant Assign simple variable to array variable");
				else
				{}
				
				switch( CmpType(to,from) )
				{
				case 1:  // both int nothing req					
					Store(to,bsubscript);				
					break;
				case 2: // int real
					Err("Cannot implicitly convert type 'float' to 'int'");						
					break;
				case 3: // real int

					if( from->GetTypeId() == VAR_ARRAY &&
						to->GetTypeId() == VAR_ARRAY )
					{
						Err("Cannot implicitly convert type 'int[]' to 'double[]'");					
					}					

					Gen("conv.r8");
					Store(to,bsubscript);
					break;
				case 4: // real,real															
					Store(to,bsubscript);									
					break;
				case 0:
					Err("Unhandles Type");
				}
				
				return;
			}
			Err("Expected '='");
			break;
		case T_leftparen:
			{
				////
				Var* v;
				v = mSStack.top();
				mSStack.pop();
				
				if( v->GetTypeId() == VAR_RETURN_VAL)
				{					
									
					int scope = mScope;
					mScope = 0;
					v = FindInST(v);
					mScope = scope;				
				}

				Parameters(v);

				DumpSymenticStack();

				if( v->GetTypeId() != VAR_FUNCTION &&
					v->GetTypeId() != VAR_PROCEDURE
				  )
				  Err("Function or Procedure required");

				if( IsReserved(v) == true )
					return;

				Call(v);

				if( v->GetTypeId() == VAR_FUNCTION )
				{
					
					Var* v1 = new Var;	
					Var* v2 = v->GetResultType();
					v1->SetTypeId(VAR_TEMP);
					v1->SetSubTypeId( v2->GetSubTypeId() );
					mSStack.push(v1);
				}
			}			
			return;
		case T_semicolon:
			{
				Var* v;
				char t[LIMIT];
				v = mSStack.top();
				mSStack.pop();
				
				if( v->GetTypeId() != VAR_PROCEDURE )
					Err("Procedure required");
				
				VarList* vl = v->GetParams();
				if( vl->Length() != 0 )
				{
					code = v->GetName();
					code += " take ";
					code += itoa(vl->Length(),t,10);
					code += " parameter(s)";
					Err(code.c_str());
				}
				Call(v);
				return;
			}
		default:
			Err();
			break;
		}
	}
	// 38,39	
	void Subscript(bool& bsubscript,int& count)
	{
		Var* v;
		switch( token() )
		{
		case T_leftbracket:
			bsubscript = true;			
			/// New Function Call
			mParamCountStack.push(0);
			/////////////////////////
			
			////
			v = mSStack.top();
			
			if( v->GetTypeId() != VAR_ARRAY )
				Err("subscript requires array");

			////
			A();
			ExpressionList(v);
			DumpSymenticStack();
			if ( isTok( T_rightbracket) )
			{
				A();	
				mSStack.pop();
				
				/// Array Dimension check
				int p = mParamCountStack.top();
				mParamCountStack.pop();

				count = p;
				
				if( v->GetDimension() != p)
				{
					char t[LIMIT];
					code = "Type Mismatch: " ;
					code += v->GetName();
					code += " has dimesion " ;
					code += itoa(v->GetDimension(),t,10);
					Err(code.c_str());
				}
				/////////////////////////			
				return;
			}
			Err("Expected ':'");
			break;
		case T_assignop:
		case T_mulop:
		case T_addop:
		case T_relop:
		case T_leftparen:
		case T_semicolon:
		case T_comma:
			return;
		default:
			Err();
		}
	}
	// 40,41
	void Parameters(Var* fv)
	{
		switch( token() )
		{
		case T_leftparen:
			A();
			/// New Function Call
			mParamCountStack.push(0);
			/////////////////////////
			ExpressionList(fv);
			if( isTok( T_rightparen ) )
			{
				A();
				if ( IsReserved(fv) == true)
				{
					mParamCountStack.pop() ;
					return;
				}
			
				//Function call finished clean no of param
				VarList* pl = fv->GetParams();
				if ( pl->Length() != mParamCountStack.top() )
				{					
					code = "Wrong Num Of Arguments in call to '" + fv->GetName() + "'";
					Err(code.c_str());
				}
				mParamCountStack.pop() ;
				////
				return;
			}
			Err();
			break;
		case T_semicolon:
			return;
		default:
			Err();
			break;
		}
	}
	// 42
	void ExpressionList(Var* fv)
	{
		switch( token() )
		{
		case T_addop:
		case T_identifier:
		case T_constant:
		case T_leftbracket:
		case T_not:
			Expression();
			{
				//// Inc the no of param on stack
				int p = mParamCountStack.top();
				mParamCountStack.pop();

				if( fv->GetTypeId() == VAR_ARRAY )
				{
					Var* v = mSStack.top();				
					mSStack.pop();
					
					if( v->GetSubTypeId() == VAR_INT )
					{
						Bounds b;						
						b = fv->GetBound(p);
						Load(VAR_INT,b.lb);
						Gen("sub");
						
						Var* v = new Var;
						v->SetTypeId(VAR_TEMP);
						v->SetSubTypeId(VAR_INT);
						mSStack.push(v);

						p++;
						mParamCountStack.push(p);
						////
						
						ExpressionListTail(fv);
						return;
					}
					else
					{
						Err("Array Index Must be of type integer");
						return;
					}
				}
			

				if (IsReserved(fv) == true )
				{
					Var* v = mSStack.top();				
					mSStack.pop();
					LoadReserved(fv,v);
					p++;
					mParamCountStack.push(p);
					ExpressionListTail(fv);
					return;
				}
				
				VarList* vl = fv->GetParams();
				Var* pv= vl->FindByIndex(p);
				Var* v = mSStack.top();				
				mSStack.pop();

				switch( CmpType(v,pv) )
				{
				case 1:  // both int nothing req					
					break;
				case 2: // int real										
					Gen("conv.r8");
					break;
				case 3: // real int
					Err("Type Mismatch : Function or procedure expects int");
					break;
				case 4: // real,real					
					break;
				case 0:
					Err("Unhandles Type");
				}				
				p++;
				mParamCountStack.push(p);
				////
			}			
			ExpressionListTail(fv);
			return;
		case T_rightparen:
			return;
		default:
			Err();
		}
	}
	// 43,44
	void ExpressionListTail(Var* fv)
	{
		switch( token() )
		{
		case T_comma:
			A();
			
			if( fv->GetTypeId() == VAR_ARRAY )
			{
				int p = mParamCountStack.top();
				
				Bounds b;						
				b = fv->GetBound(p);
				
				int n;
				n = b.ub - b.lb + 1;
				Load(VAR_INT,n);
				Gen("mul");											
			}
			
			Expression();
			{
				//// Inc the no of param on stack
				int p = mParamCountStack.top();
				mParamCountStack.pop();
				
				if( fv->GetTypeId() == VAR_ARRAY )
				{
					Var* v = mSStack.top();				
					mSStack.pop();
					
					if( v->GetSubTypeId() == VAR_INT )
					{
						Gen("add");
						Bounds b;						
						b = fv->GetBound(p);
						Load(VAR_INT,b.lb);
						Gen("sub");
						
						Var* v = mSStack.top();//new Var;
						v->SetTypeId(VAR_TEMP);
						v->SetSubTypeId(VAR_INT);
						//mSStack.push(v);
				
						p++;
						mParamCountStack.push(p);
						////
						ExpressionListTail(fv);
						return;
					}
					else
					{
						Err("Array Index Must be of type integer");
						return;
					}
				}
			
				if (IsReserved(fv) == true )
				{
					Var* v = mSStack.top();				
					mSStack.pop();
					LoadReserved(fv,v);
					p++;
					mParamCountStack.push(p);
					ExpressionListTail(fv);
					return;
				}
				
				VarList* vl = fv->GetParams();
				int pcount = vl->Length();

				if( p + 1 > pcount )
				{
					char t[LIMIT];
					code = "'" + fv->GetName() + "' takes only " +
						   itoa(pcount,t,10) + " arguments "; 
					Err(code.c_str());
				}
				Var* pv= vl->FindByIndex(p);
				Var* v = mSStack.top();
				mSStack.pop();

				switch( CmpType(v,pv) )
				{
				case 1:  // both int nothing req
					break;
				case 2: // int real					
					Gen("conv.r8");					
					break;
				case 3: // real int
					Err("Cant Assign Real Int Required");
					break;
				case 4: // real,real
					break;
				case 0:
					Err("Unhandles Type");
				}		
				
				p++;
				mParamCountStack.push(p);
				////			
			}

			ExpressionListTail(fv);
			return;
		case T_rightparen:
		case T_rightbracket:
			return;
		default:
			code = "Misplasec token '" + temp + "'";
			Err(code.c_str());
			return;
		}
	}
	// 45
	void Expression()
	{
		switch( token() )
		{
		case T_addop:		
		case T_identifier:
		case T_constant:
		case T_leftparen:
		case T_not:
			SimpleExpression();
			ExpressionTail();
			return;			
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:
		case T_of:
			return;			
		default:
			Err();
		}
	}
	// 46,47
	void ExpressionTail()
	{
		switch( token() )
		{
		case T_relop:
			{
				switch( mpTok->Value )
				{
				case OP_le:
					RelLEQ();
					break;
				case OP_ne:
					RelNEQ();
					break;
				case OP_lt:
					RelLT();
					break;
				case OP_eq:
					RelEQ();
					break;
				case OP_ge:
					RelGEQ();
					break;
				case OP_gt:
					RelGT();
					break;
					
				}
			}
			return;

		case T_rightbracket:
		case T_semicolon:
		case T_comma:
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
			return;
		default:
			Err();
			break;
		}
	}
	// 48,49
	void SimpleExpression()
	{
		int sign = -1;
		switch( token() )
		{
		case T_identifier:
		case T_constant:
		case T_leftparen:
		case T_not:
			Term();
			SimpleExpressionTail();
			return;
		case T_addop:
			Sign(sign);			
			Term();
			if( sign == T_unaryminus || sign == T_unaryplus )
			{
				Var* v = mSStack.top();				
				
				v->SetNeg(!v->GetNeg());			
			}
			else
			{
				Var* v1;
				Var* v2;
				Var* v = new Var;  // The result of op
				v->SetTypeId(VAR_TEMP);
				
				DumpSymenticStack();
				
				v2 = mSStack.top();
				mSStack.pop();
				
				v1 = mSStack.top();
				mSStack.pop();
				
				
				switch( CmpType(v1,v2) )
				{
				case 1:  // both int nothing req
					v->SetSubTypeId(VAR_INT);
					break;
				case 2: // int real	
					v->SetSubTypeId(VAR_REAL);
					break;
				case 3: // real int
					Gen("conv.r8");
					v->SetSubTypeId(VAR_REAL);
					break;
				case 4: // real,real
					v->SetSubTypeId(VAR_REAL);
					break;
				case 0:
					Err("Unhandles Type");
				}
				Gen(GetStringOp( sign ) );
				mSStack.push(v);
			}
			SimpleExpressionTail();
			return;
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
		case T_comma:
		case T_semicolon:
		case T_rightbracket:
		case T_relop:
			
			return;
		default:
			Err();
		}
	}
	// 50,51
	void SimpleExpressionTail()
	{
		Token* opTok;
		switch( token() )
		{
		case T_addop:			
			opTok = mpTok;
			A();
			Term();
			{
				Var* v1;
				Var* v2;
				Var* v = new Var;  // The result of op
				v->SetTypeId(VAR_TEMP);
				
				DumpSymenticStack();
				
				v2 = mSStack.top();
				mSStack.pop();
				
				v1 = mSStack.top();
				mSStack.pop();

				if( v1->GetTypeId() == VAR_ARRAY &&
					v2->GetTypeId() == VAR_ARRAY )
				{
					code = "Operator " + opTok->Name + " cannot be applied to operands of type []";					 
					Err(code.c_str());
				}					

				
				
				switch( CmpType(v1,v2) )
				{
				case 1:  // both int nothing req
					v->SetSubTypeId(VAR_INT);
					break;
				case 2: // int real					
					break;
				case 3: // real int
					Gen("conv.r8");
					v->SetSubTypeId(VAR_REAL);
					break;
				case 4: // real,real
					v->SetSubTypeId(VAR_REAL);
					break;
				case 0:
					Err("Unhandles Type");
				}
				Gen(GetStringOp(opTok->Value));
				mSStack.push(v);
			}
			
			SimpleExpression();
			return;
			
		case T_relop:
		case T_rightbracket:
		case T_semicolon:
		case T_comma:
		case T_rightparen:
		case T_then:		
		case T_of:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
			return;
		default:
			Err();
		}
	}
	// 52
	void Term()
	{
		switch( token() )
		{
		case T_identifier:
		case T_constant:
		case T_leftparen:
		case T_not:
			Factor();
			TermTail();
			return;
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
			return;
		default:
			Err();	
		}
	}
	// 53,54
	void TermTail()
	{
		switch( token() )
		{
		case T_mulop:
			{
				Token* opTok;

				opTok = mpTok; // Save the current op
								
				A();
				Factor();
				{
					Var* v1;
					Var* v2;
					Var* v = new Var;  // The result of op
					v->SetTypeId(VAR_TEMP);
					
					DumpSymenticStack();

					v2 = mSStack.top();
					mSStack.pop();
					
					v1 = mSStack.top();
					mSStack.pop();

					if( v1->GetTypeId() == VAR_ARRAY &&
						v2->GetTypeId() == VAR_ARRAY )
					{
						code = "Operator " + opTok->Name + " cannot be applied to operands of type []";					 
						Err(code.c_str());
					}					

					
				switch( CmpType(v1,v2) )
				{
				case 1:  // both int nothing req
					v->SetSubTypeId(VAR_INT);
					break;
				case 2: // int real					
					break;
				case 3: // real int
					Gen("conv.r8");
					v->SetSubTypeId(VAR_REAL);
					break;
				case 4: // real,real
					v->SetSubTypeId(VAR_REAL);
					break;
				case 0:
					Err("Unhandles Type");
				}
					Gen(GetStringOp(opTok->Value));
					mSStack.push(v);
					// temp on stack
				}
			}
			
			TermTail();
			return;

		case T_addop:
		case T_relop:
		case T_rightbracket:
		case T_semicolon:
		case T_comma:
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
			return;
		default:
			Err("Unexpected Token ");
		}
	}
	// 55,56 ,57,58
	void Factor()
	{
		switch( token() )
		{
		case T_identifier:
			{
				////
				Var* v;
				v = FindInST(mpTok);
				if( v == NULL )
				{
					code = "'" ;
					code +=  mpTok->Name + "' : undeclared identifier";
					Err( code.c_str() );
				}			

				if( v->GetClassId() == VAR_RETURN_VAL )
				{					
					int scope = mScope;
					mScope = 0;
					v = FindInST(v);
					mScope = scope;					
				}
			
				
				Load(v);
				mSStack.push(v);
				DumpSymenticStack();
				////				
			}
			A();
			FactorTail();
			return;

		case T_constant:
			{
				Var*  v = new Var;
				v->SetTypeId(VAR_CONSTANT);
				v->SetClassId(VAR_CONSTANT);
				switch( mpTok->Value )
				{
				case CONST_integer:
					v->SetSubTypeId(VAR_INT);
					break;
				case CONST_real:
					v->SetSubTypeId(VAR_REAL);
					break;
				case CONST_string:
					v->SetSubTypeId(VAR_STRING);
					break;				
				default:
					Err("Illegal Constant");
				}
				v->SetName(mpTok->Name);
				Load(v);
				mSStack.push(v);
				
			}

			A();
			return;
		case T_leftparen:
			A();
			Expression();
			if( isTok( T_rightparen ) )
			{
				A();
				DumpSymenticStack();
				return;
			}
			Err("Expected ')'");
			break;
		case T_not:
			{
				Var* v;
				v = mSStack.top();

				if( v->GetTypeId() == VAR_CONSTANT ||
					v->GetTypeId() == VAR_SIMPLE  ||
					v->GetTypeId() == VAR_ARRAY )
				{
					if( v->GetSubTypeId() != VAR_INT )
						Err("Operator '~' can only be applied to operand of type 'int'");
				}
				Gen("not");
			}
			A();
			Factor();
			return;
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
			return;
		default:
			Err();
		}
	}
	// 59,60
	void FactorTail()
	{
		switch( token() )
		{
		case T_leftparen:
			{
				Var* fv;
				fv = mSStack.top();
				mSStack.pop();

						
				if( fv->GetTypeId() != VAR_FUNCTION )
					Err("Function required");				
			
				ActualParameters(fv);			
							
				Call(fv);
//				mSStack.pop();
				
				Var* v = new Var;	
				Var* v1 = fv->GetResultType();
				v->SetTypeId(VAR_TEMP);
				v->SetSubTypeId( v1->GetSubTypeId() );
				mSStack.push(v);

			}
			return;
		case T_leftbracket:
			{
				Var* v;
				
				v = mSStack.top();
				mSStack.pop();

				/// New ARray Call
				mParamCountStack.push(0);
				/////////////////////////
			

				if( v->GetTypeId() != VAR_ARRAY )
					Err("subscript requires array");
				
				////
				A();
				ExpressionList(v);
				DumpSymenticStack();
				if ( isTok( T_rightbracket) )
				{
					A();
					LoadElement(v);

					// pop index from stack
					Var* vt;
					vt = mSStack.top();
					mSStack.pop();

					// push temp value of the element
					vt = new Var;
					vt->SetClassId(VAR_TEMP);
					vt->SetTypeId(VAR_TEMP);
					vt->SetSubTypeId(v->GetSubTypeId());
					mSStack.push(vt);

					/// Array Dimension check
					int p = mParamCountStack.top();
					mParamCountStack.pop();
					
					if( v->GetDimension() != p)
					{
						char t[LIMIT];
						code = "Type Mismatch: " ;
						code += v->GetName();
						code += " has dimesion " ;
						code += itoa(v->GetDimension(),t,10);
						Err(code.c_str());
					}
					/////////////////////////			
					return;				
				}
				Err();
			}
			break;			
		
		///// GHAPLA HAY SAB GHAPLA HA ////
		case T_mulop:
		case T_addop:
		case T_relop:
		case T_rightbracket:
		case T_semicolon:
		case T_comma:
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
			{
				Var* v = mSStack.top();
				if( v->GetTypeId() == VAR_FUNCTION)
				{
					mSStack.pop();
					v = FindInST(v);
					Load(v);
					mSStack.push(v);
				}
				return;
				///// ///////////////////////////////	
			}
		default:
			Err();
		}
	}
	// 61,62
	void ActualParameters(Var* fv)
	{
		switch( token() )
		{
		case T_leftparen:
			A();
			
			/// New Function Call
			mParamCountStack.push(0);
			/////////////////////////
			
			ExpressionList(fv);
			if ( isTok(T_rightparen ) )
			{
				A();
				if ( IsReserved(fv) == true)
				{
					mParamCountStack.pop() ;
					return;
				}
			
				//Function call finished clean no of param
				VarList* pl = fv->GetParams();
				if ( pl->Length() != mParamCountStack.top() )
				{					
					code = "Wrong Num Of Arguments in call to '" + fv->GetName() + "'";
					Err(code.c_str());
				}
				mParamCountStack.pop();
				return;
			}
			Err();
			break;
		///// GHAPLA HAY SAB GHAPLA HA ////
		case T_mulop:
		case T_addop:
		case T_relop:
		case T_rightbracket:
		case T_semicolon:
		case T_comma:
		case T_rightparen:
		case T_then:
		case T_do:
		case T_step:
		case T_to:
		case T_downto:		
		case T_of:
			return;
		///////////////////////////////////////
		default:
			Err();
			return;
		}
	}
	// 63,64
	void Sign(int& sign)
	{
		switch( token() )
		{
		case T_addop:
			sign = mpTok->Value;
			A();
			return;
		default:
			Err();			
		}
	}
	// 65
	void IfStatement()
	{			
		string lfalse = NewLabel();
		string lendif = NewLabel();

		switch( token() )
		{
		case T_if:
			A();
			GenComment("Start IF");
			Expression();

			mEmit.Flush();
			
			code = "brfalse\t" ;
			code += lfalse;
			Gen(code);

			if( isTok(T_then) )
			{
				GenComment("True");
				A();
				Statement();
		
				code = "br\t" ;
				code += lendif;
				Gen(code);

				Gen(lfalse + ":");
				GenComment("ELSE");
				ElseClause();
				Gen(lendif + ":");
				GenComment("Enf IF");
				return;
			}
			else
				Err("Missing 'then'");
			return;
		default:
			Err();
			return;

		}
	}
	// 66,67
	void ElseClause()
	{
		switch( token() )
		{
		case T_else:
			A();
			Statement();
			return;
		case T_identifier:		
		case T_begin:
		case T_if:		
		case T_case:			
		case T_while:					
		case T_for:			
		case T_end:
			return;
		default:
			Err();
		}
	}
	// 68
	void WhileStatement()
	{
		string lstartwhile = NewLabel();
		string lendwhile = NewLabel();

		switch( token() )
		{
		case T_while:
			A();
			
			GenComment("WHILE START HERE");
			Gen(lstartwhile + ":");

			Expression();
			GenComment("IF STACK TOP CONTAINS FALSE JUMP END WHILE");
			Gen("brfalse\t" + lendwhile);
			
			if ( isTok( T_do) )
			{
				A();
				Statement();
				GenComment("GO TO START WHILE AND CHECK CONDITION AGAIN");
				Gen("br\t" + lstartwhile);

				GenComment("WHILE END HERE");
				Gen(lendwhile + ":");			

				return;
			}
			else
				Err("Missing 'do'");
			break;
		default:
			Err();
		}
	}
	// 69
	void ForStatement()
	{
		Var* v;
		switch( token() )
		{
		case T_for:			
			A();
			GenComment("FOR START");
			if ( isTok( T_identifier) )
			{
				////
				v = FindInST(mpTok);
				
				if( v == NULL )
				{
					code = "'" ;
					code +=  mpTok->Name + "' : undeclared identifier";
					Err( code.c_str() );
				}			
				// Push 				
				mSStack.push(v);				
				////
				
				A();
				if ( isTok( T_assignop ) )
				{
					////
					v = mSStack.top();
					
					if( v->GetTypeId() != VAR_SIMPLE && 
						v->GetTypeId() != VAR_ARRAY  )
					{
						Err("Cant Assign To Function or Procedure Vaiable");
					}
					
					A();
					GenComment("INITIALIZING LOOP VARIABLE");
					Expression();					

					////  Stack Contains Two Variables on top
					Var* from;
					Var* to;				
					from = mSStack.top();
					mSStack.pop();
					
					to = mSStack.top();
					mSStack.pop();
					
					switch( CmpType(to,from) )
					{
					case 1:  // both int nothing req						
						Store(to);				
						break;
					case 2: // int real
						Err("Cannot implicitly convert type 'float' to 'int'");						
						break;
					case 3: // real int						
						Gen("conv.r8");
						Store(to);
						break;
					case 4: // real,real																
						Store(to);									
						break;
					case 0:
						Err("Unhandles Type");
					}					

					GenComment("LOOP VARIABLE INITIALIZED");

					ForTail(v);
					
					GenComment("FOR LOOP END HERE SHUKAR ALLAH KA");
					return;
				}
			}
		}
	}
	// 70,71
	void ForTail(Var* v)
	{
		string lsfor = NewLabel();
		string lefor = NewLabel();
		bool step = false;

		switch( token() )
		{
		case T_to:
			{
				A();
				
				GenComment("LOOP CONDITION YAHAN HAY");
				Gen(lsfor + ":");
				
				Load(v);
				Expression();
				
				string ltrue,lfalse;
				ltrue  = NewLabel();
				lfalse =  NewLabel();			
				
				Gen("bgt.s  " + lfalse);
				Load(VAR_INT,1);
				Gen("br  " + ltrue);
				Gen(lfalse + ":");
				Load(VAR_INT,0);
				Gen(ltrue + ":");
				
				GenComment("IF TRUE THEN CONTINUE ELSE JUMP END FOR");
				Gen("brfalse\t" + lefor);
				
				if( isTok(T_step) )
				{
					A();
					DumpSymenticStack();
					step = true;
					GenComment("This is Step Expression");
					Expression();
					Var* v;
					v = mSStack.top();
					if(  v->IsTemp() )
						Gen("dup"); 
					GenComment("Step Expression End");
					DumpSymenticStack();
				}
				if( isTok(T_do) )
				{
					A();
					Statement();
					GenComment("INC LOOP VARIABLE");
					DumpSymenticStack();
					Load(v);
					if( step == false)
						Load(VAR_INT,1);
					Gen("add");				
					Store(v);
					
					GenComment("JUMP TO START FOR CONDITION");
					Gen("br\t" + lsfor);
					GenComment("FOR FINISHED FOR GOOD");
					Gen(lefor + ":" );
					return;
				}
				else
					Err("Missing 'do'");
				step = false;
				return;
			}
		case T_downto:
			{
				
				A();
				
				GenComment("LOOP CONDITION YAHAN HAY");
				Gen(lsfor + ":");
				
				Load(v);
				Expression();
				
				string ltrue,lfalse;
				ltrue  = NewLabel();
				lfalse =  NewLabel();
				
				Gen("blt.s  " + lfalse);
				Load(VAR_INT,1);
				Gen("br  " + ltrue);
				Gen(lfalse + ":");
				Load(VAR_INT,0);
				Gen(ltrue + ":");
				
				
				GenComment("IF TRUE THEN CONTINUE ELSE JUMP END FOR");
				Gen("brfalse\t" + lefor);
				
				if( isTok(T_step) )
				{
					A();
					DumpSymenticStack();
					step = true;
					GenComment("This is Step Expression");
					Expression();				
					Gen("neg");
					Var* v;
					v = mSStack.top();
					if(  v->IsTemp() )
						Gen("dup"); 					
					
					GenComment("Step Expression End");
					DumpSymenticStack();
				}
				if( isTok(T_do) )
				{
					A();
					Statement();
					GenComment("INC LOOP VARIABLE");
					DumpSymenticStack();
					Load(v);
					
					if( step == false)
					{
						Load(VAR_INT,1);
						Gen("sub");
					}
					else
						Gen("add");
					
					Store(v);
					
					GenComment("JUMP TO START FOR CONDITION");
					Gen("br\t" + lsfor);
					GenComment("FOR FINISHED FOR GOOD");
					Gen(lefor + ":" );
					return;
				}
				else
					Err("Missing 'do'");
				step = false;
				return;
			}
		default:
			Err();
			return;
			}
	}
	struct CaseData
	{		
		string label;
		list<Token*> val;
	};
	bool IsUniqueLabel(Token* intok,list<CaseData*>& caseList)
	{
		int ncases = caseList.size();
		list<CaseData*>::iterator _cditer;
		list<Token*>::iterator _valiter;
		CaseData* cd;			
		Token* tok;
		for(_cditer = caseList.begin();_cditer != caseList.end(); _cditer++)
		{
			cd = *_cditer;				
			
			for(_valiter = cd->val.begin() ; _valiter != cd->val.end();
			    _valiter++)
			{
				tok = *_valiter;
				if( tok->Name == intok->Name )
					return false;
			}
				
		}
		return true;		
	}
	// 72
	void CaseStatement()
	{
		string nextlabel;
		string testlabel;
		list<CaseData*> caseList;
		switch( token()) 
		{
		case T_case:
			GenComment("switch begin");
			testlabel = NewLabel();
			nextlabel = NewLabel();			
			A();
			DumpSymenticStack();
			Expression();
			Gen("br " + testlabel);
			
			Var* v ;
			v = mSStack.top();
			
			if(  v->IsTemp() )
				Gen("dup"); 					
			
			DumpSymenticStack();
			if ( isTok( T_of ) )
			{
				A();
				CaseBranch(caseList,nextlabel);
				CaseBranchTail(caseList,nextlabel);
				if( isTok( T_end ) )
				{
					A();
					DumpSymenticStack();
					GenComment("test cases begin here");
					Gen(testlabel + ":");
					int ncases = caseList.size();
					CaseData* cd;
					Token* tok;
					bool bdefault = false;
					CaseData* defaultData;

					for(int i =0 ;i < ncases ; i++ )
					{
						cd = caseList.front();
						caseList.pop_front();
						
						int nconst = cd->val.size();
						for(int j = 0 ; j < nconst ; j++)
						{
							tok = cd->val.front(); 
							cd->val.pop_front();

							if( tok->Class == T_default )
							{	
								if( bdefault == true )
								{
									mpTok = tok;
									Err("The label 'default:' already occurs in this case statement");
									return;
								}
								bdefault = true;
								defaultData = cd;
								continue;
							}
							
							Gen("dup");

							if( IsUniqueLabel(tok,caseList) == false )
							{
								code = "The label '";
								code += tok->Name + "' already occurs in this case statement";
								mpTok = tok;
								Err(code.c_str());
							}
							
							if( tok->Value == CONST_integer && 
								v->GetSubTypeId() == VAR_INT )
							    Load(VAR_INT,tok->Name);
							else if( tok->Value == CONST_integer && 
								v->GetSubTypeId() == VAR_REAL )
							{
								Load(VAR_INT,tok->Name);
								Gen("convr.r8");
							}
							else if( tok->Value == CONST_real && 
								v->GetSubTypeId() == VAR_INT )
							    Err("Cant compare int with real");

							else if( tok->Value == CONST_real && 
								v->GetSubTypeId() == VAR_REAL )
							    Load(VAR_REAL,tok->Name);

							Gen("ceq ");
							Gen("brtrue  " + cd->label);							
						}
					}
					if( bdefault == true )
						Gen("br  " + defaultData->label);

					Gen(nextlabel + ":");
					Gen("pop");
					GenComment("switch end");
					return;
				}
				else
					Err("Keyword 'end' Expected");
			}
			Err("Keyword 'of' Expected");
			break;
		default:
			Err();

		}
	}
	// 73
	void CaseBranch(list<CaseData*>& caseList,string& nextlabel)
	{
		string clabel;		
		
		switch( token() )
		{
		case T_constant:
			{				
				////
				CaseData* cd = new CaseData;
				caseList.push_back(cd);
				clabel = NewLabel();
				Gen(clabel + ":");
				
				CaseLabel(cd,clabel);
				if( isTok(T_colon))
				{
					A();
					Statement();
					Gen("br " + nextlabel);
					
					return;
				}
				Err();
				break;
			}
		case T_default:
			{
				////
				CaseData* cd = new CaseData;
				caseList.push_back(cd);
				clabel = NewLabel();
				Gen(clabel + ":");
				
				CaseLabel(cd,clabel);
				if( isTok(T_colon))
				{
					A();
					Statement();
					Gen("br " + nextlabel);
					
					return;
				}
				Err();
				break;
			}

		case T_end:
			return;
		default:
			Err();		
			}
		}
		// 74
		void CaseLabel(CaseData* cd,string& label)
		{
			switch( token() )
			{
			case T_constant:
				cd->label = label;
				cd->val.push_back(mpTok);			
				A();
				CaseLabelTail(cd);
				return;
			case T_default:
				cd->label = label;
				cd->val.push_back(mpTok);			
				A();
				CaseLabelTail(cd);
				return;				
			default:
				Err();
			}
		}
	// 75
	void CaseLabelTail(CaseData* cd)
	{
		switch( token() )
		{
		case T_comma:
			A();
			if( isTok( T_constant) )
			{
				cd->val.push_back(mpTok);
				A();
				CaseLabelTail(cd);
				return;
			}
			Err();
			break;
		case T_colon:
			return;
		default:
			Err();
		}
	}
	// 76
	void CaseBranchTail(list<CaseData*>& caseList,string& nextlabel)
	{
		switch( token() )
		{			
		case T_constant:
		case T_default:
			CaseBranch(caseList,nextlabel);
			CaseBranchTail(caseList,nextlabel);
			return;
		case T_end:
			return;
		default:
			Err();
		}
	}
	void DmpFile(const char* str)
	{
		mFpDmp = fopen(str,"w");
	}

	void Err(const char* str = "")
	{
		/*ErrManager::Register(
			Error::Create(ERR_SYNTEX_ERROR,mpTok)
			);		*/
		Error::Create(ERR_SYNTEX_ERROR,(char*)temp.c_str(),mpTok->LineNo)->Dump();
		printf("%s\n",str);

		printf("Failed To Compile\n");
		exit(0);
		A();
	}
	
};

#endif