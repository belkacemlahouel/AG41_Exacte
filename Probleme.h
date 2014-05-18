#ifndef __PROBLEME__
#define __PROBLEME__

#include <vector>
#include "Produit.h"
#include "Client.h"
#include "Transport.h"

using namespace std;

class Probleme {
	private:
		int m; 						// Nombre de clients
		int n; 						// Nombre de produits total
		int* nh;					// Nombre de produits/client
		vector<Client*> clients;		// Liste des clients
		vector<Produit*> produits;	// Liste des produits
		Transport* transport;		// Transporteur

	public:
		Probleme(int, int, int*, vector<Client*>, vector<Produit*>, Transport);
};

#endif // __PROBLEME__