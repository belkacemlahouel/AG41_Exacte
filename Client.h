#ifndef __CLIENT__
#define __CLIENT__

class Client {
	private:
		int h;				// N° du client
		float dist;			// Distance entrepôt <-> client
		float cout;			// Coût de stockage unitaire chez le client #h

	public:
		Client(int _h, float _dist, float _cout);
		~Client(){};
		float coutUnitaireStockage();
		float getDist();
		int getNum();
};

#endif // __CLIENT__