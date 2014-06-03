#include "Probleme.h"

/* Constructeur par défaut, n'est utilisé que pour tester le jeu d'essai fourni dans l'énoncé
 * Note : ce jeu d'essai est en fait le jeu d'essai dans l'énoncé, mais tout est décalé de 60
 * (Pour pouvoir partir à la date t=0 et on t=-60 */

Probleme::Probleme(){

	m = 4;
	n = 5;
	c = 5;
	eta = 2;
	eval_bestSol = 0;

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
    solution_heuristique();

	printBatches();
    printBestSol();
}

Probleme::Probleme(int m, int n, int* nh, vector<Client*> clts, vector<Produit*> prods) {
	this->m = m;
	this->n = n;
	this->nh = nh;
	this->clients = clts;
	this->produits = prods;
}

/* /!\ FONCTION A AMELIORER /!\ Est-il possible de trouver directement les bons batches, sans entrer dans l'arborescence ?
 *
 * Construit la liste des batches en fonction de la liste des produits
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

	int i;

	for(i=0;i<batches.size();++i){
        batches[i]->printBatch();
	}
}

/* Cette fonction créé une solution avec l'heuristique suivante :
 *      - trouver le batch avec la somme des dates dues * le coût unitaire  le plus petit
 *      - regarder si ce batch compromet la livraison des autres (= si on livre ce batch, peut-on respecter la livraison a temps des autres ?)
 *      - si ce batch n'est pas compromettant, on le livre, sinon, on livre le batch le plus urgent (date due la plus faible)
 *
 *      NOTE : CET HEURISTIQUE N'EST PEUT-ETRE PAS VIABLE SELON LES CONFIGURATIONS : mettre en backup l'heuristique "livraison par date due minimum
 *      croissante" ?
 */

float Probleme::solution_heuristique(){

    vector<Batch*> tempBatches = this->batches;
    Batch* tempBatch;
    vector<int> solution;
    int time = 0;
    float totalCost = 0;

    while(tempBatches.size() > 0){
        solution.push_back(0); // 0 est l'entrepôt

        tempBatch = batchEvalCoutMin(tempBatches);

        if(estCompromettant(tempBatches,tempBatch,time)){
            tempBatch = batchDueMin(tempBatches);
        }

        solution.push_back(tempBatch->getBatchClient()->getNum());

        totalCost += batchCost(tempBatch,time);

        eraseBatch(tempBatches,tempBatch); // le batch a été livré, on le supprime de la liste de batches à livrer
    }

    solution.push_back(0); // retour final à l'entrpôt

    this->bestSol = solution;
    this->eval_bestSol = totalCost;

    return totalCost;
}

/* CETTE FONCTION HEURISTIQUE EST MOINS EFFICACE, MAIS FONCTIONNE A TOUS LES COUPS

float Probleme::solution_heuristique(){

    vector<Batch*> tempBatches = this->batches;
    Batch* tempBatch;
    vector<int> solution;
    int time = 0;
    float totalCost = 0;

    while(tempBatches.size() > 0){
        solution.push_back(0); // 0 est l'entrepôt

        tempBatch = batchDueMin(tempBatches);

        solution.push_back(tempBatch->getBatchClient()->getNum());

        totalCost += batchCost(tempBatch,time);

        eraseBatch(tempBatches,tempBatch); // le batch a été livré, on le supprime de la liste de batches à livrer
    }

    solution.push_back(0); // retour final à l'entrpôt

    this->bestSol = solution;
    this->eval_bestSol = totalCost;

    return totalCost;
} */

/* Cette fonction retourne le batch qui est évalué comme le moins coûteux de livrer (parmis une liste de batches), sans information de temps */
Batch* Probleme::batchEvalCoutMin(vector<Batch*> &bs){
    int i;
    Batch* best = bs[0];

    for(i=1;i<bs.size();++i){
        if(bs[i]->dateDueBatch()*bs[i]->getBatchClient()->getCoutUnitaireStockage() < best->dateDueBatch()*best->getBatchClient()->getCoutUnitaireStockage()){
            best = bs[i];
        }
    }
    return best;
}

/* Retourne le batch qui contient le produit à la date due la plus faible, parmis une liste de batches */

Batch* Probleme::batchDueMin(vector<Batch*> &bs){

    Batch* batchMin = bs[0];
    int i;

    for(i=1;i<bs.size();++i){
        if(bs[i]->getDueMin() < batchMin->getDueMin()){
            batchMin = bs[i];
        }
    }

    return batchMin;
}

/* Cette méthode calcule le coût de la livraison d'un batch à un temps "time".
 * Ce coût comporte :
 *  - Les coûts de transport aller/retour (distance entrepot/client * eta * 2)
 *  - Les coûts de stockage (cout_u *(date_due - time) pour chaque produit)
 *
 * Note : la variable time est passée par référence, et modifiée au cours de ce calcul */

float Probleme::batchCost(Batch* b, int &time){

    float cost = 0;
    int i;
    float cu = b->getBatchClient()->getCoutUnitaireStockage();

    cost += b->getBatchClient()->getDist()*this->eta;
    time += b->getBatchClient()->getDist();

    for(i=0;i<b->getBatch().size();++i){
        int dateDue = b->getBatch()[i]->getDateDue();
        cost += cu *(dateDue - time);
    }

    cost += b->getBatchClient()->getDist()*this->eta;
    time += b->getBatchClient()->getDist();

    return cost;
}

/* Efface le batch d'une liste de batches */

void Probleme::eraseBatch(vector<Batch*> &bs, Batch* b){
    vector<Batch*>::iterator it;

    for(it=bs.begin();it!=bs.end();++it){
        if(*it == b){
            bs.erase(it);
            break;
        }
    }
}

/* Affiche la meilleure solution trouvee jusqu'a present */
void Probleme::printBestSol(){
    int i;
    cout<<"______________________________________________________\n";
    cout<<"Meilleure solution trouvee :\n\t";
    for(i=0;i<bestSol.size()-1;++i){
        cout<<bestSol[i]<<"--->";
    }
    cout<<"0\n\n";	// A la fin, on revient chez le fournisseur
    cout<<"Evaluation de cette solution : "<<eval_bestSol<<"\n";
    cout<<"______________________________________________________\n";
}

/* Cette fonction regarde si le batch b est compromettant pour les autres (= si sa livraison compromet la livraison à temps d'un autre batch) */

bool Probleme::estCompromettant(vector<Batch*> &bs,Batch* b, int time){

    int i;
    int tempsLivraison = (b->getBatchClient()->getDist()*2); // temps qu'il faut pour livrer ce batch

    for(i=0;i<bs.size();++i){
        if(bs[i]->getDueMin() < (time + tempsLivraison + bs[i]->getBatchClient()->getDist())){
            return true;
        }
    }

    return false;
}

