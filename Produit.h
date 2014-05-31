#ifndef __PRODUIT__
#define __PRODUIT__

#include "Client.h"

class Produit {
	private:
		int i;					// N° du job
		int date;				// Date due du job i
		Client *client;				// Client qui demande le job i

	public:
		Produit(int, int, Client*);
		int getDateDue();
		Client* getClient();
		int getNProduit();
		int coutStockage(int, Client*);

};

#endif // __PRODUIT__
