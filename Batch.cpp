#include "Batch.h"

Batch::Batch(){}

void Batch::addProduit(Produit* p){
	this->push_back(p);
}

/* Retourne la somme des dates due du batch */

int Batch::dateDueBatch(){

	int i;
	int sum = 0;
	
	for(i=0;i<this->batch.size();++i){
		sum += this->batch[i];
	}
	
	return sum;
}