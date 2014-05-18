#ifndef __CLIENT__
#define __CLIENT__

class Client {
	private:
		int h;				// N° du client
		int cout_u;			// Coût de stockage unitaire chez le client h
		bool fini;			// Est-ce que le client a fini d'être livré ?
		// Temps qu'on mets à arriver à ce client ?

	public:
		Client(int, int);
		int getCoutUnitaireStockage();
};

#endif // __CLIENT__