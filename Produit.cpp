#include "Produit.h"

// Produit(numéro de produit, date de livraison due, Client concerné)
Produit::Produit(int i, double date, Client* client) {
	this->i = i;
	this->date = date;
	this->client = client;
}

// Renvoie la date due de livraison
double Produit::getDateDue() {
	return date;
}

// Renvoie un pointeur sur le Client concerné
Client* Produit::getClient() {
	return client;
}

// Renvoie le numéro de produit
int Produit::getNProduit() {
	return i;
}

// Coût de stockage du produit, si la commande arrive trop tôt chez le Client
int Produit::coutStockage(int t, Client* c) {
	return (date - t) * c->getCoutUnitaireStockage();
}
