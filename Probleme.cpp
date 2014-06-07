#include "Probleme.h"

void Probleme::init(int _capa, float _eta, vector<Client*> _clients,
											vector<Produit*> _produits) {
	capa = _capa;
	eta = _eta;

	clients = _clients;
	produits = _produits;

	buildBatchs();

	evalSol = 0;
	evalBestSol = 0;

	dateCourante = 0.0f;
}

// Instance avec appel parseur
Probleme::Probleme(string filename) {
    Parser r(filename);
    init(r.getCapa(), r.getEta(), r.getClients(), r.getProduits());
}

// Instance de base, voir sujet
Probleme::Probleme() {
	capa = 5;
	eta = 2;

	// Création des clients
	clients.push_back(new Client(1, 100, 3/2.));
	clients.push_back(new Client(2, 100, 3));
	clients.push_back(new Client(3, 100, 9/2.));
	clients.push_back(new Client(4, 100, 6));

	// Création des produits, dans le désordre exprès
	produits.push_back(new Produit(1, 310, clients[0]));
	produits.push_back(new Produit(2, 310, clients[0]));
	produits.push_back(new Produit(3, 300, clients[2]));
	produits.push_back(new Produit(4, 360, clients[2]));
	produits.push_back(new Produit(5, 400, clients[2]));

	// construction des batchs
	buildBatchs();

	// initialisation des évaluations
	evalSol = 0;
	evalBestSol = 0;

	// initialisation des solutions
	sol = batchs;
	bestSol = batchs;

	dateCourante = 0;

	cout << "Initialisation instance de test OK\n";
}

Probleme::~Probleme() {
	Tools::viderVector(clients);
	Tools::viderVector(produits);

	Tools::viderVector(batchs);
}


// Batchs faits "bêtement" ici...
void Probleme::buildBatchs() {
	// Tri produits à faire sur les produits
	cout << "Avant tri" << endl;
	printProduits();
	sort(produits.begin(), produits.end(), Tools::comparatorProduitPtrDateDue);
	cout << "Après tri" << endl;
	printProduits();

	vector<Produit*> tmpProduits = produits;

	while (tmpProduits.size() > 0) {
		// cout << "Il reste " << tmpProduits.size() << " produits" << endl;

		Batch* tmp = new Batch(tmpProduits[0]);
		tmpProduits.erase(tmpProduits.begin());

		vector<Produit*>::iterator it = tmpProduits.begin();

		while (it != tmpProduits.end()) {
			// cout << "Recherche de produits avec le même client" << endl;
			if ((*it)->getClient()->getNum() == tmp->getClient()->getNum() &&
														tmp->size() < capa) {
				// cout << "Même client trouvé" << endl;
				tmp->addProduit((*it));
				tmpProduits.erase(it);
			} else {
				++it;
			}
		}

		batchs.push_back(tmp);
	}
}

void Probleme::solutionHeuristique() {
	// Nous n'avons plus qu'à dire que la solution heuristique
	// est la liste des batchs ordonnés
	sort(batchs.begin(), batchs.end(), Tools::comparatorBatchPtrDateDue);
	sol = batchs;

	// cout << "batchs.size() : " << batchs.size() << endl;
	// cout << "bestSol.size() : " << bestSol.size() << endl;

	evalBestSol = 0;
	for (int i = 0; i < sol.size(); ++i) {
		evalBestSol += livraison(sol[i]);
	}

	evalSol = 0;
	bestSol = sol;

	dateCourante = 0;
	// cout << "Solution heuristique OK\n";
}

void Probleme::printBatchs() {
	int i;
	for(i = 0; i < batchs.size(); ++i) {
        batchs[i]->printBatch();
	}
}

void Probleme::printProduits() {
	cout << "Liste des produits à livrer :" << endl;
	for (int i = 0; i < produits.size(); ++i) {
		produits[i]->printProduit();
	}
	cout << endl;
}

void Probleme::printBestSol() {
    int i;
    cout << "______________________________________________________\n";
    cout << "Meilleure solution trouvee :\n\t";
    for (i = 0; i < bestSol.size(); ++i) {
        cout << "0--->" << bestSol[i]->getClient()->getNum() << "--->";
    }
    cout << "0\n\n";	// A la fin, on revient chez le fournisseur
    cout << "Evaluation de cette solution : " << evalBestSol << "\n";
    cout << "______________________________________________________\n";
}

void Probleme::printSol(int niveau) {
	int i;
    cout << "______________________________________________________\n";
    cout << "Solution courante trouvee :\n\t";
    for (i = 0; i <= niveau; ++i) {
        cout << "0--->" << sol[i]->getClient()->getNum() << "--->";
    }
    cout << "0\n\n";	// A la fin, on revient chez le fournisseur
    cout << "Evaluation de cette solution : " << evalSol << "\n";
    cout << "______________________________________________________\n";
}

float Probleme::livraison(Batch* b) {
	float rep = b->getClient()->getDist() * 2*eta;
	dateCourante += b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);
	dateCourante += b->getClient()->getDist();
	return rep;
}

float Probleme::annulerLivraison(Batch* b) {
	float rep = b->getClient()->getDist() * 2*eta;
	dateCourante -= b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);
	dateCourante -= b->getClient()->getDist();
	return rep;
}

void Probleme::solve() {
	cout << "Lancement heuristique" << endl;
	// Appel des autres heuristiques et ne garder que la meilleure solution
	solutionHeuristique();

	printBestSol();
	cout << endl;

	dateCourante = 0;
	cout << "Lancement résolution" << endl;
	solve(0, batchs);

	/*dateCourante = 0;
	cout << "Est-ce que livraison/annulerLivraison fonctionnent ?" << endl;
	cout << "Date avant : " << dateCourante << endl;
	cout << "livraison : " << livraison(batchs[0]) << endl;
	cout << "annulerLivraison : " << annulerLivraison(batchs[0]) << endl;
	cout << "Date après : " << dateCourante << endl;
	cout << "Est-ce qu'elles fonctionnent ? :: OUI" << endl;*/
}

void Probleme::solve(int iter, vector<Batch*> reste) {
	// cout << "reste.size() : " << reste.size() << endl;
	if (evalSol < evalBestSol && encorePossible(reste)) {
		if (iter == batchs.size()-1 && reste.size() == 1) {
			sol[iter] = reste[0];
			evalSol += livraison(reste[0]);

			if (evalSol < evalBestSol) {
				cout << "Une meilleure solution est trouvée" << endl;
				evalBestSol = evalSol;
				bestSol = sol;
			}
			evalSol -= annulerLivraison(reste[0]);
		} else {
			for (int i = 0; i < reste.size(); ++i) {
				// cout << "iter : " << iter << ", " << i << endl;

				sol[iter] = reste[i];
				evalSol += livraison(reste[i]);

				printSol(iter);

				vector<Batch*> reste2 = reste;
				reste2.erase(reste2.begin() + i);
				solve(iter+1, reste2);

				evalSol -= annulerLivraison(reste[i]);
			}
		}
	} else {
		cout << "STOP : ";

		if (evalSol >= evalBestSol) {
			cout << "evalSol=" << evalSol << " >= evalBestSol=" << evalBestSol;
			cout << endl;
		}

		if (!encorePossible(reste)) {
			cout << "Ce chemin mène vers une impasse" << endl;
		}
	}
}

bool Probleme::encorePossible(vector<Batch*> reste) {
	for (int i = 0; i < reste.size(); ++i) {
		if (reste[i]->dateDueGlobale() < // dateCourante) {
						dateCourante + reste[i]->getClient()->getDist()) {
			
			return false;
		}
	}
	return true;
}

// ------

// /* /!\ FONCTION A AMELIORER /!\ Est-il possible de trouver directement les bons batches, sans entrer dans l'arborescence ?
//  *
//  * Construit la liste des batches en fonction de la liste des produits
//  * Les batches sont construits de la manière suivante :
//  *  - si les produits sont une date due dont la différence entre avec celle qui est minimale parmis ces produits est < à 2* la distance client/entrepôt
//  */

// void Probleme::build_batches(){

// 	int i;
// 	vector<Produit*> tempProduits = this->produits; // On va créer une liste temporaire : On va supprimer les produits à mesures qu'ils sont assignés à des bacs
// 	int k=0;
// 	bool trouve;
// 	int tempDueMin;
// 	Produit* tempProd;
// 	Client* tempClient;
// 	vector<Produit*>::iterator it;

// 	while(tempProduits.size() > 0){

// 		this->batches.push_back(new Batch());

// 		tempProd = produitDueMin(tempProduits);
// 		tempClient = tempProd->getClient(); /* On enregistre le client, puisque tous les autres produits du batch seront aussi pour ce client */
// 		tempDueMin = tempProd->getDateDue();

// 		this->batches[k]->addProduit(tempProd);

// 		eraseProduit(tempProduits,tempProd);

// 		if(tempProduits.size() > 0){
//             trouve = true;
// 			while(this->batches[k]->getBatchSize() < c && trouve == true){
//                 trouve = false;
// 				// On cherche un produit dont la date due est <= tempDueMin + distance client/entrepot*2
// 				tempProd = produitDueMinClient(tempProduits,tempClient);
// 				if(tempProd != NULL){
//                     if(tempProd->getDateDue() < tempDueMin + (tempClient->getDist()*2)){
//                         this->batches[k]->addProduit(tempProd);
//                         eraseProduit(tempProduits,tempProd);
//                         trouve = true;
//                     }
// 				}
// 			}
// 		}

// 		++k;
// 	}
// }


// /* Ecrase le produit p dans la liste de produits prods */

// void Probleme::eraseProduit(vector<Produit*> &prods, Produit* p){
// 	vector<Produit*>::iterator it;

// 	for(it = prods.begin();it != prods.end();++it){
// 		if(*it == p){
// 			prods.erase(it);
// 			break;
// 		}
// 	}

// }

// /* Cette fonction prend en paramètre une liste de produits, et retourne le produit qui a la date due min parmis toute cette liste */

// Produit* Probleme::produitDueMin(vector<Produit*> &prods){

// 	Produit *prodMin = prods[0];
// 	int i;

// 	for(i=0;i<prods.size();++i){
// 		if(prods[i]->getDateDue() < prodMin->getDateDue()){
// 			prodMin = prods[i];
// 		}
// 	}

// 	return prodMin;
// }

// /* Retourne le produit qui a la date due la plus faible, parmis les produits commandés par le client c */

// Produit* Probleme::produitDueMinClient(vector<Produit*> prods, Client* cli){

// 	vector<Produit*> tempProds;
// 	int i;

// 	for(i=0;i<prods.size();++i){
// 		if(prods[i]->getClient()->getNum() == cli->getNum()){
// 			tempProds.push_back(prods[i]);
// 		}
// 	}

//     if(tempProds.size() > 0){ // Vérification nécessaire pour éviter la segfault dans produitDueMin
//         return produitDueMin(tempProds);
//     } else {
//         return NULL;
//     }
// }

// float Probleme::solution_heuristique(){

//     vector<Batch*> tempBatches = this->batches;
//     Batch* tempBatch;
//     vector<int> solution;
//     int time = 0;
//     float totalCost = 0;

//     while(tempBatches.size() > 0){
//         solution.push_back(0); // 0 est l'entrepôt

//         tempBatch = batchEvalCoutMin(tempBatches);

//         if(estCompromettant(tempBatches,tempBatch,time)){
//             tempBatch = batchDueMin(tempBatches);
//         }

//         solution.push_back(tempBatch->getBatchClient()->getNum());

//         totalCost += batchCost(tempBatch,time);

//         eraseBatch(tempBatches,tempBatch); // le batch a été livré, on le supprime de la liste de batches à livrer
//     }

//     solution.push_back(0); // retour final à l'entrpôt

//     this->bestSol = solution;
//     this->eval_bestSol = totalCost;

//     return totalCost;
// }

// /* CETTE FONCTION HEURISTIQUE EST MOINS EFFICACE, MAIS FONCTIONNE A TOUS LES COUPS

// float Probleme::solution_heuristique(){

//     vector<Batch*> tempBatches = this->batches;
//     Batch* tempBatch;
//     vector<int> solution;
//     int time = 0;
//     float totalCost = 0;

//     while(tempBatches.size() > 0){
//         solution.push_back(0); // 0 est l'entrepôt

//         tempBatch = batchDueMin(tempBatches);

//         solution.push_back(tempBatch->getBatchClient()->getNum());

//         totalCost += batchCost(tempBatch,time);

//         eraseBatch(tempBatches,tempBatch); // le batch a été livré, on le supprime de la liste de batches à livrer
//     }

//     solution.push_back(0); // retour final à l'entrpôt

//     this->bestSol = solution;
//     this->eval_bestSol = totalCost;

//     return totalCost;
// } */

// /* Cette fonction retourne le batch qui est évalué comme le moins coûteux de livrer (parmis une liste de batches), sans information de temps */
// Batch* Probleme::batchEvalCoutMin(vector<Batch*> &bs){
//     int i;
//     Batch* best = bs[0];

//     for(i=1;i<bs.size();++i){
//         if(bs[i]->dateDueBatch()*bs[i]->getBatchClient()->getCoutUnitaireStockage() < best->dateDueBatch()*best->getBatchClient()->getCoutUnitaireStockage()){
//             best = bs[i];
//         }
//     }
//     return best;
// }

// /* Retourne le batch qui contient le produit à la date due la plus faible, parmis une liste de batches */

// Batch* Probleme::batchDueMin(vector<Batch*> &bs){

//     Batch* batchMin = bs[0];
//     int i;

//     for(i=1;i<bs.size();++i){
//         if(bs[i]->getDueMin() < batchMin->getDueMin()){
//             batchMin = bs[i];
//         }
//     }

//     return batchMin;
// }

// /* Cette méthode calcule le coût de la livraison d'un batch à un temps "time".
//  * Ce coût comporte :
//  *  - Les coûts de transport aller/retour (distance entrepot/client * eta * 2)
//  *  - Les coûts de stockage (cout_u *(date_due - time) pour chaque produit)
//  *
//  * Note : la variable time est passée par référence, et modifiée au cours de ce calcul */

// float Probleme::batchCost(Batch* b, int &time){

//     float cost = 0;
//     int i;
//     float cu = b->getBatchClient()->getCoutUnitaireStockage();

//     cost += b->getBatchClient()->getDist()*this->eta;
//     time += b->getBatchClient()->getDist();

//     for(i=0;i<b->getBatch().size();++i){
//         int dateDue = b->getBatch()[i]->getDateDue();
//         cost += cu *(dateDue - time);
//     }

//     cost += b->getBatchClient()->getDist()*this->eta;
//     time += b->getBatchClient()->getDist();

//     return cost;
// }

// /* Efface le batch d'une liste de batches */

// void Probleme::eraseBatch(vector<Batch*> &bs, Batch* b){
//     vector<Batch*>::iterator it;

//     for(it=bs.begin();it!=bs.end();++it){
//         if(*it == b){
//             bs.erase(it);
//             break;
//         }
//     }
// }


// /* Cette fonction regarde si le batch b est compromettant pour les autres (= si sa livraison compromet la livraison à temps d'un autre batch) */

// bool Probleme::estCompromettant(vector<Batch*> &bs,Batch* b, int time){

//     int i;
//     int tempsLivraison = (b->getBatchClient()->getDist()*2); // temps qu'il faut pour livrer ce batch

//     for(i=0;i<bs.size();++i){
//         if(bs[i]->getDueMin() < (time + tempsLivraison + bs[i]->getBatchClient()->getDist())){
//             return true;
//         }
//     }

//     return false;
// }

