#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include<vector>
#include "token.h"

using namespace std;

class TokenStream
{
private:
	vector<Token*> mStream;
	int mCurr;
	FILE* mfp;
	
	////////////////////////////
	void print(char *c,const char* type )
	{
		if(type)
		{
			fprintf(mfp,"%s : (%s)\n",c,type);				
		}
		else
			fprintf(mfp,"%s \n",c);				
	}
	void print(char *c)
	{
		fprintf(mfp,"%s \n",c);				
	}
	void print(char *c,int v)
	{
		fprintf(mfp,"%s ,%d\n",c,v);				
	}
	////////////////////////////
public:
	TokenStream()
	{
		mCurr = 0;
	}
	int Retreat(int offset = -1)
	{
		if( offset != -1)
		{
			if( (mCurr - offset ) >= 0 )
			{
				mCurr-=offset;
				return 1;
			}
			else
				return 0;
		}
		else
		{
			mCurr = 0;
			return 1;
		}

	}
	void Add(Token* t)
	{
		mStream.push_back(t);
	}
	Token* GetNextToken()
	{
		int total;
		total = mStream.size();
		if ( mCurr < total)
			return mStream[mCurr++];
		else
			return NULL;
	}
	void Dump(FILE* fp = stdout)
	{
		mfp = fp;
		Token* t;
		int total;
		total = mStream.size();
		
		for(int i = 0 ; i < total; i++)
		{
			t = mStream[i];

			switch( t->Class )
			{
			case T_program:
				print("T_program",t->Name.c_str());
				break;
			case T_begin:
				print("T_begin",t->Name.c_str());
				break;
			case T_end:
				print("T_end",t->Name.c_str());
				break;
			case T_var:
				print("T_var",t->Name.c_str());
				break;
			case T_function:
				print("T_function",t->Name.c_str());
				break;
			case T_procedure:
				print("T_procedure",t->Name.c_str());
				break;
			case T_integer:
				print("T_integer",t->Name.c_str());
				break;
			case T_real:
				print("T_real",t->Name.c_str());
				break;
			case T_array:
				print("T_array",t->Name.c_str());
				break;
			case T_of:
				print("T_of",t->Name.c_str());
				break;
			case T_if:
				print("T_if",t->Name.c_str());
				break;
			case T_then:
				print("T_then",t->Name.c_str());
				break;
			case T_else:
				print("T_else",t->Name.c_str());
				break;
			case T_while:
				print("T_while",t->Name.c_str());
				break;
			case T_do:
				print("T_do",t->Name.c_str());
				break;
			case T_not:
				print("T_not",t->Name.c_str());
				break;
			case T_identifier:
				print("T_identifier",t->Name.c_str()); 				
				break;
			case T_constant:
				print("T_constant",t->Name.c_str()); 				
				break;
			case T_relop:				
				print("T_relop",t->Name.c_str()) ;
				break;
			case T_addop:
				print("T_addop",t->Name.c_str());
				break;
			case T_mulop:
				print("T_mulop",t->Name.c_str());
				break;
			case T_doubledot:
				print("T_doubledot",t->Name.c_str());
				break;
			case T_rightparen:
				print("T_rightparen",t->Name.c_str());
				break;
			case T_leftparen:
				print("T_leftparen",t->Name.c_str());
				break;
			case T_rightbracket:
				print("T_rightbracket",t->Name.c_str());
				break;
			case T_leftbracket:
				print("T_leftbracket",t->Name.c_str());
				break;
			case T_unaryminus:
				print("T_unaryminus",t->Name.c_str());
				break;
			case T_unaryplus:
				print("T_unaryplus",t->Name.c_str());
				break;
			case T_endmarker:
				print("T_endmarker",t->Name.c_str());
				break;
			case T_eof:
				print("T_eof");
				break;		
			}
		}
		
		
		
	}
};
#endif