#include "Probleme.h"

/* Constructeur par défaut, n'est utilisé que pour tester le jeu d'essai fourni dans l'énoncé
 * Note : ce jeu d'essai est en fait le jeu d'essai dans l'énoncé, mais tout est décalé de 60
 * (Pour pouvoir partir à la date t=0 et on t=-60 */

Probleme::Probleme(){

	m = 4;
	n = 5;
	c = 5;
	eta = 2;

	/* Création des clients */

	clients.push_back(new Client(1,3/2.,100));
	clients.push_back(new Client(2,3,100));
	clients.push_back(new Client(3,9/2.,100));
	clients.push_back(new Client(4,6,100));

	/* Création des produits */

	produits.push_back(new Produit(1, 310, clients[0]));
	produits.push_back(new Produit(2, 310, clients[0]));
	produits.push_back(new Produit(3, 300, clients[2]));
	produits.push_back(new Produit(4, 360, clients[2]));
	produits.push_back(new Produit(5, 400, clients[2]));

	/* construction des batches */

	build_batches();

	printBatches();

}

Probleme::Probleme(int m, int n, int* nh, vector<Client*> clts, vector<Produit*> prods) {
	this->m = m;
	this->n = n;
	this->nh = nh;
	this->clients = clts;
	this->produits = prods;
}

/* Construit la liste des batches en fonction de la liste des produits
 * Les batches sont construits de la manière suivante :
 *  - si les produits sont une date due dont la différence entre avec celle qui est minimale parmis ces produits est < à 2* la distance client/entrepôt
 */

void Probleme::build_batches(){

	int i;
	vector<Produit*> tempProduits = this->produits; // On va créer une liste temporaire : On va supprimer les produits à mesures qu'ils sont assignés à des bacs
	int k=0;
	bool trouve;
	int tempDueMin;
	Produit* tempProd;
	Client* tempClient;
	vector<Produit*>::iterator it;

	while(tempProduits.size() > 0){

		this->batches.push_back(new Batch());

		tempProd = produitDueMin(tempProduits);
		tempClient = tempProd->getClient(); /* On enregistre le client, puisque tous les autres produits du batch seront aussi pour ce client */
		tempDueMin = tempProd->getDateDue();

		this->batches[k]->addProduit(tempProd);

		eraseProduit(tempProduits,tempProd);

		if(tempProduits.size() > 0){
            trouve = true;
			while(this->batches[k]->getBatchSize() < c && trouve == true){
                trouve = false;
				// On cherche un produit dont la date due est <= tempDueMin + distance client/entrepot*2
				tempProd = produitDueMinClient(tempProduits,tempClient);
				if(tempProd != NULL){
                    if(tempProd->getDateDue() < tempDueMin + (tempClient->getDist()*2)){
                        this->batches[k]->addProduit(tempProd);
                        eraseProduit(tempProduits,tempProd);
                        trouve = true;
                    }
				}
			}
		}

		++k;
	}

	// Prendre tous les produits qui ont une date due < à date due min + 2*dist
	// Les rajouter au batch

	// Rajouter ce batch à la liste de batches
	// Supprimer ces produits nouvellement ajoutés de tempProduits

}


/* Ecrase le produit p dans la liste de produits prods */

void Probleme::eraseProduit(vector<Produit*> &prods, Produit* p){
	vector<Produit*>::iterator it;

	for(it = prods.begin();it != prods.end();++it){
		if(*it == p){
			prods.erase(it);
			break;
		}
	}

}

/* Cette fonction prend en paramètre une liste de produits, et retourne le produit qui a la date due min parmis toute cette liste */

Produit* Probleme::produitDueMin(vector<Produit*> &prods){

	Produit *prodMin = prods[0];
	int i;

	for(i=0;i<prods.size();++i){
		if(prods[i]->getDateDue() < prodMin->getDateDue()){
			prodMin = prods[i];
		}
	}

	return prodMin;
}

/* Retourne le produit qui a la date due la plus faible, parmis les produits commandés par le client c */

Produit* Probleme::produitDueMinClient(vector<Produit*> prods, Client* cli){

	vector<Produit*> tempProds;
	int i;

	for(i=0;i<prods.size();++i){
		if(prods[i]->getClient()->getNum() == cli->getNum()){
			tempProds.push_back(prods[i]);
		}
	}

    if(tempProds.size() > 0){ // Vérification nécessaire pour éviter la segfault dans produitDueMin
        return produitDueMin(tempProds);
    } else {
        return NULL;
    }
}

void Probleme::printBatches(){

	int i,j;

	for(i=0;i<batches.size();++i){
        batches[i]->printBatch();
	}
}
