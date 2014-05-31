#include "Client.h"

Client::Client(int h, int cout_u,int dist) {
	fini = false;
	this->h = h;
	this->cout_u = cout_u;
	this->dist = dist;
}

int Client::getCoutUnitaireStockage() {
	return cout_u;
}
