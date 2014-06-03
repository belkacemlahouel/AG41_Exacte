#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <list>
#include "Probleme.h"

using namespace std;

class Parser{

	private:
		int n; // nb produits
		int m;
		float eta;
		int c; // capacite transporteur
		
		list<Client> clients;
		list<Produit> produits;
	
	public:
		Parser(){}
		~Parser(){}
		void parseFile(string name);
		void remove_spaces(string &str);

};

#endif // __PARSER__