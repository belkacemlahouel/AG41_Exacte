#ifndef __PROBLEME_2__
#define __PROBLEME_2__

#include "Parser.h"
#include <algorithm>

using namespace std;

class Probleme2 {
private:
    // --- Input du problème
    int capa;						// Capacité du transporteur
    float eta;						// Coefficient pour les coûts de transport
    vector<Client*> clients;		// Liste des clients
    vector<Produit*> produits;		// Liste des produits

    // --- Recherche de la meilleure solution
    vector<Batch*> batchs;

    vector<Batch*> bestSol;			// Meilleure solution (l'ordre des batches est le bon
    vector<Batch> bestSol_noptr;			// Meilleure solution (l'ordre des batches est le bon

    float evalBestSol;				// + Evaluation
    vector<Batch*> sol;				// Solution en cours
    float evalSol;					// + Evaluation

    float dateCourante;				// Date courante lors du calcul

    // --- Méthodes privées, résolution principalement
    // void solve(int iter, vector<Batch*> reste);
    void solutionHeuristique();
    void init(int capa, float eta, vector<Client*>, vector<Produit*>);
    bool encorePossible(vector<Batch*> reste);
    float livraison(Batch*);		// Renvoient le coût de cette livraison
    float annulerLivraison(Batch*); // et avancent (ou reculent) la date
    // void buildBatchs();
    void solve(vector<Batch*> cur, vector<Produit*> reste);
    float evaluerSolution(vector<Batch*>);
    bool livraisonImmediate(Produit*);


    /* Fonctions pour solution by indo : */

    void printBatchs(vector<Batch*> blist);
    void printProduits(vector<Produit*> plist);
    void printSol(vector<Batch*> solution,float evalCurSol);
    void printSol_noptr(vector<Batch> solution,float evalCurSol);
    void setDates_livraison_bestSol();
    void setDates_livraison_bestSol_noptr();
    void build_batches(vector<Batch*> &cur, vector<Produit*> res);
    void solve_indo(vector<Batch*> curSol, vector<Batch*> resBatches,float curEval);
    Produit* produitDueMax(vector<Produit* > plist);
    float evaluerSolution_auto(vector<Batch*>);
    float evaluerSolution_auto_noptr(vector<Batch> s);
    void removeProduct(vector<Produit*> &plist, Produit* p);
    void removeBatch(vector<Batch*> &newRest,Batch* temp);
    void build_batches_bruteforce(vector<Batch> &cur);
    vector<Produit*> getProdsClient(int num);
    void printCombinations(vector<Batch> combs);
    void printCombinations(vector<int*> combs,int r);
    void removeEmptyFields(vector<Produit*> &ptr);
    void solve_bruteforce(vector<Batch> curSol, vector<Produit*> res, float curTime, float curEval);

public:
    Probleme2();
    Probleme2(string filename);
    ~Probleme2();

    void printBatchs();
    void printBatchs(vector<Batch> blist);
    void printProduits();

    void printBestSol();
    void printBestSol_indo();
    void printBestSol_indo_noptr();
    void printSol(int niveau);

    void solve();
    void solve_indo();
    void solve_bruteforce();
};

#endif // __PROBLEME__
