//---------------------------------------
//          PARTIE BELKA
//---------------------------------------


void Probleme2::solve() {
    // On trie les produits par ordre décroissant des dates dues
    sort(produits.begin(), produits.end(),
         Tools::comparatorProduitPtrDateDueDec);

    // Heuristique
    solutionHeuristique();

    // On appelle la résolution récursive
    vector<Batch> cur(0);
    vector<Produit*> res = produits;
    solve(cur, res);

    // -------
    // heuristique4000(produits);
    // -------

//    for (int i = 0; i < bestSol.size(); ++i) {
//        bestSol[i]->printBatch();
//    }

//    sort(bestSol.begin(), bestSol.end(),
//         Tools::comparatorBatchPtrCoutStockageCourant);
}

// bestSol -> vector(#produits) et on utilise (int) iter ?
// Solution itérative... pour le moment !
void Probleme2::solve(vector<Batch> cur, vector<Produit*> res) {
    bool f = true; // Pour voir comment on initialise la date courante

    while (res.size() > 0) {
        Batch tmp = Batch(res[0]);
        res.erase(res.begin());

        bool b = true;
        // b : est-ce qu'on doit continuer à chercher
        //      si d'autres éléments doivent être ajoutés au batch
        //      pour le même client, en supposant un tri DEC sur les dates dues

        for (int i = 0; b && i < res.size(); ++i) {
            if (tmp.getClient()->getNum() == res[i]->getClient()->getNum()) {
                cout << "Même client" << endl;
                if (dateCourante - res[i]->dateDue() >                /// la première fois qu'on passe ici, dateCourante = 0 ?
                                    res[i]->getClient()->getDist()) { /// getDist*2 ?
                    // On ne doit pas l'ajouter, mais on ferait bien
                    // de vérifier si on peut, amélioration...
                    // Stocker revient moins cher qu'un AR ?
                    cout << "VRAI1";
                    if (livraisonImmediate(res[i])) {
                        cout << "VRAI";
                        tmp.addProduit(res[i]);
                        res.erase(res.begin()+i);
                    } else {
                        b = false;
                    }
                    cout << endl;
                } else {
                    // avant de l'ajouter, vérifier si cela vaut le coût...
                    tmp.addProduit(res[i]);
                    res.erase(res.begin()+i);
                }
            } else cout << "Pas même client" << endl;
        }

        if (f) {
            dateCourante = tmp.dateDueGlobale()-tmp.getClient()->getDist();
            f = false;
        } else {
            dateCourante = min(dateCourante-tmp.getClient()->getDist()*2,
                           tmp.dateDueGlobale()-tmp.getClient()->getDist()*2);
        }

        cout << "Date courante : " << dateCourante << endl;
        tmp.printBatch();
        cur.push_back(tmp);
    }

    // On renverse l'ordre des batchs
    // (ils sont du dernier à livrer au premier là)
    reverse(cur.begin(), cur.end());

    evalBestSol = evaluerSolution(cur);
    bestSol = cur;
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
float Probleme2::livraison(Batch b) {
    /* rep est le coût de livraison + stockage du batch */
	float rep = b.getClient()->getDist() * 2*eta;
    // cout << "Cout de livraison client " << b->getClient()->getNum();
    // cout << " : " << rep << endl;

	dateCourante += b.getClient()->getDist();
	rep += b.coutStockage(dateCourante);

	cout<<"Batch pour client "<<b.getProduits()[0]->getClient()->getNum()<<", Date : "<<dateCourante<<"/"<<b.dateDueGlobale()<<endl;

	dateCourante += b.getClient()->getDist();
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
float Probleme2::evaluerSolution(vector<Batch> s) {
    cout << "Evaluation solution, dateCourante = " << dateCourante << endl;
    cout << "Detail de la solution :\n";
    float ev = 0;
    for (int i = 0; i < s.size(); ++i) {
        ev += livraison(s[i]);
    }
    return ev;
}

// True si on devrait livrer ce produit tout de suite
// False sinon, et on devrait plutôt livrer au moins après un AR
// On suppose que le produit peut attendre un AR
// ATTENTION, ON NE TIENT PAS COMPTE DE L'AVANCE QU'ON DEVRA PRENDRE
//      SUR LES AUTRES PRODUITS A CAUSE DE CELUI LA... REVOIR
bool Probleme2::livraisonImmediate(Produit* p) {
    // On calcule le coût de la livraison tout de suite, pour ce produit
    // Cela inclut aussi les stockages
    float maintenant = 0;

    dateCourante += p->getClient()->getDist();      // Aller

    maintenant = p->coutStockage(dateCourante);     // On livre

    dateCourante += p->getClient()->getDist()*2;    // Retour puis re-aller

    // Alternative, on livre dans un AR
    // Du coup, on fait un AR rien que pour ce produit
    float apres = 0;

    apres = p->coutStockage(dateCourante);          // On livre
    apres += eta*p->getClient()->getDist()*2;       // AR en plus

    dateCourante -= p->getClient()->getDist()*3;

    // Et malgré tout, on préfère livrer immédiatement, si c'est le même coût
    return (maintenant > apres);
}

void Probleme2::computeCoutsStockageCourants(vector<Batch> reste, float date) {
    for (int i = 0; i < reste.size(); ++i) {
        reste[i].computeCoutStockageCourant(date);
    }
}
void Probleme2::computeCoutsStockageCourantsInit(vector<Batch> reste) {
    for (int i = 0; i < reste.size(); ++i) {
        reste[i].computeCoutStockageCourant(reste[i].getDateGlobale());
    }
}









// Encore une heuristique
// Tant qu'il reste des produits à livrer
//      On les trie par ordre de clients ASC
//      On trie par date due (on suppose le tri stable...) DEC
//      On regarde pour chaque client, le produit le plus tardif -> batch
//          On regarde les autres, voir si ils doivent être ajoutés ou pas
//              Selon juste un critère : si on a le temps de faire l'AR ou pas
//
//      On regarde classe les batchs obtenus par couts de stockage DEC
// On commence par le dernier, jusqu'au premier
//      On livre le dernier à min(dc-AR, dd - A) et on calcule eval

// -----
// cs(produit k) = tho(client i)*eta + (dd-dc-tho(client i))*beta(client i)
// après livraison chez client i : dc <- min(dc-2*tho(client i), dd-tho(client i))
// initialisation de la dc <- 999999 (inf)
//                 puis dc <- [formule] mais on prends la supposée dd(batch sélectionné)
// -----
void Probleme2::heuristique4000(vector<Produit*> _p) {
    // vector<Produit*> p;
    // p.assign(_p.begin(), _p.end());

    // sort(p.begin(), p.end(), Tools::comparatorProduitPtrClientDateDue);

    // // for (int i = 0; i < p.size(); ++i) {
    // //     p[i]->printProduit();
    // // }

    // vector<Batch*> b;

    // // on fait les batchs de manière bidon (à améliorer)
    // // on ajoute à chaque fois qu'on est obligés à cause d'un AR trop grand
    // for (int i = 0; i < p.size(); ++i) {
    //     Batch* tmp = new Batch(p[i]);
    //     while (i+1 < p.size() && tmp->getClient()->getNum() == p[i+1]->getClient()->getNum()
    //             && tmp->getDateGlobale() < { // tant que la date est inférieure à AR/2 ? = A
    //         if () { // inférieur à AR on ajoute
    //             tmp.addProduit(p[i+1]);
    //         }
    //         ++i;
    //     }
    //     b.push_back(tmp);
    // }

    // // DU COUP on a tous les batchs c'est cool ma poule, heuristique sisi

    // // et on peut alors parler du tri des batchs sur les couts de stockages en fonction de la date due

    // // on affiche la solution trouvée + évaluation
}