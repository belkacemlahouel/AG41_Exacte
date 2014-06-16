#include "Probleme2.h"

int main(int argc, char** argv) {

    cout << "----- Hello World! -----" << endl;

    Probleme2* p = NULL;

    if (argc == 2) {
        p = new Probleme2(argv[1]);
    } else {
        // p = new Probleme2("instances/instance15n2cl.txt");
        p = new Probleme2();
    }

    p->solve();

    // p->printBatchs();
    // // p->solve();
    // p->solve_indo();
    // //p->printBestSol_indo();
    // p->solve_bruteforce();
    // // p->printBestSol();
    // p->printBestSol_indo_noptr();

    delete(p);
    cout << "------ Bye World! ------" << endl;
    return 0;
}


