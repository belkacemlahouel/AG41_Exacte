#ifndef __PROBLEME__
#define __PROBLEME__

#include <vector>
#include "Produit.h"
#include "Client.h"

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
		Probleme(int, int, int*, vector<Client*>, vector<Produit*>, Transport);
		Probleme();
		~Probleme(){}
};

#endif // __PROBLEME__