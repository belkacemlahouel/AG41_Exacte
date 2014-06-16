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

		static bool comparatorBatchLength(Batch b1, Batch b2) {
			// cout << "batchptr comparator" << endl;
			return b1.getProduits().size() > b2.getProduits().size() ;
		};

        static bool comparatorProduitPtrDateDueDec(Produit* p1, Produit* p2) {
            return !comparatorProduitPtrDateDue(p1, p2);
        };

        static bool comparatorBatchPtrDateDueDec(Batch* b1, Batch* b2) {
            return !comparatorBatchPtrDateDue(b1, b2);
        };

        // ATTENTION, ne pas oublier de màj les couts de stockages courants
        // Pour les produits des batchs concernés par le tri
        static bool comparatorBatchPtrCoutStockageCourant(Batch* b1, Batch* b2) {
            return b1->coutStockageCourant()>b2->coutStockageCourant();
        };
        static bool comparatorBatchCoutStockageCourant(Batch b1, Batch b2) {
            return b1.coutStockageCourant()>b2.coutStockageCourant();
        };

        static void bubbleSort(int *num,int r){
            int i, j, flag = 1;    // set flag to 1 to start first pass
            int temp;             // holding variable
            int numLength = r;
            for(i = 1; (i <= numLength) && flag; i++)
            {
                  flag = 0;
                  for (j=0; j < (numLength -1); j++)
                 {
                       if (num[j+1] < num[j])      // descending order
                      {
                            temp = num[j];             // swap elements
                            num[j] = num[j+1];
                            num[j+1] = temp;
                            flag = 1;               // indicates that a swap occurred.
                       }
                  }
             }
             return;   //arrays are passed to functions by address; nothing is returned
        };

		static bool comparatorBatchCoutsStockageCourantsDec(Batch b1, Batch b2) {
            return b1.coutStockageCourant()>b2.coutStockageCourant();
        };



        // Trient sur les numéros de client (ASC)
        // Et par dates dues DEC pour un même client
		static bool comparatorProduitPtrClientDateDue(Produit* p1, Produit* p2) {
			if (p1->getClient()->getNum() == p2->getClient()->getNum())
				return p1->dateDue() > p2->dateDue(); // DECROISSANT
			else
				return p1->getClient()->getNum() < p2->getClient()->getNum();
		};

		static bool comparatorProduitClientDateDue(Produit p1, Produit p2) {
			if (p1.getClient()->getNum() == p2.getClient()->getNum())
				return p1.dateDue() > p2.dateDue(); // DECROISSANT
			else
				return p1.getClient()->getNum() < p2.getClient()->getNum();
		};
};

#endif // __TOOLS__
