#pragma once
#include "Syntactic.h"


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
				expop(p, py, znp);
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
		int a = num, b = 0;
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
		string tut, elt = "", alt = "";
		expr(el->arr[0], elt, alt);
		tut = elt.substr(1) + alt;
		elt = "";
		alt = "";
		expr(el->arr[2], elt, alt);
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
				expop(p, py, znp);
				vot.push_back(py + znp + ' ');
			}
			else {
				p->fl &= el->fl;
				ift(p);
			}

		}
	}

	void expop(Node* el, string& py, string& znp) {
		if (table[el->arr[0]->arr[0]->s].type == "")erro.insert("Переменная " + el->arr[0]->arr[0]->s + " не определена\n");
		else if (table[el->arr[0]->arr[0]->s].type == "PROGRAM")erro.insert("Переменная " + el->arr[0]->arr[0]->s + " не типа INTEGER\n");
		py = el->arr[0]->arr[0]->s;
		znp = " =";
		expr(el->arr[2], py, znp);
		if (el->fl)table[el->arr[0]->arr[0]->s].val = el->arr[2]->val;
		//cout << el->fl << ' ' << el->arr[0]->arr[0]->s << ' ' << el->arr[2]->val << '\n';
	}
	void simplexpr(Node* el, string& py, string& znp) {
		for (Node* p : el->arr) {
			if (p->s == "(" || p->s == ")") continue;
			if (p->s == "Id") {
				id_t(p, py);
				el->val = p->val;
			}
			else if (p->s == "Const") {
				const_t(p, py);
				el->val = p->val;
			}
			else if (p->s == "Expr") {
				string npy, nznp;
				expr(p, npy, nznp);
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
				simplexpr(p, py, znp);
				el->val = p->val;
			}
			if (p->s == "Expr") {
				expr(p, py, znp);
				if (opert) el->val += p->val;
				else el->val -= p->val;
			}
		}
	}
	void id_t(Node* el, string& py) {
		if (table[el->arr[0]->s].type == "")erro.insert("Переменная " + el->arr[0]->s + " не определена\n");
		else if (table[el->arr[0]->s].type == "PROGRAM")erro.insert("Переменная " + el->arr[0]->s + " не типа INTEGER\n");
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
