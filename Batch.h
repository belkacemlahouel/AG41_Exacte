#ifndef __BATCH__
#define __BATCH__

#include <vector>
#include <iostream>
#include "Produit.h"

using namespace std;

class Batch{

	private:
		vector<Produit*> batch;

	public:
		Batch(){}
		~Batch(){}
		void addProduit(Produit*);
		int dateDueBatch();
		int getBatchSize();
        void printBatch();
        int getDueMin();
        vector<Produit*> getBatch();
        Client* getBatchClient();

        int coutStockage(int);
        int sommeAvancesDueMin();
};

#endif // BATCH
