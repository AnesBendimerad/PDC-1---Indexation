#include "stdafx.h"
#include "Hasher.h"

int Hasher::hash(string token)
{
	int resultat = 0;
	for (unsigned int i = 0; i < token.length(); i++)
	{
		resultat <<= 2;
		resultat += token[i];
	}
	return resultat;
}
