#ifndef LEX_H
#define LEX_H

#include <stdio.h>
#include "global.h"
#include "SimpleToken.h"
#include "SymbolTable.h"
#include "TokenStream.h"
class Lex
{
private:
	TokenStream* mTStream;
	char *mFileName;
public:
	Lex(const char* file,TokenStream* tstream)
	{
		mTStream = tstream; 
		mFileName = strdup(file);
	}
	void start()
	{
		Token *t;
		SimpleTokenizer T(mFileName);
		do
		{
			t = new Token();
			*t = T.GetNextToken();
			if( t->Class != T_null)
				mTStream->Add(t);
		}while( t->Class != T_eof );
	}	
};
#endif
