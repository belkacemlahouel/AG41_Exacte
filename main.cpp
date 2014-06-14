#include "Probleme2.h"

int main(int argc, char** argv) {
//	if (argc == 2) {
//		cout << "----- Hello World! -----" << endl;
//		Parser r(argv[1]);
//
//		// Probleme p;
//		Probleme p(r.getCapa(), r.getEta(),
//					r.getClients(), r.getProduits());
//
//		// ou : on peut envoyer en paramètre le fichier
//		// qu'un Parseur dans Probleme lira
//        // Pour la gestion de la mémoire (delete)
//        // cela paraît plus normal...
//
//		p.printBatchs();
//		p.solve();
//		p.printBestSol();
//		cout << "------ Bye World! ------" << endl;
//		return 0;
//	} else {
//		cout << "Mauvais paramètres - ERREUR" << endl;
//	}

    cout << "----- Hello World! -----" << endl;

    Probleme2* p = NULL;

    if (argc == 2) {
        p = new Probleme2(argv[1]);
    } else {
        p = new Probleme2("instances/instance15n2cl.txt");
        // p = new Probleme2();
    }

    p->printBatchs();
    // p->solve();
    p->solve_indo();
    p->printBestSol_indo();
    p->solve_bruteforce();
    // p->printBestSol();
    p->printBestSol_indo_noptr();

    delete(p);
    cout << "------ Bye World! ------" << endl;
    return 0;
}


