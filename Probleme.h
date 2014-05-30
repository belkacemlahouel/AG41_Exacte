#ifndef __PROBLEME__
#define __PROBLEME__

#include "Batch.h"

using namespace std;

class Probleme {
	private:
		int m; 							// Nombre de clients
		int n; 							// Nombre de produits total
		int c;							// Capacité du transporteur
		int eta;						// Coefficient pour les coûts de transport
		int* nh;						// Nombre de produits/client
		vector<Client*> clients;		// Liste des clients
		vector<Produit*> produits;		// Liste des produits
		vector<Batch*> batches;

	public:
		Probleme(int, int, int*, vector<Client*>, vector<Produit*>);
		Probleme();
		~Probleme(){}
		void build_batches();
		void printBatches();
		void eraseProduit(vector<Produit*> &prods, Produit* p);
        Produit* produitDueMin(vector<Produit*> &prods);
        Produit* produitDueMinClient(vector<Produit*> prods, Client* c);
};

#endif // __PROBLEME__
