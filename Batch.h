#ifndef __BATCH__
#define __BATCH__

#include <vector>
#include <iostream>
#include "Produit.h"

using namespace std;

class Batch{

	private:
		vector<Produit*> produits;
		float date_livraison;       // la date à laquelle a été livrée le batch

	public:
		Batch(Produit*);
		~Batch(){}
		void printBatch();

		void addProduit(Produit*);
		float dateDueGlobale();
		int size();

        vector<Produit*> getProduits();
        Client* getClient();

        float coutStockage(float date);
        float sommeAvancesMin();

        void setDate_livraison(float date);
        float getDate_livraison();
};

#endif // __BATCH__
