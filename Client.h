#ifndef __CLIENT__
#define __CLIENT__

class Client {
	private:
		int h;				// N° du client
		int dist;			// Distance entrepôt/client (en unité de temps)
		float cout_u;			// Coût de stockage unitaire chez le client h
		bool fini;			// Est-ce que le client a fini d'être livré ?

	public:
		Client(int, int, int);
		int getCoutUnitaireStockage();
		int getDist();
		int getNum();
};

#endif // __CLIENT__
