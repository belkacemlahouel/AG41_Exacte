#include "Batch.h"

using namespace std;

// Ajout d'un produit à la liste des produits constituant un batch
void Batch::addProduit(Produit* p){
	batch.push_back(p);
}

// Retourne la somme des dates dues du batch
// Utilisée pour évaluer le coût minimal du batch
// DEPRECATED
int Batch::dateDueBatch(){
	int i;
	int sum = 0;

	for(i = 0; i < batch.size(); ++i) {
		sum += batch[i]->getDateDue();
	}

	return sum;
}
// --- A UTILISER
// --- Renvoie la somme des avances sur la date due de livraison minimale dans ce batch
int Batch::sommeAvancesDueMin(){
    int i;
    int sum = 0;
    int min = batch[0]->getDateDue();

    for (i = 1; i < batch.size(); ++i) {
        if (min < batch[i]->getDateDue())
            sum += (batch[i]->getDateDue() - min);
        else {
            sum += (min - batch[i]->getDateDue()) * i;
            min = batch[i]->getDateDue();
        }
    }

    return sum;
}

// --- Retourne le coût de stockage du batch chez le client, à partir d'une date
// --- On fait comment si la date spécifiée est en retard ?
int Batch::coutStockage(int date) {
    int i;
    Client* c = getBatchClient();
    int cout_stockage_total = 0;

    for (i = 0; i < batch.size(); ++i) {
        cout_stockage_total += batch[i]->coutStockage(date, c);
    }

    return cout_stockage_total;
}

// Retourne la première date due du batch, sert de référentiel pour la livraison du batch
int Batch::getDueMin(){
    int min = batch[0]->getDateDue();
    int i;

    for(i = 1; i < batch.size(); ++i) {
        if(batch[i]->getDateDue() < min){
            min = batch[i]->getDateDue();
        }
    }

    return min;
}

// Renvoie la taille du batch concerné
int Batch::getBatchSize(){
    return batch.size();
}

// Print le batch avec un certain format
void Batch::printBatch(){
    int j;
    cout << "Batch pour client " << getBatchClient()->getNum() << " :\n";

	for(j = 0; j < batch.size(); ++j) {
		cout << "\tProduit " << batch[j]->getNProduit() <<
                ", Date due : " << batch[j]->getDateDue() << "\n";
    }

    cout << "\tDate due globale    : " << getDueMin() << "\n";
    //cout << "\tSomme des avances   : " << sommeAvances() << "\n\n";
}

// Si les batches sont bien construits, tous les produits dans les batches sont destinés
//      au même client, on peut donc dire qu'un Batch est destiné à un seul client
// En particulier, ce client commun est le client du premier produit constituant ce batch
Client* Batch::getBatchClient(){
    return batch[0]->getClient();
}

// Renvoie la liste des produits de ce Batch
vector<Produit*> Batch::getBatch(){
    return batch;
}
