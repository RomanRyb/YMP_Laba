#include "HashTable.h"
#include "Lexical.h"
#include "Syntactic.h"
#include "Semantic.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	lt.START();
	lt.print();
	if (lt.countfail != 0) return 0;
	Sip sp;
	if (!sp.fl)return 0;
	sp.print();
	Sema sem;
	sem.start(sp.root);
	sem.print();
}