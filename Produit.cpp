#include "Produit.h"

Produit::Produit(int i, int date, int client) {
	this->i = i;
	this->date = date;
	this->client = client;
}

int Produit::getDateDue() {
	return date;
}

int Produit::getNClient() {
	return client;
}

int Produit::getNProduit() {
	return i;
}

int Produit::coutStockage(int t, Client* c) {
	return (date - t) * c->getCoutUnitaireStockage();
}
