#ifndef __PROBLEME_2__
#define __PROBLEME_2__

#include "Parser.h"
#include <ctime>
#include <algorithm>

using namespace std;

class Probleme2 {
private:

    int capa;						// Capacité du transporteur
    float eta;						// Coefficient pour les coûts de transport
    vector<Client*> clients;		// Liste des clients
    vector<Produit*> produits;		// Liste des produits
    vector<Batch> bestSol;			// Meilleure solution
    float evalBestSol;				// + Evaluation

    /* Fonctions d'initialisation du problème*/
    void init(int capa, float eta, vector<Client*>, vector<Produit*>);

    /* Fonctions d'affichage */
    void printBatchs(vector<Batch> blist);
    void printCombinations(vector<int*> permuts, int r);
    void printSol(vector<Batch> solution,float evalCurSol);
    void printProduits(vector<Produit*> plist);

    /* Fonctions utilisées pour l'heuristique */
    void heuristique(vector<Batch> curSol, vector<Batch> resBatches,float curEval);
    float evaluerSolution_auto(vector<Batch>);
    void build_batches(vector<Batch> &cur, vector<Produit*> res);

    /* Fonctions de manipulation de listes */
    void removeProduct(vector<Produit*> &plist, Produit* p);
    void removeBatch(vector<Batch> &newRest,Batch temp);
    void removeBatchAndDoubles(vector<Batch> &newRest,Batch temp);

    /* Fonctions nécessaires à la construction des batches pour la résolution exacte*/
    void build_batches_bruteforce(vector<Batch> &cur);
    vector<Produit*> getProdsClient(int num);
    bool isUseless(int* permutList, int r);

    /* Fonctions nécessaires à la méthode de résoution exacte */
    void solve(vector<Batch> curSol, vector<Batch> res,float curTime,float curEval);
    bool sameProducts(Batch batch1,Batch batch2);

        /* Cut dans l'arbre : calcul de coupMini*/
        void computeCoutsStockageCourants(vector<Batch>, float);
        void computeCoutsStockageCourantsInit(vector<Batch>);
        void computeCoutMini();
        float coutMini;
        int* nbBatchsMini;
        int* nbBatchsUsed;

    /* Autres outils */
    void setDates_livraison_bestSol();
    Produit* produitDueMax(vector<Produit* > plist);


public:
    Probleme2();
    Probleme2(string filename);
    ~Probleme2();

    void printProduits();

    void printBestSol();

    void heuristique();
    void solve();
};

#endif // __PROBLEME__
