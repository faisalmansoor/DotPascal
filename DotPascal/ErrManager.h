#ifndef ERR_MANAGER_H
#define ERR_MANAGER_H

#include<vector>
#include<stdio.h>
#include "Error.h"

using namespace std;

class ErrManager
{
private:
	vector<Error*> ErrList;
	ErrManager()
	{}
	static ErrManager* mPtrErrManager;
public:
	static ErrManager* Create()
	{
		if( mPtrErrManager == NULL)
		{
			mPtrErrManager = new ErrManager();
		}
		
		return mPtrErrManager;
	}
	void Add(Error* err)
	{
		ErrList.push_back(err);
	}
	void Dump(FILE* fp = stdout)
	{
		vector<Error*>::iterator iter;
		
		for( iter = ErrList.begin() ; iter != ErrList.end() ; iter++)
		{
			(*iter)->Dump(fp);
			fprintf(fp,"\n");
		}
	}
	
	static void Register(Error* pe)
	{
		ErrManager* pem = ErrManager::Create();
		pem->Add(pe);
		
	}
};
ErrManager* ErrManager::mPtrErrManager = NULL;
#endif
