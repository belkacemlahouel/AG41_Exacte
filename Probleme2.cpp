#include "Probleme2.h"

void Probleme2::init(int _capa, float _eta, vector<Client*> _clients,
                         vector<Produit*> _produits) {
	capa = _capa;
	eta = _eta;
    
	clients = _clients;
	produits = _produits;
    
//	buildBatchs();
    
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
	// buildBatchs();
    
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
    // Prends directement en compte les delete pour le parser, normalement
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

// ----------------------------------------------------------
// ----------------------- RESOLUTION -----------------------
// ----------------------------------------------------------

void Probleme2::solve() {
    // On trie les produits par ordre décroissant des dates dues
    sort(produits.begin(), produits.end(),
         Tools::comparatorProduitPtrDateDueDec);
    
    // Heuristique
    solutionHeuristique();

    // On appelle la résolution récursive
    vector<Batch*> cur(0);
    vector<Produit*> res = produits;
    solve(cur, res);
    
//    for (int i = 0; i < bestSol.size(); ++i) {
//        bestSol[i]->printBatch();
//    }
}

// bestSol -> vector(#produits) et on utilise (int) iter ?
// Solution itérative... pour le moment !
void Probleme2::solve(vector<Batch*> cur, vector<Produit*> res) {
    bool f = true; // Pour voir comment on initialise la date courante
    
    while (res.size() > 0) {
        Batch* tmp = new Batch(res[0]);
        res.erase(res.begin());
        
        bool b = true;
        // b : est-ce qu'on doit continuer à chercher
        //      si d'autres éléments doivent être ajoutés au batch
        //      pour le même client, en supposant un tri DEC sur les dates dues
        
        for (int i = 0; b && i < res.size(); ++i) {
            if (tmp->getClient()->getNum() == res[i]->getClient()->getNum()) {
                if (dateCourante - res[i]->dateDue() >
                                    res[i]->getClient()->getDist()) {
                    b = false;
                } else {
                    // avant de l'ajouter, vérifier si cela vaut le coût...
                    tmp->addProduit(res[i]);
                    res.erase(res.begin()+i);
                }
            }
        }
        
        if (f) {
            dateCourante = tmp->dateDueGlobale()-tmp->getClient()->getDist();
            f = false;
        } else {
            dateCourante = min(dateCourante-tmp->getClient()->getDist()*2,
                           tmp->dateDueGlobale()-tmp->getClient()->getDist()*2);
        }
        
        cout << "Date courante : " << dateCourante << endl;
        tmp->printBatch();
        cur.push_back(tmp);
    }
    
    // On renverse l'ordre des batchs
    // (ils sont du dernier à livrer au premier là)
    reverse(cur.begin(), cur.end());
    
    evalBestSol = evaluerSolution(cur);
    bestSol = cur;
}

// ----------------------------------------------------------

// On suppose une heuristique vraiment nulle... Vision pessimiste
void Probleme2::solutionHeuristique() {
    evalBestSol = 9999999999;
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

// Livraison et annulation de livraison (incluent chgts temporels)
// Avec cette méthode, nous avancons dans le temps
float Probleme2::livraison(Batch* b) {
	float rep = b->getClient()->getDist() * 2*eta;
	dateCourante += b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);
	dateCourante += b->getClient()->getDist();
	return rep;
}

// Tandis qu'avec cette méthode, on recule
float Probleme2::annulerLivraison(Batch* b) {
	float rep = b->getClient()->getDist() * 2*eta;
	dateCourante -= b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);
	dateCourante -= b->getClient()->getDist();
	return rep;
}

// On suppose qu'on commence à la bonne date, et les batchs sont corrects
float Probleme2::evaluerSolution(vector<Batch*> s) {
    cout << "Evaluation solution, dateCourante = " << dateCourante << endl;
    float ev = 0;
    for (int i = 0; i < s.size(); ++i) {
        ev += livraison(s[i]);
    }
    return ev;
}
