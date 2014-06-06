#include <vector>
#include <algorithm>
#include "Nono.h"

using namespace std;

bool comparator(const Nono &n1, const Nono &n2) {
	cout << "comparator &" << endl;
	return n1.num < n2.num;
}

bool comparatorptr(Nono* n1, Nono* n2) {
	cout << "comparator *" << endl;
	return n1->num < n2->num;
}

int main(int argc, char** argv) {
	cout << endl;

	vector<Nono*> nonos;
	for (int i = 0; i < 5; ++i)
		nonos.push_back((new Nono(i+1)));

	for (int i = 0; i < 5; ++i)
		nonos[i]->print();
	cout << endl;

	// --------------------------------
	sort(nonos.begin(), nonos.end(), comparatorptr);
	// --------------------------------

	for (int i = 0; i < 5; ++i)
		nonos[i]->print();
	cout << endl;

	cout << endl;
	cout << endl;

	// Test opérateur seul
	nonos[4]->print();
	if (nonos[4] < nonos[0]) {
		cout << " < ";
	} else {
		cout << " > ";
	}
	nonos[0]->print();

	return 0;
}

