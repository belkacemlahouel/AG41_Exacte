#include "Client.h"

// Client(numéro de client, coût unitaire de stockage, distance au fournisseur)
Client::Client(int h, float cout_u, int dist) {
	fini = false;
	this->h = h;
	this->cout_u = cout_u;
	this->dist = dist;
}

// Renvoie le coût unitaire de stockage pour ce client
float Client::getCoutUnitaireStockage() {
	return cout_u;
}

// Renvoie la distance au fournisseur
int Client::getDist(){
    return this->dist;
}

// Renvoie le numéro de client
int Client::getNum(){
    return this->h;
}
