#ifndef __PROBLEME_2__
#define __PROBLEME_2__

#include "Parser.h"
#include <ctime>
#include <algorithm>

using namespace std;

class Probleme2 {
private:
    // --- Input du problème
    int capa;						// Capacité du transporteur
    float eta;						// Coefficient pour les coûts de transport
    vector<Client*> clients;		// Liste des clients
    vector<Produit*> produits;		// Liste des produits

    vector<Batch> bestSol;			// Meilleure solution (l'ordre des batches est le bon

    float evalBestSol;				// + Evaluation

    void init(int capa, float eta, vector<Client*>, vector<Produit*>);
    void computeCoutsStockageCourants(vector<Batch>, float);
    void computeCoutsStockageCourantsInit(vector<Batch>);
    void computeCoutMini();
    float coutMini;
    void heuristique4000(vector<Produit*>);
    int* nbBatchsMini;
    int* nbBatchsUsed;


    /* Fonctions pour solution by indo : */

    void printBatchs(vector<Batch*> blist);
    void printProduits(vector<Produit*> plist);
    void printSol(vector<Batch> solution,float evalCurSol);
    void printSol_noptr(vector<Batch> solution,float evalCurSol);
    void setDates_livraison_bestSol();
    void build_batches(vector<Batch> &cur, vector<Produit*> res);
    void heuristique(vector<Batch> curSol, vector<Batch> resBatches,float curEval);
    Produit* produitDueMax(vector<Produit* > plist);
    float evaluerSolution_auto(vector<Batch>);
    void removeProduct(vector<Produit*> &plist, Produit* p);
    void removeBatch(vector<Batch> &newRest,Batch temp);
    void removeBatchAndDoubles(vector<Batch> &newRest,Batch temp);
    void build_batches_bruteforce(vector<Batch> &cur);
    vector<Produit*> getProdsClient(int num);
    void removeEmptyFields(vector<Produit*> &ptr);
    void solve(vector<Batch> curSol, vector<Batch> res,float curTime,float curEval);
    bool alreadyInSol(Batch batch,vector<Batch> sol);
    bool allProdsInSol(vector<Batch> sol);
    bool sameProducts(Batch batch1,Batch batch2);
    void printBatchs(vector<Batch> blist);
    void printCombinations(vector<int*> permuts, int r);
    bool isUseless(int* permutList, int r);

public:
    Probleme2();
    Probleme2(string filename);
    ~Probleme2();

    void printBatchs();
    void printProduits();

    void printBestSol();

    void heuristique();
    void solve();
};

#endif // __PROBLEME__
