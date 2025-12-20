#pragma once
#include "Lec.h"
#include<iostream>
#include<string>
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



struct Sema {
	Node* root;
	set<string> erro;
	vector<string> post;
	set<string> idman;
	deque<string> vot;
	string tmp = "";
	string tmpzn = "";
	int cop = 0;
	int num = 1;
	ofstream out;
	void start(Node* stat) {
		root = stat;
		for (Node* el : root->arr) {
			if (el->s == "Begin")beg(el);
			if (el->s == "Descriptions")dest(el);
			if (el->s == "Operators")pop(el);
			if (el->s == "End")endp(el);
		}
		try {
			out = ofstream("semout.txt");
		}
		catch (...) {
			cout << "file_error!\n";
		}

	}
	//--------------------------------------------------------------------------------------
	void beg(Node* el) {
		post.push_back(el->arr[0]->s + ' ' + el->arr[1]->arr[0]->s + " 2 CALL\n");
		table[el->arr[1]->arr[0]->s].type = el->arr[0]->s;
	}
	//--------------------------------------------------------------------------------------
	void endp(Node* el) {
		if (table[el->arr[1]->arr[0]->s].type != "PROGRAM") erro.insert("Аргумент в END не совпадает с аргументом в PROGRAM\n");
		post.push_back(el->arr[0]->s + ' ' + el->arr[1]->arr[0]->s + " 2 CALL\n");
	}
	//--------------------------------------------------------------------------------------
	void dest(Node* el) {
		for (Node* p : el->arr) {
			if (p->s == "Descriptions") {
				dest(p);
			}
			else {
				tmp = "";
				cop = 1;
				dop(p);
				tmp += to_string(cop) + " DECL\n";
				post.push_back(tmp);
			}
		}
	}
	void dop(Node* el) {
		tmp = el->arr[0]->arr[0]->s + ' ';
		redop(el->arr[0]->arr[0]->s, el->arr[1]);
	}
	void redop(string s, Node* el) {
		for (Node* p : el->arr) {
			if (p->s == ",") continue;
			if (p->s == "Id") {
				if (table[p->arr[0]->s].type != "") erro.insert("Повтороное определение переменной " + p->arr[0]->s + '\n');
				else table[p->arr[0]->s].type = s;
				idman.insert(p->arr[0]->s);
				tmp += p->arr[0]->s + ' ';
				++cop;
			}
			else redop(s, p);
		}
	}
	//--------------------------------------------------------------------------------------
	void pop(Node* el) {
		for (Node* p : el->arr) {
			if (p == nullptr) break;
			if (p->s == "Operators") {
				p->fl &= el->fl;
				pop(p);
				continue;
			}
			if (p->arr[0]->s == "Id") {
				string py, znp;
				expop(p,py,znp);
				post.push_back(py + znp + '\n');
			}
			else {
				p->fl &= el->fl;
				ift(p);
				num = 1;
				string res = "";
				for (int i = 0; i < vot.size(); ++i) {
					res += vot[i] + ' ';
				}
				res += '\n';
				post.push_back(res);
				vot.clear();
			}

		}
	}
	void ift(Node* el) {
		bool bol = false;
		int a = num, b=0;
		++num;
		if (el->arr.size() == 8) {
			b = num;
			++num;
		}
		for (Node* p : el->arr) {
			if (p->s == "ELSE") {
				bol = true;
				continue;
			}
			if (p->s == "Condition") {
				cond(p);
				el->val = p->val;
				vot.push_back("m[" + to_string(a) + "] BF ");
			}
			else if (p->s == "Operators") {
				if (bol) {
					vot.push_back("m[" + to_string(a) + "] DEFL ");
					if (el->val)p->fl = 0;
					p->fl &= el->fl;
				}
				else {
					if (!el->val)p->fl = 0;
					p->fl &= el->fl;
				}
				okl(p);
				if (!bol) {
					if (b == 0) vot.push_back("m[" + to_string(a) + "] BRL ");
					else vot.push_back("m[" + to_string(b) + "] BRL ");
				}
			}
		}
		if (b == 0) vot.push_back("m[" + to_string(a) + "] DEFL ");
		else vot.push_back("m[" + to_string(b) + "] DEFL ");
	}
	void cond(Node* el) {
		string tut,elt="", alt="";
		expr(el->arr[0],elt,alt);
		tut = elt.substr(1) + alt;
		elt = "";
		alt = "";
		expr(el->arr[2],elt, alt);
		tut += elt + alt;
		tut += " " + el->arr[1]->arr[0]->s;
		vot.push_back(tut);
		el->val = opt(el->arr[0]->val, el->arr[2]->val, el->arr[1]->arr[0]->s);
	}
	int opt(int& a, int& b, const string& s) {
		if (s == ".EQ.") return a == b;
		if (s == ".NE.") return a != b;
		if (s == ".GE.") return a >= b;
		if (s == ".GT.") return a > b;
		if (s == ".LE.") return a <= b;
		if (s == ".LT.") return a < b;
		return 0;
	}
	void okl(Node* el) {
		for (Node* p : el->arr) {
			if (p->s == "Operators") {
				p->fl = el->fl;
				okl(p);
				continue;
			}
			if (p->arr[0]->s == "Id") {
				string py, znp;
				p->fl &= el->fl;
				expop(p,py,znp);
				vot.push_back(py + znp + ' ');
			}
			else {
				p->fl &= el->fl;
				ift(p);
			}

		}
	}

	void expop(Node* el,string & py, string & znp) {
		if (table[el->arr[0]->arr[0]->s].type == "")erro.insert("Переменная " + el->arr[0]->arr[0]->s + " не определена\n");
		py = el->arr[0]->arr[0]->s;
		znp = " =";
		expr(el->arr[2],py,znp);
		if (el->fl)table[el->arr[0]->arr[0]->s].val = el->arr[2]->val;
		//cout << el->fl << ' ' << el->arr[0]->arr[0]->s << ' ' << el->arr[2]->val << '\n';
	}
	void simplexpr(Node* el, string& py, string& znp) {
		for (Node* p : el->arr) {
			if (p->s == "(" || p->s == ")") continue;
			if (p->s == "Id") {
				id_t(p,py);
				el->val = p->val;
			}
			else if (p->s == "Const") {
				const_t(p,py);
				el->val = p->val;
			}
			else if (p->s == "Expr") {
				string npy, nznp;
				expr(p,npy,nznp);
				py += npy + nznp;
				el->val = p->val;
			}
		}
	}
	void expr(Node* el, string& py, string& znp) {
		bool opert = false;
		for (Node* p : el->arr) {
			if (p->s == "+" or p->s == "-") {
				znp = ' ' + p->s + znp;
				if (p->s == "+")opert = true;
				else opert = false;
				continue;
			}
			if (p->s == "SimpleExpr") {
				simplexpr(p,py,znp);
				el->val = p->val;
			}
			if (p->s == "Expr") {
				expr(p,py, znp);
				if (opert) el->val += p->val;
				else el->val -= p->val;
			}
		}
	}
	void id_t(Node* el, string& py) {
		if (table[el->arr[0]->s].type == "")erro.insert("Переменная " + el->arr[0]->s + " не определена\n");
		//else idman.insert(el->arr[0]->s);
		el->val = table[el->arr[0]->s].val;
		py += ' ' + el->arr[0]->s;
	}
	void const_t(Node* el, string& py) {
		el->val = stoi(el->arr[0]->s);
		py += ' ' + el->arr[0]->s;
	}
	//--------------------------------------------------------------------------------------
	void print_erro() {
		for (auto el : erro) {
			out << el;
		}
	}

	void print_post() {
		for (int i = 0; i < post.size(); ++i) {
			out << post[i];
		}
		out << "-----------------------------------\n";
		/*for (auto el : idman) {
			out << el << " = " << table[el].val << '\n';
		}*/
	}

	void print() {
		if (!out.is_open()) return;
		if (erro.size() > 0) print_erro();
		else print_post();
	}

	~Sema() {
		if (out.is_open()) out.close();
	}
};

