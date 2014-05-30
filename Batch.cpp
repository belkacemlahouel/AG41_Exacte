#include "Batch.h"

using namespace std;

Batch::Batch(){}

void Batch::addProduit(Produit* p){
	batch.push_back(p);
}

/* Retourne la somme des dates due du batch */

int Batch::dateDueBatch(){

	int i;
	int sum = 0;

	for(i=0;i<this->batch.size();++i){
		sum += this->batch[i]->getDateDue();
	}

	return sum;
}

int Batch:: getBatchSize(){
    return batch.size();
}

int Batch::printBatch(){
    int j;
    cout<<"Batch pour client "<<this->batch[0]->getClient()->getNum()<<" :\n";
		for(j=0;j<this->batch.size();++j){
			cout<<"\tProduit "<<this->batch[j]->getNProduit()<<", Date due : "<<this->batch[j]->getDateDue()<<"\n";
    }
}
