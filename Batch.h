#ifndef __BATCH__
#define __BATCH__

#include <vector>
#include <iostream>
#include "Produit.h"

using namespace std;

class Batch{

	private:
		vector<Produit*> produits;
		float date_livraison;       // la date � laquelle a �t� livr� le batch
        float cout_st_cour;         // Cout de stockage courant

	public:
		Batch(Produit*);
		Batch(vector<Produit*> plist);
		Batch();
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
    
        void computeCoutStockageCourant(float date);
        float coutStockageCourant();
};

#endif // __BATCH__
