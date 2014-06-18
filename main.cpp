#include "Probleme2.h"

int main(int argc, char** argv) {

    cout << "----- Hello World! -----" << endl;

    Probleme2* p = NULL;

    if (argc == 2) {
        p = new Probleme2(argv[1]);
    } else {
        p = new Probleme2("instances/instance10n3cl.txt");
        // p = new Probleme2();
    }

    // Initialisation du chrono
    clock_t debut, fin;
    double duree;
    debut = clock();

    p->solve_indo();
    p->printBestSol_indo();
    p->solve_bruteforce();

    // Arrêt et affichage du chrono
    fin = clock();
    duree = (fin - debut) / (double) CLOCKS_PER_SEC;
    printf("Durée de calcul = %.2lf sec\n", duree);

    delete(p);
    cout << "------ Bye World! ------" << endl;
    return 0;
}


