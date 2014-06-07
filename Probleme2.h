#ifndef __PROBLEME_2__
#define __PROBLEME_2__

#include "Tools.h"
#include "Parser.h"
#include "Batch.h"
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
    
    vector<Batch*> bestSol;			// Meilleure solution
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
    
public:
    Probleme2();
    Probleme2(string filename);
    ~Probleme2();
    
    void printBatchs();
    void printProduits();
    void printBestSol();
    void printSol(int niveau);
    
    void solve();
};

#endif // __PROBLEME__
