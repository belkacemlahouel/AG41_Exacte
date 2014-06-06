#ifndef __PROBLEME__
#define __PROBLEME__

#include "Tools.h"
#include "Batch.h"
#include <algorithm>

using namespace std;

class Probleme {
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

		// --- Méthodes privées
		void solve(int iter, vector<Batch*> reste);
		// template<class T> void viderVector(vector<T>); // --> tools
        void solutionHeuristique();

	public:
		Probleme(int capa, float eta, vector<Client*>, vector<Produit*>);
		Probleme();
		~Probleme();

		void buildBatchs();
		void printBatchs();
		void printProduits();
		void printBestSol();
		void printSol(int niveau);

        bool encorePossible(vector<Batch*> reste);
        void solve();

        float livraison(Batch*);		// Renvoient le coût de cette livraison
        float annulerLivraison(Batch*); // et avancent (ou reculent) la date
};

#endif // __PROBLEME__
