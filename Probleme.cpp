#include "Probleme.h"

Probleme::Probleme(int m, int n, int* nh, vector<Client*> clts, vector<Produit*> prods, Transport transp) {
	this->m = m;
	this->n = n;
	this->nh = nh;
	this->clients = clts;
	this->produits = prods;
	this->transport = &transp;
}
