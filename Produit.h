#ifndef __PRODUIT__
#define __PRODUIT__

#include <iostream>

using namespace std;

class Produit {
	private:
		int i;					// N° du job
		int di;					// Date due du job i
		int cli;				// Client qui demande le job i

	public:
		Produit();
};

#endif // __PRODUIT__