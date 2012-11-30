#ifndef IL_GENERATOR_H
#define IL_GENERATOR_H

#include<string>
#include<list>
#include<stdio.h>
using namespace std;


class ILGenerator
{
private:
	list<string> mILBuffer;	
	int count;
	FILE* mFp;
	string s;
public:
	ILGenerator(const char* fname)
	{ 
		count = 0;
		mFp = fopen(fname,"w");
		if( mFp == NULL )
		{
			printf("Cant open file for writing\n");
			exit(0);
		}

	}
	void Add(string il)
	{
		mILBuffer.push_back(il);
		count++;
		if( count == 2 )
		{			
			s = mILBuffer.front();			
			mILBuffer.pop_front();			
			fprintf(mFp,"%s \n",s.c_str());
			count = 0;
		}
	}
	void Flush()
	{
		while( mILBuffer.empty() == false )
		{
			s = mILBuffer.front();			
			mILBuffer.pop_front();			
			fprintf(mFp,"%s \n",s.c_str());
		}
		fflush(mFp);
	}
	int GetCount()
	{
		return mILBuffer.size();
	}
	
};

#endif
