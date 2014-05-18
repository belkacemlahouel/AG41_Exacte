#include "Client.h"

Client::Client(int h, int cout_u) {
	fini = false;
	this->h = h;
	this->cout_u = cout_u;
}

int Client::getCoutUnitaireStockage() {
	return cout_u;
}
