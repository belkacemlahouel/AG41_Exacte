#include "Probleme2.h"

/* Initialise le problème avec les données collectées par le parser */
void Probleme2::init(int _capa, float _eta, vector<Client*> _clients,
                         vector<Produit*> _produits) {
	capa = _capa;
	eta = _eta;

	clients = _clients;

	produits = _produits;

	evalBestSol = 999999999; /* Valeur volontairement grande pour être sûr de trouver mieux */

    nbBatchsMini = new int[clients.size()];
    nbBatchsUsed = new int[clients.size()];

    for (int i = 0; i < clients.size(); ++i) {
        nbBatchsUsed[i] = 0;
        nbBatchsMini[i] = 0;
    }
}

// Instance avec appel parseur
Probleme2::Probleme2(string filename) {
    Parser r(filename);
    init(r.getCapa(), r.getEta(), r.getClients(), r.getProduits());
}

/* Affiche les produits (jobs) du problème */
void Probleme2::printProduits(){
vector<Produit*>::iterator it;

    cout<<"\nListe des jobs :\n\n";

    for(it = produits.begin();it != produits.end(); ++it){
        cout<<"\tNum : "<<(*it)->getNum()<<"\n";
        cout<<"\tClient : "<<(*it)->getClient()->getNum()<<"\n";
        cout<<"\tSon cout unitaire: "<<(*it)->getClient()->coutUnitaireStockage()<<"\n";
        cout<< setprecision(12) << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << "\tDate due : "<<(*it)->dateDue()<<"\n";
        cout<<"\n";
    }
}

/* Affiche une liste de produits*/
void Probleme2::printProduits(vector<Produit*> plist){
vector<Produit*>::iterator it;

    cout<<"\nListe des jobs :\n\n";

    for(it = plist.begin();it != plist.end(); ++it){
        cout<<"\tNum : "<<(*it)->getNum()<<"\n";
        cout<<"\tClient : "<<(*it)->getClient()->getNum()<<"\n";
        cout<<"\tSon cout unitaire: "<<(*it)->getClient()->coutUnitaireStockage()<<"\n";
        cout<< setprecision(12) << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << "\tDate due : "<<(*it)->dateDue()<<"\n";
        cout<<"\n";
    }
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
	evalBestSol = 99999999;

	nbBatchsMini = new int[clients.size()];
    nbBatchsUsed = new int[clients.size()];

    for (int i = 0; i < clients.size(); ++i) {
        nbBatchsUsed[i] = 0;
        nbBatchsMini[i] = 0;
    }

	cout << "Initialisation instance de test OK\n";
}

// Destructeur
Probleme2::~Probleme2() {
	Tools::viderVector(clients);
	Tools::viderVector(produits);
    // Prends directement en compte les delete pour le parser, normalement

    delete[] nbBatchsUsed;
    delete[] nbBatchsMini;

	// Tools::viderVectorNoPtr(bestSol);
}

/* Affiche une liste de batches */
void Probleme2::printBatchs(vector<Batch> blist) {
	int i;
	for(i = 0; i < blist.size(); ++i) {
        blist[i].printBatch();
	}
}

/* écriture de la meilleure solution, de façon plus détaillée */
void Probleme2::printBestSol() {
    int i;

    setDates_livraison_bestSol();

    cout << "______________________________________________________\n";
    cout << "\nMeilleure solution trouvee :\n\n";

    for (i = 0; i < bestSol.size(); ++i) {
        cout << "0--->" << bestSol[i].getClient()->getNum() << "--->0\n";
        cout << "Details du batch :\n";
        bestSol[i].printBatch();
        cout<<"\tCout stock. client  : "<<bestSol[i].coutStockage(bestSol[i].getDate_livraison());
        cout <<"\n\n";
    }
    cout << "Evaluation de cette solution : " << evalBestSol << "\n";
    cout << "______________________________________________________\n";
}

/* Affiche une solutoin donnée et son évaluation */
void Probleme2::printSol(vector<Batch> solution,float evalCurSol) {
	int i;
    cout << "______________________________________________________\n";
    cout << "Solution courante trouvee :\n\t";
    for (i = 0; i < solution.size(); ++i) {
        cout << "0--->" << solution[i].getClient()->getNum() << "--->";
    }
    cout << "0\n\n";	// A la fin, on revient chez le fournisseur
    cout << "Evaluation de cette solution : " << evalCurSol << "\n";
    cout << "______________________________________________________\n";
}

// ----------------------------------------------------------
// ----------------------- RESOLUTION -----------------------
// ----------------------------------------------------------

/* Fonction de résolution, arbre avec élagage */

void Probleme2::solve(){

	//solutionHeuristique();

    vector<Batch> res;
    build_batches_bruteforce(res);
	vector<Batch> curSol;
    float curEval = 0;
    float curTime = 0;


    /* Tri des batches pour une meilleure efficacité */

    // sort(res.begin(), res.end(),
        // Tools::comparatorBatchLengthDec);                    // Tri 1
        // Tools::comparatorBatchCoutsStockageCourantsDec);     // Tri 2
        // Tools::comparatorBatchLengthDecDateDueGlobaleDec);   // Tri 3
        // Tools::comparatorCoefSpecial);

    // computeCoutMini();

    solve(curSol, res,curTime, curEval);
}

/* Fait tous les batches possibles, en prenant compte le fait que certains batches sont inutiles */
void Probleme2::build_batches_bruteforce(vector<Batch> &cur){

    int i;

    cout<<"Recherche de toutes les combinaisons de batches...\n";

    for(i=0;i<clients.size();++i){

        vector<Produit*> prodsClient = getProdsClient(i+1);
        //cout<<"Produits du client "<<(i+1)<<" :\n";
        //printProduits(prodsClient);

        // Petit tri pour être sûrs que les produits sont bien ordonnés
        sort(prodsClient.begin(), prodsClient.end(),
         Tools::comparatorProduitPtrDateDue);

        int k;
        int limit;

        if (capa > prodsClient.size()) limit = prodsClient.size();
        else limit = capa;

        for(k=limit-1;k>=0;--k){ // k = 0;k<limit;++k

            int n = prodsClient.size(); // nb produits
            int r = k+1;
            vector<int*> combinations;


            std::vector<bool> v(n);
            std::fill(v.begin() + n - r, v.end(), true);
            int j;

            do {
                j = 0;
                int* ptr = new int[n];
                for (int i = 0; i < n; ++i) {
                    if (v[i]) {
                        ptr[j] = i;
                        ++j;
                    }
                }

                // On vérifie encore l'ordre dans la permutation, pour ne pas avoir des choses
                // comme [0,2,1] qui feraient échouer isUseless
                Tools::bubbleSort(ptr,r);

                if(!isUseless(ptr,r)){
                    combinations.push_back(ptr);
                } else delete[] ptr; // MOTHERFUCKER pourquoi ça ne marche pas ?
            } while (std::next_permutation(v.begin(), v.end()));

            // On a toutes les combinaisons sous formes d'indice, maintenant il faut les ajouter aux combinaisons
            // Des batches.

            //printCombinations(combinations,r);

            int l,m;

            for(l=0;l<combinations.size();++l){
                Batch temp = Batch();
                for(m=0;m<r;++m){
                    temp.addProduit(prodsClient[combinations[l][m]]);
                }
                if(temp.size() > 0)
                    cur.push_back(temp);
            }

			Tools::viderVector(combinations);
        }
    }

    //printBatchs(cur);
    cout<<"Recherche terminee.\n";
    cout<<"Nombre de batches trouves : "<<cur.size()<<endl;
}

/* On peut dire qu'une permutation est inutile si l'écart entre les indices un à un est != 1
 * On peut le dire parce qu'on sait qu'un indice de 3 représente un produit qui arrive
 * plus tard qu'un produit représenté par 1 ou 2. [1,3] est inutile à faire parce qu'on sait qu'il
 * sera AU MOINS pire que [1,2].
 *
 * IL FAUDRA SECURISER CETTE FONCTION EN FAISANT DES TRIS REGULIERS POUR SAVOIr SI TOUT EST BIEN EN ORDRE*/
bool Probleme2::isUseless(int* permut, int r){

    int i;
    for(i=0;i<r-1;++i){
        if(permut[i+1]-permut[i] > 1){
            return true;
        }
    }
    return false;
}

/* Affichage des permutations d'indices*/
void Probleme2::printCombinations(vector<int*> combs,int r){

    int i,j;

    for(j=0;j<combs.size();++j){
        cout<<"[";
        for(i=0;i<r;++i){
            cout<<" "<<combs[j][i]<<" ";
        }
        cout<<"]\n";
    }
}

/* Récupère tous les produits du client d'incide num.
 * Note : les batches sont récupérés dans l'ordre croissant */
vector<Produit*> Probleme2::getProdsClient(int num){

    vector<Produit*> prods;

    int i;
    for(i = 0;i<produits.size();++i){
        if(produits[i]->getClient()->getNum() == num){
            prods.push_back(produits[i]);
        }
    }
    return prods;
}

void Probleme2::solve(vector<Batch> curSol, vector<Batch> res,float curTime,float curEval){

    // ----
    vector<int> tmp; // Liste des numéros des produits déjà livrés
    for (int i = 0; i < curSol.size(); ++i) {
        for (int j = 0; j < curSol[i].size(); ++j) {
            tmp.push_back(curSol[i].getProduits()[j]->getNum());
        }
    }
    // Nous avons alors la liste des numéros de produits déjà livrés

    // Nous cherchons le coût minimum sur les batchs restants à livrer
    float min = 0;
    sort(res.begin(), res.end(), Tools::comparatorBatchLengthDec);
    for (int i = 0; i < res.size(); ++i) {
        bool cont = true; // Vérifier que tous les produits du batch sont différents
        for (int j = 0; j < res[i].size() && cont; ++j) {
            if (Tools::contains(tmp, res[i].getProduits()[j]->getNum())) {
                cont = false;
            }
        }

        if (cont) {
            // Calcul du coût de la livraison de ce batch
            min += res[i].getClient()->getDist()*2*eta;
            // Ajout de tous ces produits au vecteur des produits déjà livrés
            for (int j = 0; j < res[i].size(); ++j) {
                tmp.push_back(res[i].getProduits()[j]->getNum());
            }
        }
    }

    // if (curSol.size() == 0) {
    //     cout << "Min = " << min << endl;
    //     cout << "coutMini = " << coutMini << endl;
    //     return;
    // }
    // ----

	// ----
	sort(res.begin(), res.end(),
        // Tools::comparatorBatchLengthDec);                    // Tri 1
        // Tools::comparatorBatchCoutsStockageCourantsDec);     // Tri 2
        // Tools::comparatorBatchLengthDecDateDueGlobaleDec);   // Tri 3
        Tools::comparatorCoefSpecial);
	// ----

    /* Il faut évaluer la solution à chaque tour, pour voir si on peut cut ou pas */
    if(curSol.size() > 0){
        // if (coutMini+curEval > evalBestSol){
        // if(curEval > evalBestSol){
            //cout<<"Solution plus mauvaise : cut.\n\n"<<endl;
        if(min+curEval >= evalBestSol){
            return;
        }
    }

    /* Tous les produits sont inclus dans la solution, on peut donc regarder si elle est meilleure ou non */
    if(res.size() == 0){
                if(curEval < evalBestSol){
                    cout<<"Meilleure solution trouvee. On l'enregistre.\n";
                    reverse(curSol.begin(), curSol.end()); // on inverse avant de rendre la meilleure solution, puisqu'elle était inversée

                    bestSol = curSol;
                    // evalBestSol = curEval+coutMini;
                    evalBestSol = curEval;
                    printBestSol();

                } else {
                    //cout<<"Pire solution, on oublie.\n";
                }
            return;
    }

	/* Test de toutes les combinaisons de livraison de lots récursivement*/
    vector<Batch>::iterator it = res.begin();

    while(it != res.end()){
		Batch tempBatch = *it;

        vector<Batch> newSol = curSol;
        newSol.push_back(tempBatch);

        vector<Batch> newRest = res;
        removeBatchAndDoubles(newRest, tempBatch);

        float newTime = curTime;
        float newEval = curEval;

        // Comptage des batchs déjà livrés, jusque là
        int num = newSol[newSol.size()-1].getClient()->getNum();
        ++nbBatchsUsed[num-1];

        /* Calcul du temps et des coûts */
        if(newSol.size() == 1){
            newTime = newSol[0].dateDueGlobale();
            newEval = newSol[0].coutStockage(newTime);
            // if (nbBatchsUsed[num-1] > nbBatchsMini[num-1]) // >= ?
                newEval += newSol[0].getClient()->getDist()*2*eta;
            newTime -= newSol[0].getClient()->getDist();
        } else {
            int indice = newSol.size() - 1;
            newTime -= newSol[indice].getClient()->getDist();
            if(newTime > newSol[indice].dateDueGlobale()){
                newTime = newSol[indice].dateDueGlobale();
            }

            // if (nbBatchsUsed[num-1] > nbBatchsMini[num-1]) // >= ?
                newEval += newSol[indice].getClient()->getDist()*2*eta;

            newEval += newSol[indice].coutStockage(newTime);
            newTime -= newSol[indice].getClient()->getDist();
        }

        solve(newSol,newRest,newTime,newEval);

        --nbBatchsUsed[num-1];

        it++;
    }

    // -----
    Tools::viderVectorNoPtr(tmp);
    // -----
}

/* Solution heuristique. Algo :
 * - créer les batch comme d'habitude. Si un batch se voit livré plus tôt que sa date due, reprendre les batchs aux alentours*/

void Probleme2::heuristique(){

    vector<Batch> batches(0);
    vector<Produit*> res = produits;
    build_batches(batches,res);

    vector<Batch> cursol(0);
    float curCost = 0;
    heuristique(cursol, batches, curCost);

}

void Probleme2::build_batches(vector<Batch> &cur, vector<Produit*> res){

    if(res.size() == 0){
        return;
    }

    /* On cherche à construire le nouveau batch en fonction du produit à la date due maximale dans res (à livrer au plus tard en théorie) */
    Produit* firstProd = produitDueMax(res);

    /* Le nouveau batch est construit */
    Batch tmp = Batch(firstProd);

    /* Le nouveau produit ajouté au batch, on peut donc le supprimer de la liste */
    removeProduct(res,firstProd);

    /* On construit le batch à partir du premier produit dedans */
    int i = 0;

    while(tmp.getProduits().size() < capa && i<res.size()){
        if(res[i]->getClient()->getNum() == tmp.getClient()->getNum()){
            /* Condition pour être ajouté au batch : Doit avec une date due <= le premier produit du batch - l'aller-retour */
            if(res[i]->dateDue() >= (tmp.getProduits()[0]->dateDue() - tmp.getProduits()[0]->getClient()->getDist()*2)){
                tmp.addProduit(res[i]);
                removeProduct(res,res[i]);
            } else {
                ++i; // On n'incrémente que si aucun produit n'a été supprimé
            }
        } else {
            ++i;
        }
    }

    cur.push_back(tmp);

    tmp.printBatch();

    /* On continue à créer les batches et à les placer dans la timeline récursivement */
    build_batches(cur, res);
}

/* Après avoir construit des lots de façon heuristique, on teste leurs permutations pour trouver une solution viable */
void Probleme2::heuristique(vector<Batch> curSol, vector<Batch> resBatches,float curEval){

    if(resBatches.size() == 0){
        curEval = evaluerSolution_auto(curSol);
        //printBatchs(curSol);
        //printSol(curSol,curEval);

        if(curEval < evalBestSol){
            cout<<"Meilleure solution trouvee. On l'enregistre.\n";
            reverse(curSol.begin(), curSol.end()); // on inverse avant de rendre la meilleure solution, puisqu'elle était inversée
            bestSol = curSol;
            evalBestSol = curEval;
            printBestSol();
        } else {
            //cout<<"Pire solution, on oublie.\n";
        }

        return;
    }

    if(curSol.size() > 0){
        curEval = evaluerSolution_auto(curSol);
        if(curEval > evalBestSol){
            //cout<<"Solution plus mauvaise : cut.\n\n"<<endl;
            return;
        }
    }

    /* Test de toutes les combinaisons de livraison récursivement*/
    vector<Batch>::iterator it = resBatches.begin();

    while(it != resBatches.end()){
        Batch temp = *it;

        vector<Batch> newSol = curSol;

        newSol.push_back(temp);
        vector<Batch> newRest = resBatches;

        removeBatch(newRest,temp);

        heuristique(newSol,newRest,curEval);
        ++it;
    }
}

/* Supprime un batch d'une liste de batches */
void Probleme2::removeBatch(vector<Batch> &newRest,Batch temp){

    vector<Batch>::iterator it;

    for(it=newRest.begin();it!=newRest.end();++it){
        if(*it == temp){
            newRest.erase(it);
            break;
        }
    }

}

/* Supprime un batch d'une liste de batches
    ainsi que tous les batches précédent
    et tous les batches qui contiennent les mêmes
    produits que dans le Batch temp (gros gros élagage)*/
void Probleme2::removeBatchAndDoubles(vector<Batch> &newRest,Batch temp){

    vector<Batch>::iterator it;

    /* Première étape : on supprime temp */
    for(it=newRest.begin();it!=newRest.end();++it){
        if(*it == temp){
            newRest.erase(it);
            break;
        }
    }

    /* Seconde étape : on supprime les batches contenant  un ou plusieurs produits
       similaires avec temp */

    it = newRest.begin();
    while(it != newRest.end()){
        if(sameProducts(temp,*it)){
            it = newRest.erase(it);
        } else {
            ++it;
        }
    }

}

/* Regarde si deux batches ont des produits en commun (1 seul suffit pour renvoyer true) */
bool Probleme2::sameProducts(Batch batch1,Batch batch2){

    int i,j;

    for(i=0;i<batch1.getProduits().size();++i){
        for(j=0;j<batch2.getProduits().size();++j){
            if(batch1.getProduits()[i]->getNum() == batch2.getProduits()[j]->getNum()){
                return true;
            }
        }
    }
    return false;
}

/* Supprime le produit p de la liste de produits plist */
void Probleme2::removeProduct(vector<Produit*> &plist, Produit* p){
    vector<Produit*>::iterator it;

    for(it=plist.begin();it!=plist.end();++it){
        if(*it == p){
            plist.erase(it);
            break;
        }
    }
}

/* Renvoie un pointeur sur le produit qui a la plus grosse date due parmis une liste de produits */
Produit* Probleme2::produitDueMax(vector<Produit* > plist){

    Produit* max = plist[0];
    int i;
    for(i=0;i<plist.size();++i){
        if(plist[i]->dateDue() > max->dateDue()){
            max = plist[i];
        }
    }
    return max;
}

/* Contrairement à evaluerSolution, cette fonction va évaluer la solution donnée en reconstruisant
   lui même la timeline de livraison à partir d'une liste de batch. Elle n'a donc pas besoin de dateCourante
   /!\ CETTE EVALUATION SE FAIT EN BACKTRACK /!\ */
float Probleme2::evaluerSolution_auto(vector<Batch> s) {

    float curTime = 0;

    float ev = 0;
    /* La date de départ est la date de livraison du dernier batch + le temps de retour a l'entrepôt */
    curTime = s[0].dateDueGlobale() + s[0].getClient()->getDist();
    for (int i = 0; i < s.size(); ++i) {

        ev += s[i].getClient()->getDist() *2*eta; /* On fait payer l'aler-retour */

        curTime -= s[i].getClient()->getDist();

        /* Si la date de livraison est trop tôt pour le client, il faut attendre à l'entrepôt pour ne pas l'amener trop tôt */
        if(curTime > s[i].dateDueGlobale()){
            curTime = s[i].dateDueGlobale();
        }

        float tempCoutStock = s[i].coutStockage(curTime);
        s[i].setDate_livraison(curTime);
        ev += tempCoutStock;
        curTime -= s[i].getClient()->getDist();
    }
    return ev;
}

/* Prend la meilleure solution, et reconstitue en backtrack les dates de livraison */
void Probleme2::setDates_livraison_bestSol(){
    vector<Batch> &tempBestSol = bestSol;
    reverse(tempBestSol.begin(),tempBestSol.end());

    float curTime = tempBestSol[0].dateDueGlobale() + tempBestSol[0].getClient()->getDist(); // départ du temps à l'entrepot, à la fin
    for (int i = 0; i < tempBestSol.size(); ++i) {

        curTime -= tempBestSol[i].getClient()->getDist();

        if(curTime > tempBestSol[i].dateDueGlobale()){
            curTime = tempBestSol[i].dateDueGlobale();
        }

        tempBestSol[i].setDate_livraison(curTime);

        curTime -= tempBestSol[i].getClient()->getDist();
    }

    reverse(tempBestSol.begin(),tempBestSol.end());
}

void Probleme2::computeCoutMini() {
    coutMini = 0;
    sort(produits.begin(), produits.end(),
        Tools::comparatorProduitPtrClientDateDue);

    cout << "Capacité du transporteur : " << capa << endl;
    printProduits();

    for (int i = 0; i < produits.size(); ++i) {
        coutMini += produits[i]->getClient()->getDist()*2*eta;

        int j = 0;
        int num = produits[i]->getClient()->getNum();
        ++nbBatchsMini[num-1];

        while ((i+j) < produits.size() && j < capa && produits[i+j]->getClient()->getNum() == num) {
            ++j;
        }

        i += j;
    }
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
