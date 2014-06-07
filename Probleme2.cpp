#include "Probleme2.h"

void Probleme2::init(int _capa, float _eta, vector<Client*> _clients,
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
Probleme2::Probleme2(string filename) {
    Parser r(filename);
    init(r.getCapa(), r.getEta(), r.getClients(), r.getProduits());
}

// Instance de base, voir sujet
Probleme2::Probleme2() {
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

// Destructeur
Probleme2::~Probleme2() {
	Tools::viderVector(clients);
	Tools::viderVector(produits);
	Tools::viderVector(batchs);
}

// Printings
void Probleme2::printBatchs() {
	int i;
	for(i = 0; i < batchs.size(); ++i) {
        batchs[i]->printBatch();
	}
}

void Probleme2::printProduits() {
	cout << "Liste des produits à livrer :" << endl;
	for (int i = 0; i < produits.size(); ++i) {
		produits[i]->printProduit();
	}
	cout << endl;
}

void Probleme2::printBestSol() {
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

void Probleme2::printSol(int niveau) {
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

// Livraison et annulation de livraison (incluent chgts temporels)
float Probleme2::livraison(Batch* b) {
	float rep = b->getClient()->getDist() * 2*eta;
	dateCourante += b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);
	dateCourante += b->getClient()->getDist();
	return rep;
}

float Probleme2::annulerLivraison(Batch* b) {
	float rep = b->getClient()->getDist() * 2*eta;
	dateCourante -= b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);
	dateCourante -= b->getClient()->getDist();
	return rep;
}

// ----------------------------------------------------------
// ----------------------- RESOLUTION -----------------------
// ----------------------------------------------------------

void Probleme2::solve() {
    
}

void Probleme2::buildBatchs() {
    
}

void Probleme2::solutionHeuristique() {
    
}

void Probleme2::solve(int iter, vector<Batch*> reste) {
    
}

bool Probleme2::encorePossible(vector<Batch*> reste) {
	for (int i = 0; i < reste.size(); ++i) {
		if (reste[i]->dateDueGlobale() < // dateCourante) {
            dateCourante + reste[i]->getClient()->getDist()) {
			
			return false;
		}
	}
	return true;
}
