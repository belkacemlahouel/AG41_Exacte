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
		Batch();
		~Batch(){}
		void addProduit(Produit*);
		int dateDueBatch();
		int getBatchSize();
        int printBatch();


};

#endif // BATCH
