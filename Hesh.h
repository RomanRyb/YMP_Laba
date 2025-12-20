#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>


using namespace std;

template<typename T>
struct vec {
	int cap = 8;
	int len = 0;
	T* arr;

	vec() {
		arr = new T[cap];
	}

	vec(int n) {
		cap = n * 2;
		len = n;
		arr = new T[cap];
	}

	vec(const initializer_list<T>& p):vec() {
		if (p.size() > cap) {
			T* newarr = new T[p.size()* 2];
			cap *= p.size()*2;
			delete[] arr;
			arr = newarr;
		}
		len = 0;
		for (auto el : p) {
			push_back(el);
		}
	}

	vec(const vec<T>& a):vec() {
		if (a.size() > cap) {
			T* newarr = new T[a.size() * 2];
			cap *= a.size() * 2;
			delete[] arr;
			arr = newarr;
		}
		len = a.size();
		for (int i = 0; i < len;++i) {
			arr[i] = a[i];
		}
	}

	void push_back(T el) {
		if (len >= cap) {
			T* newarr = new T[cap * 2];
			copy(arr, arr + len, newarr);
			cap *= 2;
			delete[] arr;
			arr = newarr;
		}
		arr[len] = el;
		++len;
	}

	int size() const {
		return len;
	}

	T operator[](int i){
		return arr[i];
	}

	const T operator[](int i) const{
		return arr[i];
	}

	bool empty() {
		return len;
	}

	~vec() {
		if (arr != nullptr) delete[] arr;
	}

};

struct Token {
	string name;
	string value;
	int val = 0;
	string type = "";
	Token(){}
	Token(string name,string value): name(name),value(value){}
	Token(const initializer_list<string>& p){
		auto it = p.begin();
		name = *it;
		++it;
		value = *it;
	}
	Token& operator=(const Token& t){
		this->name = t.name;
		this->value = t.value;
		return *this;
	}
};



struct node {
	string key = "";
	Token t;
	node* next = nullptr;
	node(string key, Token t) {
		this->key = key;
		this->t = t;
		this->next = nullptr;
	}
};


const int N = 2;


struct HashTb {

	int cap;
	int col;
	node** arr;

	HashTb() {
		this->cap = 8;
		this->col = 0;
		this->arr = new node*[cap];
		for (int i = 0; i < cap; ++i) {
			arr[i] = nullptr;
		}
	}
	int HashF(const string& key) {
		int sum = 0, factor = 31;
		for (int i = 0; i < key.size(); ++i) {
			sum = ((sum % cap) + (((int)key[i]) * factor) % cap)%cap;
			factor = ((factor % INT16_MAX)*(31% INT16_MAX))%INT16_MAX;
		}
		return sum;
	}

	void doit() {
		node** larr = arr;
		arr = new node * [cap * 2]();
		int oldlen=cap;
		cap *= 2;
		col = 0;
		node* dl;
		for (int i = 0; i < oldlen; ++i) {
			node* tmp = larr[i];
			while (tmp != nullptr) {
				insert(tmp->key, tmp->t);
				dl = tmp;
				tmp = tmp->next;
				delete dl;
			}
		}
		delete[] larr;
	}

	void insert(string key, Token val) {
		if (search(key)) return;
		if (col >= cap) doit();
		//cout << HashF("=")<<'\n';
		++col;
		int id = HashF(key);
		node* newNode = new node(key,val);
		if (arr[id] == NULL) {
			arr[id] = newNode;
		}
		else {
			newNode->next = arr[id];
			arr[id] = newNode;
		}

	}

	void del(string key) {
		int id = HashF(key);
		node* prev = NULL;
		node* currNode = arr[id];
		while (currNode != NULL) {
			if (currNode->key == key) {
				if (currNode == arr[id]) {
					arr[id] = currNode->next;
				}
				else {
					prev->next = currNode->next;
				}
				free(currNode);
				break;
			}
			prev = currNode;
			currNode = currNode->next;
		}

	}

	bool search(string key) {
		int id = HashF(key);
		struct node* bucket = arr[id];
		while (bucket != nullptr) {
			if (bucket->key == key) {
				return 1;
			}
			bucket = bucket->next;
		}
		return 0;
	}

	~HashTb() {
		for (int i = 0; i < cap; ++i) {
			node* curr = arr[i];
			node* prev;
			while (curr != nullptr) {
				prev = curr;
				curr = curr->next;
				delete prev;
			}
		}
		delete[] arr;
	}
	Token& operator[](const string& s) {
		int id = HashF(s);
		struct node* bucket = arr[id];
		while (bucket != nullptr) {
			if (bucket->key == s) {
				return bucket->t;
			}
			bucket = bucket->next;
		}
		return *(new Token("NONE",""));
	}
	
};