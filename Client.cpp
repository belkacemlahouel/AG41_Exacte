#include "Client.h"

Client::Client(int h, float cout_u,int dist) {
	fini = false;
	this->h = h;
	this->cout_u = cout_u;
	this->dist = dist;
}

float Client::getCoutUnitaireStockage() {
	return cout_u;
}

int Client::getDist(){
    return this->dist;
}

int Client::getNum(){
    return this->h;
}
