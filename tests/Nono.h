#ifndef __NONO__
#define __NONO__

#include <iostream>

using namespace std;

class Nono {
	private:
		// int num;

	public:
		int num;

		Nono(int _num) : num(_num) {};

		~Nono(){};

		int getNum() {
			return num;
		};

		void print() {
			cout << "Nono : " << num << endl;
		};

		bool operator<(Nono& n) {
			return getNum() < n.getNum();
		};
};

#endif // __NONO__