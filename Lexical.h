#pragma once
#include "HashTable.h"


HashTb table;
vec<string> wor;
vec<pair<int,int>> lin;


struct LecAnal {
	ifstream file;
	ofstream out;
	ofstream tout;
	int countfail = 0;
	int numstr = 0;
	LecAnal() {
		try {
			file = ifstream("input.txt");
			out = ofstream("output.txt");
			tout = ofstream("TREEPR.txt");
		}
		catch (...) {
			cout << "file_error!\n";
		}
	}
	
	bool none(const char& c) {
		return c == '	' || c == ' ' || c == '\0' || c == '\n';
	}
	bool need(const char& c) {
		return c == '+' || c == '-' || c==',' || c=='=' || c=='(' || c==')';
	}
	bool boly(const char& c) {
		return c == 'E' || c == 'N' || c == 'G' || c == 'L';
	}
	void split(vec<string>& a, const string& s) {
		string st = "";
		for (int i = 0; i < s.size(); ++i) {
			if (none(s[i]) and st.size() == 0) continue;

			if ((none(s[i]) or need(s[i]) or (st[0] != '.' and s[i] == '.' and st.size()!=i+1 and boly(s[i+1]))) and st.size() > 0) {
				a.push_back(st);
				lin.push_back({ numstr,i - st.size()+1});
				st = "";
				--i;
				continue;
			}
			if (!none(s[i])) {
				if (st.size() > 0 and need(s[i])) {
					a.push_back(st);
					a.push_back(to_string(s[i]));
					lin.push_back({ numstr,i -st.size()});
					lin.push_back({ numstr,i - st.size()+1});
					st = "";
					continue;
				}
				st+=s[i];
				if (need(s[i]) and s[i]!='-') {
					a.push_back(st);
					lin.push_back({ numstr,i+1});
					st = "";
				}
				else if (s[i] == '-') {
					if (s[i + 1] == ' ') continue;
					string pty="";
					if (a.size() > 0)pty = a[a.size() - 1];
					if (i+1<s.size() and (s[i+1]<'0' or s[i+1]>'9' or (pty!="" and pty != "(" and pty != "IF" and pty != "=" and pty[pty.size()-1] != '.'))) {
						a.push_back(st);
						lin.push_back({ numstr,i + 1 });
						st = "";
					}
				}
			}
			if (!none(s[i]) and st[0] == '.' and s[i] == '.' and st.size() > 1) {
				a.push_back(st);
				lin.push_back({ numstr,i - st.size()+2});
				st = "";
				continue;
			}
			if (!none(s[i]) and i+1==s.size() and st.size() > 0) {
				a.push_back(st);
				lin.push_back({ numstr,i - st.size()+2});
				st = "";
			}
		}
	}
	bool KeyWord(const string& s) {
		return s == "INTEGER" || s == "IF" || s == "THEN" || s == "ELSE" ||
			s == "PROGRAM" || s == "END";
	}
	bool Id(const string& s) {
		if (s.size() == 0) return 0;
		for (int i = 0; i < s.size(); ++i) {
			if ((s[i] < 'a' or s[i]>'z') and (s[i] < 'A' or s[i]>'Z')) return 0;
		}
		return 1;
	}
	bool Op(const string& s) {
		return s == "=" || s == "-" || s == "+";
	}
	bool OpRel(const string& s) {
		return s == ".EQ." || s == ".NE." ||
			s == ".GE." || s == ".GT." || s == ".LE." || s == ".LT.";
	}
	bool Demc(const string& s) {
		return s=="," || s=="(" || s==")";
	}
	bool Const(const string& s) {
		if (s.size() == 0 or (s.size()>1 and s[0]=='0')) return 0;
		int i = 0;
		if (s[0] == '-') i = 1;
		for (; i < s.size(); ++i) {
			if (s[i] < '0' or s[i]>'9') return 0;
		}
		return 1;
	}
	void tokenize(const vec<string>& a) {
		for (int i = 0; i < a.size(); ++i) {
			if (Op(a[i])) {
				table.insert(a[i], { "OP",a[i] });
				wor.push_back(a[i]);
				continue;
			}
			if (OpRel(a[i])) {
				table.insert(a[i], { "ROP",a[i] });
				wor.push_back(a[i]);
				continue;
			}
			if (Demc(a[i])) {
				table.insert(a[i], { "D",a[i] });
				wor.push_back(a[i]);
				continue;
			}
			if (KeyWord(a[i])) {
				table.insert(a[i], { "KW",a[i] });
				wor.push_back(a[i]);
				continue;
			}
			if (Const(a[i])) {
				table.insert(a[i], { "CONST",a[i] });
				wor.push_back(a[i]);
				continue;
			}
			if (Id(a[i])) {
				table.insert(a[i], { "ID",a[i] });
				wor.push_back(a[i]);
				continue;
			}
			out << "Не знаю слово " << a[i] << " в стрчке " << numstr << '\n';
			++countfail;
		}
	}
	void writeF(const string& s) {
		tout << s;
	}

	void START() {
		if (!file.is_open() or !out.is_open()) return;
		string s;
		out << "Ошибки:\n";
		while (getline(file, s)) {
			++numstr;
			vec<string> a;
			split(a, s);
			/*for (int i = 0; i < a.size(); ++i) {
				cout << a[i] << ' ';
			}
			cout << '\n';*/
			tokenize(a);
		}
		out << "-------------------------------------\n";

	}
	void print() {
		if (!out.is_open()) return;
		out << "Хеш-таблица:\n";
		struct node* bucket;
		Token tmp;
		for (int i = 0; i < table.cap; ++i) {
			bucket = table.arr[i];
			//if (bucket != nullptr) cout << "Ячейка: " << i + 1 << '\n';
			while (bucket != nullptr) {
				tmp = bucket->t;
				out << tmp.name << ' ' << tmp.value << ' ' << i << '\n';
				bucket = bucket->next;
			}
		}
		out << "-------------------------------------\n";
	}

	~LecAnal() {
		if(file.is_open()) file.close();
		if(out.is_open()) out.close();
		if (out.is_open()) tout.close();
	}
};

LecAnal lt;