#ifndef __TOOLS__
#define __TOOLS__

#include <iostream>
#include <vector>
#include "Batch.h"

using namespace std;

class Tools {
	public:
		// Vider un vector<T>
		template<class T> static void viderVector(vector<T> vect) {
			while (vect.size() > 0) {
				delete vect[0];
				vect.erase(vect.begin());
			}
		};

		// Fonctions statiques de comparaison
		static bool comparatorProduitPtrDateDue(Produit* p1, Produit* p2) {
			// cout << "produitptr comparator" << endl;
			return p1->dateDue() < p2->dateDue();
		};

		static bool comparatorBatchPtrDateDue(Batch* b1, Batch* b2) {
			// cout << "batchptr comparator" << endl;
			return b1->dateDueGlobale() < b2->dateDueGlobale();
		};

		static bool comparatorBatchDateDue(Batch b1, Batch b2) {
			// cout << "batchptr comparator" << endl;
			return b1.dateDueGlobale() > b2.dateDueGlobale();
		};

		static bool comparatorCoutStockage(Batch b1, Batch b2) {
			// cout << "batchptr comparator" << endl;
			return b1.getClient()->coutUnitaireStockage() < b2.getClient()->coutUnitaireStockage();
		};

        static bool comparatorProduitPtrDateDueDec(Produit* p1, Produit* p2) {
            return !comparatorProduitPtrDateDue(p1, p2);
        };

        static bool comparatorBatchPtrDateDueDec(Batch* b1, Batch* b2) {
            return !comparatorBatchPtrDateDue(b1, b2);
        };
};

#endif // __TOOLS__
