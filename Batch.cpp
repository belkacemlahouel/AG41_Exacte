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

/* Retourne la date due la plus petite dans ce batch */

int Batch::getDueMin(){
    int min = batch[0]->getDateDue();
    int i;

    for(i=1;i<batch.size();++i){
        if(batch[i]->getDateDue() < min){
            min = batch[i]->getDateDue();
        }
    }

    return min;
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

/* Si les batches sont bien construits, tous les produits dans les batches sont destinés au même client
 * On peut donc dire qu'un Batch est destiné à un seul client */
Client* Batch::getBatchClient(){
    return batch[0]->getClient();
}

vector<Produit*> Batch::getBatch(){
    return this->batch;
}
