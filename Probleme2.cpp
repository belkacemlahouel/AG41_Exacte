#include "Probleme2.h"

void Probleme2::init(int _capa, float _eta, vector<Client*> _clients,
                         vector<Produit*> _produits) {
	capa = _capa;
	eta = _eta;

	clients = _clients;

	produits = _produits;

	evalSol = 0;
	evalBestSol = 0;

	dateCourante = 0.0f;
}

// Instance avec appel parseur
Probleme2::Probleme2(string filename) {
    Parser r(filename);
    init(r.getCapa(), r.getEta(), r.getClients(), r.getProduits());
}

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

void Probleme2::printBatchs(vector<Batch*> blist) {
	int i;
	for(i = 0; i < blist.size(); ++i) {
        blist[i]->printBatch();
	}
}

void Probleme2::printBatchs(vector<Batch> blist) {
	int i;
	for(i = 0; i < blist.size(); ++i) {
        blist[i].printBatch();
	}
}

void Probleme2::printBestSol() {
    int i;

    cout << "______________________________________________________\n";
    cout << "Meilleure solution trouvee :\n\n";
    for (i = 0; i < bestSol.size(); ++i) {
        cout << "0--->" << bestSol[i]->getClient()->getNum() << "--->";
    }
    cout << "0\n\n";	// A la fin, on revient chez le fournisseur
    cout << "Evaluation de cette solution : " << evalBestSol << "\n";
    cout << "______________________________________________________\n";
}

/* écriture de la meilleure solution, de façon plus détaillée */
void Probleme2::printBestSol_indo() {
    int i;

    setDates_livraison_bestSol();

    cout << "______________________________________________________\n";
    cout << "\nMeilleure solution trouvee :\n\n";

    for (i = 0; i < bestSol.size(); ++i) {
        cout << "0--->" << bestSol[i]->getClient()->getNum() << "--->0\n";
        cout << "Details du batch :\n";
        bestSol[i]->printBatch();
        cout<<"\tCout stock. client  : "<<bestSol[i]->coutStockage(bestSol[i]->getDate_livraison());
        cout <<"\n\n";
    }
    cout << "Evaluation de cette solution : " << evalBestSol << "\n";
    cout << "______________________________________________________\n";
}

void Probleme2::printBestSol_indo_noptr() {
    int i;

    setDates_livraison_bestSol_noptr();

    cout << "______________________________________________________\n";
    cout << "\nMeilleure solution trouvee :\n\n";

    for (i = 0; i < bestSol_noptr.size(); ++i) {
        cout << "0--->" << bestSol_noptr[i].getClient()->getNum() << "--->0\n";
        cout << "Details du batch :\n";
        bestSol_noptr[i].printBatch();
        cout<<"\tCout stock. client  : "<<bestSol_noptr[i].coutStockage(bestSol_noptr[i].getDate_livraison());
        cout <<"\n\n";
    }
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

void Probleme2::printSol(vector<Batch*> solution,float evalCurSol) {
	int i;
    cout << "______________________________________________________\n";
    cout << "Solution courante trouvee :\n\t";
    for (i = 0; i < solution.size(); ++i) {
        cout << "0--->" << solution[i]->getClient()->getNum() << "--->";
    }
    cout << "0\n\n";	// A la fin, on revient chez le fournisseur
    cout << "Evaluation de cette solution : " << evalCurSol << "\n";
    cout << "______________________________________________________\n";
}

void Probleme2::printSol_noptr(vector<Batch> solution,float evalCurSol) {
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

//---------------------------------------
//          PARTIE INDO
//---------------------------------------

/* Troisième fonction de résolution, cette fois en bruteforce :
 * 	- Livrer un produit
 *	- Si ce produit peut aller avec un batch livré plus tard, le faire
 *	- Sinon, le mettre dans un batch à part. */

void Probleme2::solve_bruteforce(){

	//solutionHeuristique();

    vector<Batch> res(0);
    build_batches_bruteforce(res);
	vector<Batch> curSol(0);
    float curEval = 0;
    float curTime = 0;

    //cout<<"BATCHES SANS TRI : \n";
    //printBatchs(res);

    /* Tri des batches pour une meilleure efficacité
     * Il faudra se poser la question de savoir si c'est
     * vraiment plus efficace ou non*/

    sort(res.begin(), res.end(),
         Tools::comparatorBatchLength);

    solve_bruteforce(curSol, res,curTime, curEval);

}

/* Fait tous les batches possibles */
void Probleme2::build_batches_bruteforce(vector<Batch> &cur){

    int i;

    cout<<"Recherche de toutes les combinaisons de batches...\n";

    for(i=0;i<clients.size();++i){

        vector<Produit*> prodsClient = getProdsClient(i+1);
        //cout<<"Produits du client "<<(i+1)<<" :\n";
        //printProduits(prodsClient);

        /* Petit tri pour être sûrs que les produits sont bien ordonnés */
        sort(prodsClient.begin(), prodsClient.end(),
         Tools::comparatorProduitPtrDateDue);

        int k;
        int limit;

        if (capa > prodsClient.size()) limit = prodsClient.size();
        else limit = capa;

        for(k = 0;k<limit;++k){

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
                        //std::cout << (i+1) << " ";
                    }
                }
                //std::cout << "\n";

                /* On vérifie encore l'ordre dans la permutation, pour ne pas avoir des choses
                 * comme [0,2,1] qui feraient échouer isUseless */
                Tools::bubbleSort(ptr,r);

                if(!isUseless(ptr,r)){
                    combinations.push_back(ptr);
                }
            } while (std::next_permutation(v.begin(), v.end()));

            /* On a toutes les combinaisons sous formes d'indice, maintenant il faut les ajouter aux combinaisons
             * Des batches. */

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

            //printBatchs(cur);

        }
    }

    cout<<"Recherche terminee.\n";
}

/* On peut dire qu'une permutation est inutile si l'écart entre les indices 1 à 1 est != 1
 * On peut dire par parce qu'on sait qu'un indice de 3 représente un produit qui arrive
 * plus tard qu'un produit représenté par 1 ou 2. [1,3] est inutile à faire par qu'on sait qu'il
 * sera AU MOINS pire que [1,2].
 *
 * IL FAUDRA SECURISER CETTE FONCTION EN FAISANT DES TRIS REGULIERS POUR SAVOIr SI TOUT EST BIEN EN ORDRE*/
bool Probleme2::isUseless(int* permut, int r){

    int i;
    for(i=0;i<r-1;++i){
        if(permut[i+1]-permut[i] != 1){
            return true;
        }
    }

    return false;
}

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

/* Supprime ls champs vite d'un vecteur */
void Probleme2::removeEmptyFields(vector<Produit*> &ptr){

    vector<Produit*>::iterator it = ptr.begin();

    while(it != ptr.end()){

        if(*it == NULL){
            it = ptr.erase(it);
        } else {
            ++it;
        }
    }
}

/* Récupère tous les produits du client.
 * ATTENTION : les batches sont récupérés dans l'ordre croissant */
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

void Probleme2::solve_bruteforce(vector<Batch> curSol, vector<Batch> res,float curTime,float curEval){

    /* On n'a pas trouvé de truc à inclure : regarde si tous les produits sont bien présents */
    if(res.size() == 0){
                if(curEval < evalBestSol){
                    cout<<"Meilleure solution trouvee. On l'enregistre.\n";
                    reverse(curSol.begin(), curSol.end()); // on inverse avant de rendre la meilleure solution, puisqu'elle était inversée
                    bestSol_noptr = curSol;
                    evalBestSol = curEval;
                    printBestSol_indo_noptr();

                } else {
                    //cout<<"Pire solution, on oublie.\n";
                }
            return;
    }

	/* Il faut aussi évaluer la solution à chaque tour, pour voir si on peut cut ou pas */
    if(curSol.size() > 0){
        if(curEval > evalBestSol){
            //cout<<"Solution plus mauvaise : cut.\n\n"<<endl;
            return;
        }
    }

	/* Test de toutes les combinaisons de livraison récursivement PAR PRODUITS*/
    vector<Batch>::iterator it = res.begin();


    // Modifs belka : ajout d'un tri sur les batchs restants
    // Est-ce que c'est vraiment là qu'on le mets ?...
    //computeCoutsStockageCourants(res, curTime);
    //sort(res.begin(), res.end(), Tools::comparatorBatchCoutStockageCourant);

    while(it != res.end()){
		Batch tempBatch = *it;

        vector<Batch> newSol = curSol;

        newSol.push_back(tempBatch);
        vector<Batch> newRest = res;
        removeBatch(newRest, tempBatch);

        float newTime = curTime;
        float newEval = curEval;

        /* Calcul du temps et des coûts */
        if(newSol.size() == 1){
            newTime = newSol[0].getDateGlobale();
            newEval = newSol[0].coutStockage(newTime);
            newEval += newSol[0].getClient()->getDist()*2*eta;
            newTime -= newSol[0].getClient()->getDist();
        } else {
            int indice = newSol.size() - 1;
            newTime -= newSol[indice].getClient()->getDist();
            if(newTime > newSol[indice].getDateGlobale()){
                newTime = newSol[indice].getDateGlobale();
            }
            newEval += newSol[indice].getClient()->getDist()*2*eta;
            newEval += newSol[indice].coutStockage(newTime);
            newTime -= newSol[indice].getClient()->getDist();
        }

        solve_bruteforce(newSol,newRest,newTime,newEval);

        it++;
    }
}

/* test d'une nouvelle solution de résolution. Algo :
 * - créer les batch comme d'habitude. Si un batch se voit livré plus tôt que sa date due, reprendre les batchs aux alentours
 *          (avant ou après ??)
 * - et voir s'il est possible de le scinder (envoyer un bout avant  pour que le nouveau batch soit plus optimal
 *
 *          PREMIEIERE VERSION : LES BATCHES NE SE SCINDENT JAMAIS              */

void Probleme2::solve_indo(){

    solutionHeuristique();

    vector<Batch*> batches(0);
    vector<Produit*> res = produits;
    build_batches(batches,res);

    vector<Batch*> cursol(0);
    float curCost = 0;
    solve_indo(cursol, batches, curCost);

}

void Probleme2::build_batches(vector<Batch*> &cur, vector<Produit*> res){

    if(res.size() == 0){
        return;
    }

    /* On cherche à construire le nouveau batch en fonction du produit à la date due maximale dans res (à livrer au plus tard en théorie) */
    Produit* firstProd = produitDueMax(res);

    /* Le nouveau batch est construit */
    Batch* tmp = new Batch(firstProd);
    cur.push_back(tmp);

    /* Le nouveau produit ajouté au batch, on peut donc le supprimer de la liste */
    removeProduct(res,firstProd);

    /* On construit le batch à partir du premier produit dedans */
    int i = 0;

    while(tmp->getProduits().size() < capa && i<res.size()){
        if(res[i]->getClient()->getNum() == tmp->getProduits()[0]->getClient()->getNum()){
            /* Condition pour être ajouté au batch : Doit avec une date due <= le premier produit du batch - l'aller-retour */
            if(res[i]->dateDue() >= (tmp->getProduits()[0]->dateDue() - tmp->getProduits()[0]->getClient()->getDist()*2)){
                tmp->addProduit(res[i]);
                removeProduct(res,res[i]);
            } else {
                ++i; // On n'incrémente que si aucun produit n'a été supprimé
            }
        } else {
            ++i;
        }
    }

    tmp->printBatch();

    /* On continue à créer les batches et à les placer dans la timeline récursivement */
    build_batches(cur, res);
}

void Probleme2::solve_indo(vector<Batch*> curSol, vector<Batch*> resBatches,float curEval){

    if(resBatches.size() == 0){
        curEval = evaluerSolution_auto(curSol);
        printBatchs(curSol);
        printSol(curSol,curEval);

        if(curEval < evalBestSol){
            cout<<"Meilleure solution trouvee. On l'enregistre.\n";
            reverse(curSol.begin(), curSol.end()); // on inverse avant de rendre la meilleure solution, puisqu'elle était inversée
            bestSol = curSol;
            evalBestSol = curEval;
        } else {
            cout<<"Pire solution, on oublie.\n";
        }

        return;
    }

    if(curSol.size() > 0){
        curEval = evaluerSolution_auto(curSol);
        if(curEval > evalBestSol){
            cout<<"Solution plus mauvaise : cut.\n\n"<<endl;
            return;
        }
    }

    /* Test de toutes les combinaisons de livraison récursivement*/
    vector<Batch*>::iterator it = resBatches.begin();

    while(it != resBatches.end()){
        Batch* temp = *it;

        vector<Batch*> newSol = curSol;

        printBatchs(newSol);

        newSol.push_back(temp);
        vector<Batch*> newRest = resBatches;

        removeBatch(newRest,temp);
        solve_indo(newSol,newRest,curEval);
        ++it;
    }
}

/* Supprime un batch d'une liste de batches */
void Probleme2::removeBatch(vector<Batch*> &newRest,Batch* temp){

    vector<Batch*>::iterator it;

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
void Probleme2::removeBatch(vector<Batch> &newRest,Batch temp){

    vector<Batch>::iterator it;

    /* Première étape : on supprime temp */
    for(it=newRest.begin();it!=newRest.end();++it){
        if(&(*it) == &temp){
            newRest.erase(it);
            break;
        }
    }

    /* Seconde étatpe : on supprime les batches contenant  un ou plusieurs produits
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
   /!\ CONTRAIREMENT A EVALUERSOLUTION, CETTE EVALUATION SE FAIT EN BACKTRACK /!\ */
float Probleme2::evaluerSolution_auto(vector<Batch*> s) {

    float curTime = 0;
    cout << "Evaluation solution"<<endl;
    cout << "Detail de la solution :\n";

    float ev = 0;
    /* La date de départ est la date de livraison du dernier batch + le temps de retour a l'entrepôt */
    curTime = s[0]->dateDueGlobale() + s[0]->getClient()->getDist();
    for (int i = 0; i < s.size(); ++i) {

        ev += s[i]->getClient()->getDist() *2*eta; /* On fait payer l'aler-retour */

        curTime -= s[i]->getClient()->getDist();

        /* Si la date de livraison est trop tôt pour le client, il faut attendre à l'entrepôt pour ne pas l'amener trop tôt */
        if(curTime > s[i]->dateDueGlobale()){
            curTime = s[i]->dateDueGlobale();
        }

        float tempCoutStock = s[i]->coutStockage(curTime);
        s[i]->setDate_livraison(curTime);
        ev += tempCoutStock;
        curTime -= s[i]->getClient()->getDist();
    }
    return ev;
}

/* Pour la meilleure solution, renseigne les dates d'arrivées des meilleurs batches,
 * bestSol est à l'endroit on doit donc la reverse pour bracktrack ici.*/
void Probleme2::setDates_livraison_bestSol(){
    vector<Batch*> tempBestSol = bestSol;
    reverse(tempBestSol.begin(),tempBestSol.end());
    float curTime = tempBestSol[0]->dateDueGlobale() + tempBestSol[0]->getClient()->getDist(); // départ du temps à l'entrepot, à la fin
    for (int i = 0; i < tempBestSol.size(); ++i) {

        curTime -= tempBestSol[i]->getClient()->getDist();

        if(curTime > tempBestSol[i]->dateDueGlobale()){
            curTime = tempBestSol[i]->dateDueGlobale();
        }

        tempBestSol[i]->setDate_livraison(curTime);

        curTime -= tempBestSol[i]->getClient()->getDist();
    }
}

void Probleme2::setDates_livraison_bestSol_noptr(){
    vector<Batch> &tempBestSol = bestSol_noptr;
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
    vector<Batch*> cur(0);
    vector<Produit*> res = produits;
    solve(cur, res);

//    for (int i = 0; i < bestSol.size(); ++i) {
//        bestSol[i]->printBatch();
//    }

//    sort(bestSol.begin(), bestSol.end(),
//         Tools::comparatorBatchPtrCoutStockageCourant);
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
                cout << "Même client" << endl;
                if (dateCourante - res[i]->dateDue() >                /// la première fois qu'on passe ici, dateCourante = 0 ?
                                    res[i]->getClient()->getDist()) { /// getDist*2 ?
                    // On ne doit pas l'ajouter, mais on ferait bien
                    // de vérifier si on peut, amélioration...
                    // Stocker revient moins cher qu'un AR ?
                    cout << "VRAI1";
                    if (livraisonImmediate(res[i])) {
                        cout << "VRAI";
                        tmp->addProduit(res[i]);
                        res.erase(res.begin()+i);
                    } else {
                        b = false;
                    }
                    cout << endl;
                } else {
                    // avant de l'ajouter, vérifier si cela vaut le coût...
                    tmp->addProduit(res[i]);
                    res.erase(res.begin()+i);
                }
            } else cout << "Pas même client" << endl;
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
    evalBestSol = 999999999;
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
    /* rep est le coût de livraison + stockage du batch */
	float rep = b->getClient()->getDist() * 2*eta;
    // cout << "Cout de livraison client " << b->getClient()->getNum();
    // cout << " : " << rep << endl;

	dateCourante += b->getClient()->getDist();
	rep += b->coutStockage(dateCourante);

	cout<<"Batch pour client "<<b->getProduits()[0]->getClient()->getNum()<<", Date : "<<dateCourante<<"/"<<b->dateDueGlobale()<<endl;

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
