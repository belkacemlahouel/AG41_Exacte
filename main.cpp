#include "Parser.h"

using namespace std;

int main(int argc, char** argv) {
	cout << "______________________________________________________" << endl;

    Parser parser = Parser("instance15n3cl.txt");

	Probleme p = Probleme();

	parser.initialiser_Probleme(p);

	cout << "end of challenge... thank you." << endl;

	return 0;
}
