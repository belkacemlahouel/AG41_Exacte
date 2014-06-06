#ifndef __TOOLS__
#define __TOOLS__

#include <iostream>
#include <vector>
#include "Probleme.h"
#include "Produit.h"
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
};

#endif // __TOOLS__
