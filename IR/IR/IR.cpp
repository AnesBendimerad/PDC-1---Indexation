// IR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Hasher.h"
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	Hasher *h = new Hasher();
	string s = "aymen";
	cout << " h = " << h->hash(s) << endl;
	cout << "Hello World - Aymen !" << endl;
	getchar();
	delete h;
	return 0;
}

