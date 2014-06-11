#include "Produit.h"

Produit::Produit(int _i, float _date, Client* _client) {
	i = _i;
	date = _date;
	client = _client;
}

float Produit::dateDue() {
	return date;
}

Client* Produit::getClient() {
	return client;
}

int Produit::getNum() {
	return i;
}

// Comment implémenter les coûts si retard ...?
float Produit::coutStockage(float t) {
	return (date-t) * client->coutUnitaireStockage();
}

void Produit::printProduit() {
	cout << "\tProduit " << getNum() << ", Date due : " << dateDue() << "\n";
}
