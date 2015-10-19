#include "stdafx.h"
#include "Term.h"

ofstream & operator << (std::ofstream & os, Term * rec) {
	size_t sz = rec->token.length();
	os.write((char*)&sz, sizeof(sz));
	const char* cc = rec->token.c_str();
	os.write(cc, sz);
	os.write((const char*)& rec->totalFrequency, sizeof(rec->totalFrequency));
	os.write((const char*)& rec->documentNumber, sizeof(rec->documentNumber));
	os.write((char*)& rec->postingList, sizeof(rec->postingList));
	return os;
};
ifstream & operator >> (std::ifstream & is, Term * rec) {
	size_t sz;
	is.read((char *)&sz, sizeof(sz));
	if (sz != 0) {
		char *myToken=(char*)malloc(sizeof(char)*(sz+1));
		is.read(myToken, sz);
		myToken[sz] = '\0';
		rec->token.assign(myToken, myToken + sz);
		free(myToken);
	}
	is.read(( char*)& rec->totalFrequency, sizeof(rec->totalFrequency));
	is.read((char*)& rec->documentNumber, sizeof(rec->documentNumber));
	is.read((char*)& rec->postingList, sizeof(rec->postingList));
	return is;
};
