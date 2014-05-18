#ifndef __PRODUIT__
#define __PRODUIT__

#include "Client.h"

class Produit {
	private:
		int i;					// N° du job
		int date;				// Date due du job i
		int client;				// Client qui demande le job i

	public:
		Produit(int, int, int);
		int getDateDue();
		int getNClient();
		int getNProduit();
		int coutStockage(int, Client*);

};

#endif // __PRODUIT__