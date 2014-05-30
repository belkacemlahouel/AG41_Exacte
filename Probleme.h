#ifndef __PROBLEME__
#define __PROBLEME__

#include "Batch.h"

using namespace std;

class Probleme {
	private:
		int m; 							// Nombre de clients
		int n; 							// Nombre de produits total
		int c;							// Capacité du transporteur
		int eta;						// Coefficient pour les coûts de transport
		int* nh;						// Nombre de produits/client
		vector<Client*> clients;		// Liste des clients
		vector<Produit*> produits;		// Liste des produits
		vector<Batch*> batches;

		vector<int> bestSol;            // La meilleure solution trouvée jusqu'à présent
		float eval_bestSol;             // L'évaluation de la meilleure solution (bestSol)

	public:
		Probleme(int, int, int*, vector<Client*>, vector<Produit*>);
		Probleme();
		~Probleme(){}
		void build_batches();
		void printBatches();
		void eraseProduit(vector<Produit*> &prods, Produit* p);
        Produit* produitDueMin(vector<Produit*> &prods);
        Produit* produitDueMinClient(vector<Produit*> prods, Client* c);
        float solution_heuristique();
        Batch* batchDueMin(vector<Batch*> &bs);
        float batchCost(Batch* b, int &time);
        void eraseBatch(vector<Batch*> &bs, Batch* b);
        void printBestSol();
        Batch* batchEvalCoutMin(vector<Batch*> &bs);
        bool estCompromettant(vector<Batch*> &bs,Batch* b, int time);
};

#endif // __PROBLEME__
