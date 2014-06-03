#include "Parser.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
	cout << "______________________________________________________" << endl;

    Parser parser = Parser();
    parser.parseFile("instance15n3cl.txt");

	//Probleme p = Probleme();

	cout << "end of challenge... thank you." << endl;

	return 0;
}
