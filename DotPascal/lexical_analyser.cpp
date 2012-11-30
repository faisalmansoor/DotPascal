#pragma warning(disable:4786) 

#include"lex.h"
#include "symboltable.h"
#include "SyntexBox.h"
#include "ILGenerator.h"

void MostImportant(const char* in,const char* out);

void main(int argc  , char * argv[] )
{
	string infname;
	string outfname;
	
	if( argc > 1 )
	{	
		infname = argv[1];
	}
	else		
	{
		char temp[256];
		printf("Input File : ");
		gets(temp);
		infname = temp;
	}


	int len = 0;
	len  =  infname.find_first_of ('.');
	outfname = infname.substr(0,len) + ".il";

	MostImportant(infname.c_str(),outfname.c_str());

	ILGenerator emit( outfname.c_str() );
		
	TokenStream mTokenStream;

	Lex l(infname.c_str(),&mTokenStream);
	l.start();	
	outfname = infname.substr(0,len) + ".tok";
	FILE* tokfp = fopen(outfname.c_str(),"w");
	mTokenStream.Dump(tokfp);
	fclose(tokfp);


	SyntexBox sb(mTokenStream,emit);
	outfname = infname.substr(0,len) + ".std";
	sb.DmpFile(outfname.c_str());
	sb.start();
}

void MostImportant(const char* in,const char* out)
{
	printf("SoftTronics(R) DotPascal PASCAL Compiler.  Version 1.0.100.0\n");
	printf("Copyright (C) SoftTronics Corporation 2002-2003. All rights reserved.\n");
	printf("Assembling '%s' , to il --> '%s'\n",in,out);
	printf("Source file is ANSI\n\n\n");
}
