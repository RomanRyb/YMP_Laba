#pragma once
#include "Lexical.h"
#include<vector>
#include<set>
#include<map>
#include<deque>

using namespace std;

struct Node {
	string s;
	deque<Node*> arr;
	int col;
	int val = 0;
	bool fl = true;
	Node() {
		col = -1;
	}
	Node(string s, deque<Node*> arr, int col = -1) :s(s), arr(arr), col(col) {

	}
	void add(Node* el) {
		arr.push_back(el);
	}

};


struct Sip {
	bool fl = true;
	int id = 0;
	string errro;
	Node* root;
	Sip() {
		wor.push_back("*");
		root = new Node("Program", { new Node("Begin", {}) ,new Node("Descriptions", {}),new Node("Operators", {}),new Node("End", {}) });
		begin(root->arr[0]);
		if (fl)desccription(root->arr[1]);
		if (fl) operators(root->arr[2]);
		if (fl) end(root->arr[3]);
	}
	void errore(vector<string> a) {
		if (wor[id] != "*") {
			lt.writeF("Ошибка в строке " + to_string(lin[id].first) + " позиция " + to_string(lin[id].second) + ":\n");
			lt.writeF("Введенно: " + wor[id] + "\nОжидаемое: ");
			for (int i = 0; i < a.size(); ++i) {
				if (i + 1 < a.size()) lt.writeF(a[i] + ", ");
				else lt.writeF(a[i] + '\n');
			}
		}
		else lt.writeF("Нету кода\n");
	}
	//---------------------------------------------------------------------------------
	void begin(Node*& el) {
		if (wor[id] == "PROGRAM") el->add(new Node(wor[id], {}));
		else {
			errore({ "PROGRAM" });
			fl = false;
			return;
		}
		++id;
		Node* nw = NULL;
		Id_n(nw, wor[id]);
		if (nw == NULL) {
			errore({ "id_name" });
			fl = false;
			return;
		}
		el->add(nw);
		++id;
	}
	//---------------------------------------------------------------------------------
	void end(Node*& el) {
		if (wor[id] == "END") el->add(new Node(wor[id], {}));
		else {
			errore({ "END","IF","id_name"});
			fl = false;
			return;
		}
		++id;
		Node* nw = NULL;
		Id_n(nw, wor[id]);
		if (nw == NULL) {
			errore({ "id_name" });
			fl = false;
			return;
		}
		el->add(nw);
		++id;
		if (wor[id] != "*") {
			errore({ "Конец кода" });
			fl = false;
			return;
		}
	}
	//---------------------------------------------------------------------------------
	void Id_n(Node*& el,string s) {
		if (table[s].name == "ID")el= new Node("Id", { new Node(s,{}) });
	}
	void Const_n(Node*& el, string s) {
		if (table[s].name == "CONST")el = new Node("Const", { new Node(s,{}) });
	}
	void Type_n(Node*& el, string s) {
		if (s == "INTEGER")el = new Node("Type", { new Node(s,{}) });
	}
	//---------------------------------------------------------------------------------
	void desccription(Node*& el) {
		Node* nw=NULL;
		Type_n(nw, wor[id]);
		if (nw == NULL) {
			errore({ "INTEGER" });
			fl = false;
			return;
		}
		Node* descr = new Node("Descr", { nw });
		++id;
		varlist(descr);
		if (!fl) return;
		el->add(descr);
		nw = NULL;
		Type_n(nw, wor[id]);
		if (nw == NULL) return;
		Node * descr2= new Node("Descriptions", {});
		desccription(descr2);
		if (!fl) return;
		el->add(descr2);

	}
	void varlist(Node*& el) {
		Node* nw = NULL;
		Id_n(nw, wor[id]);
		if (nw == NULL) {
			errore({ "id_name" });
			fl = false;
			return;
		}
		++id;
		Node* varl = new Node("VarList", { nw });
		el->add(varl);
		if (wor[id] == ",") varl->add(new Node(wor[id], {}));
		else return;
		++id;
		varlist(varl);
	}
	//---------------------------------------------------------------------------------
	void operators(Node*& el) {
		Node* nw = NULL;
		Id_n(nw, wor[id]);
		if (nw != NULL) {
			++id;
			if (wor[id] != "=") {
				errore({ "=" });
				fl = false;
				return;
			}
			Node* opa = new Node("Op", { nw,new Node(wor[id],{}),new Node("Expr",{})});
			++id;
			expr(opa->arr[2]);
			if (!fl) return;
			el->add(opa);
			nw = NULL;
			Id_n(nw, wor[id]);
			Node* opa2 = new Node("Operators", {});
			if (nw != NULL) {
				operators(opa2);
				if (!fl) return;
				el->add(opa2);
			}
			else if (wor[id] == "IF") {
				operators(opa2);
				if (!fl) return;
				el->add(opa2);
			}
			return;
		}
		if (wor[id] == "IF") {
			Node* opa = new Node("Op", { new Node(wor[id],{})});
			++id;
			Node* con = new Node("Condition", {});
			condition(con);
			if (!fl) return;
			opa->add(con);
			if (wor[id] != "THEN") {
				fl = false;
				errore({ "THEN" });
				return;
			}
			opa->add(new Node(wor[id], {}));
			++id;
			Node* nw1 = new Node("Operators", {});
			operators(nw1);
			if (!fl) return;
			opa->add(nw1);
			if (wor[id] != "ELSE") {
				if (wor[id] != "END") {
					fl = false;
					errore({ "END" });
					return;
				}
				opa->add(new Node(wor[id], {}));
				++id;
				if (wor[id] != "IF") {
					fl = false;
					errore({ "IF" });
					return;
				}
				opa->add(new Node(wor[id], {}));
				el->add(opa);
				++id;
				nw = NULL;
				Id_n(nw, wor[id]);
				Node* opa2 = new Node("Operators", {});
				if (nw != NULL) {
					operators(opa2);
					if (!fl) return;
					el->add(opa2);
				}
				else if (wor[id] == "IF") {
					operators(opa2);
					if (!fl) return;
					el->add(opa2);
				}
				return;
			}
			opa->add(new Node(wor[id], {}));
			++id;
			nw1 = new Node("Operators", {});
			operators(nw1);
			if (!fl) return;
			opa->add(nw1);
			if (wor[id] != "END") {
				fl = false;
				errore({ "END" });
				return;
			}
			opa->add(new Node(wor[id], {}));
			++id;
			if (wor[id] != "IF") {
				fl = false;
				errore({ "IF" });
				return;
			}
			opa->add(new Node(wor[id], {}));
			el->add(opa);
			++id;
			nw = NULL;
			Id_n(nw, wor[id]);
			Node* opa2 = new Node("Operators", {});
			if (nw != NULL) {
				operators(opa2);
				if (!fl) return;
				el->add(opa2);
			}
			else if (wor[id] == "IF") {
				operators(opa2);
				if (!fl) return;
				el->add(opa2);
			}
			return;
		}



		errore({ "id_name","IF" });
		fl = false;
	}
	void condition(Node*& el) {
		Node* nw = new Node("Expr", {});
		expr(nw);
		if (!fl) return;
		el->add(nw);
		if (table[wor[id]].name != "ROP") {
			fl = false;
			errore({ "RelationOperators" });
			return;
		}
		el->add(new Node("RelationOperators", { new Node(wor[id],{}) }));
		++id;
		nw = new Node("Expr", {});
		expr(nw);
		if (!fl) return;
		el->add(nw);
	}

	void simpleExper(Node*& el,const string& s) {
		Node* nw = NULL;
		Id_n(nw, s);
		Const_n(nw, s);
		if (nw != NULL) el = new Node("SimpleExpr", { nw });

	}
	void expr(Node*& el) {
		Node* nw = NULL;
		simpleExper(nw, wor[id]);
		if (nw != NULL) {
			++id;
			if (wor[id] == "+" or wor[id] == "-") {
				Node* privet = new Node("Expr", {});
				el->add(nw);
				el->add(new Node(wor[id],{}));
				++id;
				expr(privet);
				if (!fl) return;
				el->add(privet);
				return;
			}
			el->add(nw);
			return;
		}
		if (wor[id] == "(") {
			Node* tut = new Node("SimpleExpr", { new Node(wor[id],{}),new Node("Expr",{})});
			++id;
			expr(tut->arr[1]);
			if (!fl) return;
			if (wor[id] != ")") {
				errore({ ")" });
				fl = false;
				return;
			}
			tut->add(new Node(wor[id], {}));
			el->add(tut);
			++id;
			if (wor[id] == "+" or wor[id] == "-") {
				Node* privet = new Node("Expr", {});
				el->add(new Node(wor[id], {}));
				++id;
				expr(privet);
				if (!fl) return;
				el->add(privet);
			}
			return;
		}
		errore({ "id_name","(" });
		fl = false;
	}
	//---------------------------------------------------------------------------------
	void next(Node* x, int level) {
		for (int i = 0; i <= level; ++i) lt.writeF(".");
		lt.writeF(x->s + '\n');
		for (int i = 0; i < x->arr.size(); ++i) {
			if(x->arr[i]!=nullptr) next(x->arr[i], level + 2);
		}
	}

	void print(int level = 0) {
		next(root, level);
	}
};
