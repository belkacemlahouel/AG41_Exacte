#include "Client.h"

Client::Client(int _h, float _dist, float _cout) {
	h = _h;
	dist = _dist;
	cost = _cout;
}

float Client::coutUnitaireStockage() {
    return cost;
}

float Client::getDist() {
	return dist;
}

int Client::getNum() {
	return h;
}

