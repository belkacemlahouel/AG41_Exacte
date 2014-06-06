#ifndef __BATCH__
#define __BATCH__

#include <vector>
#include <iostream>
#include "Produit.h"

using namespace std;

class Batch{

	private:
		vector<Produit*> produits;

	public:
		Batch(Produit*);
		~Batch(){}
		void printBatch();

		void addProduit(Produit*);
		float dateDueGlobale();
		int size();

        vector<Produit*> getProduits();
        Client* getClient();

        float coutStockage(int date);
        float sommeAvancesMin();
};

#endif // __BATCH__
