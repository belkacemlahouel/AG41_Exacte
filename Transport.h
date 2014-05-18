#ifndef __TRANSPORT__
#define __TRANSPORT__

using namespace std;

#include <iostream>
#include <vector>

class Transport {
	private:
		int c;			// Capacité du transporteur <=> Nombre de produits max
		int tij[][];	// Temps de transport entre le client i et j, 0..m
		int eta;		// Coef pour le coût de transport

	public:
		Transport();
};

#endif // __TRANSPORT__