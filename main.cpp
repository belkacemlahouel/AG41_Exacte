#include "Probleme.h"
#include "Parser.h"

int main(int argc, char** argv) {
	if (argc == 2) {
		cout << "----- Hello World! -----" << endl;
		Parser r(argv[1]);

		// Probleme p;
		Probleme p(r.getCapa(), r.getEta(),
					r.getClients(), r.getProduits());

		// ou : on peut envoyer en paramètre le fichier
		// qu'un parseur dans Probleme lira

		p.printBatchs();
		p.solve();
		p.printBestSol();
		cout << "------ Bye World! ------" << endl;
		return 0;
	} else {
		cout << "Mauvais paramètres - ERREUR" << endl;
	}
}


