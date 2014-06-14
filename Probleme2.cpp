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

    //evalBestSol = 2320;

    vector<Batch> res(0);
    build_batches_bruteforce(res);
	vector<Batch> curSol(0);
    float curEval = 0;
	float curTime = 0; // On met à 0 pour l'initialiser à quelque chose, mais en réalité il sera instancié réellemet dans la résolution

    //solve_bruteforce(curSol, res, curTime, curEval);

}

/* Fait tous les batches possibles */
void Probleme2::build_batches_bruteforce(vector<Batch> &cur){

    int i;

    cout<<"Recherche de toutes les combinaisons de batches...\n";

    for(i=0;i<clients.size();++i){

        vector<Produit*> prodsClient = getProdsClient(i+1);
        //cout<<"Produits du client "<<(i+1)<<" :\n";
        //printProduits(prodsClient);

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
                combinations.push_back(ptr);
            } while (std::next_permutation(v.begin(), v.end()));

            /* On a toutes les combinaisons sous forces d'indice, maintenant il faut les ajouter aux combinaisons
             * Des batches. */

            //for(j=0;j<)

            printCombinations(combinations,r);
        }
    }

    cout<<"Recherche terminee.\n";
    printCombinations(cur);
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

void Probleme2::printCombinations(vector<Batch> combs){

    int i,j;

    for(j=0;j<combs.size();++j){
        printBatchs(combs);
    }
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

void Probleme2::solve_bruteforce(vector<Batch> curSol, vector<Produit*> res, float curTime, float curEval){

	/* Tous les produits sont inclus dans la solution : on peut la regarder */
	if(res.size() == 0){
        curEval = evaluerSolution_auto_noptr(curSol);

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

	/* Mais il faut aussi évaluer la solution à chaque tour, pour voir si on peut cut ou pas */
    if(curSol.size() > 0){
        curEval = evaluerSolution_auto_noptr(curSol);
        if(curEval > evalBestSol){
            //cout<<"Solution plus mauvaise : cut.\n\n"<<endl;
            return;
        }
    }

	/* Test de toutes les combinaisons de livraison récursivement PAR PRODUITS*/
    vector<Produit*>::iterator it = res.begin();

    while(it != res.end()){
		Produit* tempProd = *it;
        int toAdd = 0;
		/* Ici, il faut regarder s'il y a moyen de faire une fusion ou non !! */

        vector<Batch> newSol = curSol;

        //printBatchs(newSol);

        /* ICI, APRES AVOIR TROUVE LA PREMIERE SOLUTION, NEWSOL DEVRAIT ETRE VIDE !! */

		if(newSol.size() == 0){
            /* C'est le premier batch à livrer, donc on cale la date courante en fonction de lui */
            toAdd = 1;
		} else {
            /* On regarde si le batch envoyé plus tard (enfin avant, dans la solCur...)
             * est est du même client que le produit qu'on vient de tirer dans res */

             if(newSol[newSol.size()-1].getClient() == tempProd->getClient()){

                /* A partir de là, il y a plusieurs choix :
                    - en réalité (pour une vraie bruteforce), on devrait récurser sur les
                        deux possibilités (fusion avec le batch ou pas)
                    - pour accélérer les choses, pour le moment on l'ajoute juste au batch précédent */

                if(newSol[newSol.size()-1].getProduits().size() < capa){ // on regarde s'il reste de la place dans le batch
                    newSol[newSol.size()-1].addProduit(tempProd);

                } else { // sinon on en créé un nouveau
                    toAdd = 1;
                }
             } else { // si ce n'est pas le même client, idem, on créé un batch
                toAdd = 1;
             }
		}

		if(toAdd){
            Batch temp = Batch(tempProd);
            newSol.push_back(temp);
		}

        vector<Produit*> newRest = res;

        removeProduct(newRest,tempProd);

        /* Il faut rajouter ici le décallage de temps */

        solve_bruteforce(newSol,newRest,curTime,curEval);
        ++it;
    }
}


/* test d'une nouvelle solution de résolution. Algo :
 * - créer les batch comme d'habitude. Si un batch se voit livré plus tôt que sa date due, reprendre les batchs aux alentours
 *          (avant ou après ??)
 * - et voir s'il est possible de le scinder (envoyer un bout avant  pour que le nouveau batch soit plus optimal
 *
 *          PREMIEIERE VERSION : LES BATCHES NE SE SCINDENT JAMAIS
 *                                                        */

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

    /* PB ICI : ON A PARFOIS UN BATCH DE 7 !! */

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
        cout<<"Batch pour client "<<s[i]->getProduits()[0]->getClient()->getNum()<<", Cout stockage :  "<<tempCoutStock<<"\n\tDate : "<<curTime<<"/"<<s[i]->dateDueGlobale()<<endl;

        curTime -= s[i]->getClient()->getDist();
    }

    vector<Batch*> affichageSol = s;
    reverse(affichageSol.begin(), affichageSol.end());
    printSol(affichageSol,ev);

    return ev;
}

/* Contrairement à evaluerSolution, cette fonction va évaluer la solution donnée en reconstruisant
   lui même la timeline de livraison à partir d'une liste de batch. Elle n'a donc pas besoin de dateCourante
   /!\ CONTRAIREMENT A EVALUERSOLUTION, CETTE EVALUATION SE FAIT EN BACKTRACK /!\ */
float Probleme2::evaluerSolution_auto_noptr(vector<Batch> s) {

    float curTime = 0;
    //cout << "Evaluation solution"<<endl;
    //cout << "Detail de la solution :\n";

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
        //cout<<"Batch pour client "<<s[i].getProduits()[0]->getClient()->getNum()<<", Cout stockage :  "<<tempCoutStock<<"\n\tDate : "<<curTime<<"/"<<s[i].dateDueGlobale()<<endl;

        curTime -= s[i].getClient()->getDist();
    }

    vector<Batch> affichageSol = s;
    reverse(affichageSol.begin(), affichageSol.end());
    //printSol_noptr(affichageSol,ev);

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
    evalBestSol = 99999999999;
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









