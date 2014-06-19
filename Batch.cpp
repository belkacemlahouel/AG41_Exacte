#include "Batch.h"

Batch::Batch() {
    date_livraison = 0;
    dateGlobale = 0;
}

Batch::Batch(Produit* p) {
	// Pas besoin d'allouer de la mémoire au vector ?
	produits.push_back(p);
	date_livraison = 0;
	dateGlobale = dateDueGlobale();
}

Batch::Batch(vector<Produit*> plist) {
	// Pas besoin d'allouer de la mémoire au vector ?
	produits = plist;
	date_livraison = 0;
	dateGlobale = dateDueGlobale();
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
	dateGlobale = dateDueGlobale();
}

/* la date due globale est la date due minimum entre toutes les dates dues de tous les jobs du batch */
float Batch::dateDueGlobale() {
	float min = produits[0]->dateDue();
    int i;

    for(i = 1; i < size(); ++i) {
        if(produits[i]->dateDue() < min){
            min = produits[i]->dateDue();
        }
    }

    return min;
}

float Batch::coutStockage(float date) {
    int i;
    float cout_stockage_total = 0;

    for (i = 0; i < produits.size(); ++i) {
        cout_stockage_total += produits[i]->coutStockage(date);
    }

    return cout_stockage_total;
}

void Batch::printBatch(){
    int j;
    cout << "Batch pour client " << getClient()->getNum() << " :\n";

	for(j = 0; j < size(); ++j) {
		produits[j]->printProduit();
    }

    cout << "\tCoeff stock. client : " << getClient()->coutUnitaireStockage() << "\n";
    cout << "\tDate de livraison   : " << date_livraison << "/" << dateDueGlobale() << "\n";
}

void Batch::setDate_livraison(float date){
    date_livraison = date;
}

float Batch::getDate_livraison(){
    return date_livraison;
}

void Batch::computeCoutStockageCourant(float date) {
    cout_st_cour = coutStockage(date);
}

float Batch::coutStockageCourant() {
    return cout_st_cour;
}

float Batch::getDateGlobale(){
    return dateGlobale;
}

/* Deux batches sont égaux s'ils ont le même nombre de produits, et que ces derniers son similaires, dans le même ordre */
bool Batch::operator==(Batch b){

    int i;

    if(this->produits.size() != b.getProduits().size()){
        return false;
    }

    for(i=0;i<this->produits.size();++i){
        if(produits[i] != b.getProduits()[i]){
            return false;
        } /* On compare les pointeurs directement */
    }

    return true;
}


