#include<iostream>
#include<fstream>
#include<string>
#include"Hesh.h"
#include"Lec.h"
#include"Trop.h"

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