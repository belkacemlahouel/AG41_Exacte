#include "Batch.h"

Batch::Batch(Produit* p) {
	// Pas besoin d'allouer de la mémoire au vector ?
	produits.push_back(p);
}

int Batch::size() {
	return produits.size();
}

vector<Produit*> Batch::getProduits() {
	return produits;
}

Client* Batch::getClient() {
	return produits[0]->getClient();
}

void Batch::addProduit(Produit* p) {
	produits.push_back(p);
}

float Batch::dateDueGlobale() {
	int min = produits[0]->dateDue();
    int i;

    for(i = 1; i < size(); ++i) {
        if(produits[i]->dateDue() < min){
            min = produits[i]->dateDue();
        }
    }

    return min;
}

float Batch::coutStockage(int date) {
    int i;
    float cout_stockage_total = 0;

    for (i = 0; i < size(); ++i) {
        cout_stockage_total += produits[i]->coutStockage(date);
    }

    return cout_stockage_total;
}

float Batch::sommeAvancesMin() {
    int i;
    int sum = 0;
    int min = produits[0]->dateDue();

    for (i = 1; i < size(); ++i) {
        if (min < produits[i]->dateDue())
            sum += (produits[i]->dateDue() - min);
        else {
            sum += (min - produits[i]->dateDue()) * i;
            min = produits[i]->dateDue();
        }
    }

    return sum;
}

void Batch::printBatch(){
    int j;
    cout << "Batch pour client " << getClient()->getNum() << " :\n";

	for(j = 0; j < size(); ++j) {
		produits[j]->printProduit();
    }

    cout << "\tDate due globale    : " << dateDueGlobale() << "\n";
    cout << "\tSomme des avances   : " << sommeAvancesMin() << "\n\n";
}


