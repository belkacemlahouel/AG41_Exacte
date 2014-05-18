#ifndef __TRANSPORT__
#define __TRANSPORT__

class Transport {
	private:
		int capa;		// Capacité du transporteur <=> Nombre de produits max
		int** tps;		// Temps de transport entre le client i et j, 0..m
		int coef;		// Coef pour le coût de transport

	public:
		Transport(int, int, int**);
};

#endif // __TRANSPORT__