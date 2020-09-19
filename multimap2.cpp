#include <iostream>
#include <algorithm>
#include <vector>
#include <time.h>

using std::cout;
using std::cin;
using std::endl;

template<class T>
class avl_tree;

template<class T>
class node;

template<class T>
class my_set;

template<class T, class R>
class my_pair;

template<class T, class R>
class my_map;

template<class T>
class my_multiset;

template<class T>
class node2;

template<class T>
class my_list;

template<class T, class R>
class my_multimap;


template<class T>
class node
{
public:
	T key;
	int h;
	node<T>* left, *right, *parent;
	node()
	{
		h = 1;
	}
};

template<class T>
class avl_tree
{
public:

	/* тут прототипы всех функций
	bool is_in(T x);
	T get_min();
	T get_max();
	bool insert(T x);
	bool erase(T x);

	avl_tree();
	node<T> *create_node(T x);
	node<T> *min(node<T> *x); нахождение минимума в поддереве
	node<T> *max(node<T> *x); нахождение максимума в поддереве
	int find_h(node<T> *x); нахождение высоты дерева x при условии, что высоты поддеревьев посчитаны правильно
	void fix_h(node<T> *x); исправляет высоты всех родителей x до корня
	node<T> *find_place(const T& x);
	void left_rotation(node<T> *x);
	void right_rotation(node<T> *x);
	void balance(node<T> *x); балансирует дерево x если все поддеревья сбалансированы и разность высот у x не больше 2
	void print(node<T> *x);
	~avl_tree();
	*/
	node<T> *root;
	int size;
	avl_tree()
	{
		root = nullptr;
		size = 0;
	}
	node<T>* create_node(T x)
	{
		node<T>* res = new node<T>;
		res->key = x;
		res->parent = nullptr;
		res->left = nullptr;
		res->right = nullptr;
		return res;
	}
	node<T> *min(node<T> *x)
	{
		if (x == nullptr) return nullptr;
		while (x->left != nullptr)
		{
			x = x->left;
		}
		return x;
	}
	T get_min()
	{
		if (root == nullptr) return T();
		return min(root)->key;
	}
	node<T> *max(node<T> *x)
	{
		if (x == nullptr) return nullptr;
		while (x->right != nullptr)
		{
			x = x->right;
		}
		return x;
	}
	T get_max()
	{
		if (root == nullptr) return T();
		return max(root)->key;
	}
	int find_h(node<T>* x)
	{
		if (x == nullptr) return 0;
		if (x->left == nullptr)
		{
			if (x->right == nullptr)
				return 1;
			return x->right->h + 1;
		}
		if (x->right == nullptr)
			return x->left->h + 1;
		return (x->left->h < x->right->h) ? x->right->h + 1 : x->left->h + 1;
	}
	void fix_h(node<T>* x)
	{
		if (x == nullptr)
		{
			cout << "can't fix\n";
		}
		while (x != nullptr)
		{
			x->h = find_h(x);
			x = x->parent;
		}
	}
	bool is_in(const T& x)
	{
		node<T>* ptr = root;
		if (ptr == nullptr) return false;
		while (ptr != nullptr && x != ptr->key)
		{
			if (x < ptr->key)
				ptr = ptr->left;
			else ptr = ptr->right;
		}
		if (ptr != nullptr) return true;
		return false;
	}
	node<T>* find_place(const T& x)
	{
		node<T>* ptr = root;
		while (ptr != nullptr && x != ptr->key)
		{
			if (x < ptr->key)
			if (ptr->left == nullptr) return ptr;
			else ptr = ptr->left;
			else if (ptr->right == nullptr) return ptr;
			else ptr = ptr->right;
		}
		return ptr;

	}
	void left_rotation(node<T>* a)// берем поддерево с корнем а и ненулевым левым ребенком b, потом делаем корнем b, у него правый ребенок- а
	{
		if (a->left == nullptr)
		{
			cout << "ne povorachivaetsy vlevo\n";
			return;
		}
		node<T>* par = a->parent, *b = a->left, *A = b->left, *B = b->right, *C = a->right; //A<b<B<a<C тут и везде маленькие буквы- вершины, большие- поддеревья в данных вершинах
		b->right = a;
		a->left = B;
		if (par != nullptr) if (par->key < a->key)
			par->right = b;
		else par->left = b;
		if (B != nullptr) B->parent = a;
		a->parent = b;
		b->parent = par;

		if (a == root) root = b;

		fix_h(a);
	}
	void right_rotation(node<T>* a)
	{
		if (a->right == nullptr)
		{
			cout << "ne povorachivaetsy vpravo\n";
			return;
		}
		node<T>* par = a->parent, *b = a->right, *A = a->left, *B = b->left, *C = b->right;
		a->right = B;
		if (B != nullptr) B->parent = a;
		b->left = a;
		a->parent = b;
		if (par != nullptr) if (a->key < par->key)
		{
			par->left = b;
		}
		else
		{
			par->right = b;
		}
		b->parent = par;
		if (a == root) root = b;
		fix_h(a);
	}
	void balance(node<T>* x) //пусть у x высота h, тогда у одного поддерева h-1, у другого h-3(либо все хорошо, ничего делать не надо)
	{
		if (x == nullptr)
		{
			cout << "balance empty node\n";
		}
		int hl = (x->left == nullptr) ? 0 : x->left->h;
		int hr = (x->right == nullptr) ? 0 : x->right->h;
		node<T>* b = x->left;
		node<T>* c = x->right;
		if (hl - hr == 2)
		{
			if (find_h(x->left->left) == x->h - 2) // у дерева высоты h-1 могут быть
			{
				left_rotation(x); //ребенок той же стороны, что и большое поддерево высоты h-2: этот случай
			}
			else
			{
				right_rotation(b);//этот ребенок имеет высоту h-3
				left_rotation(x);
			}
			return;
		}
		if (hl - hr == -2)
		{
			if (find_h(x->right->right) == x->h - 2)
			{
				right_rotation(x);
			}
			else
			{
				left_rotation(c);
				right_rotation(x);
			}
			return;
		}
	}
	bool insert(T x)//добавление как в обычное дерево поиска, только при этом можем нарушить сбалансированность дерева, поэтому исправляем
	{
		if (root == nullptr)
		{
			root = create_node(x);
			size++;
			return true;
		}
		node<T>* n = create_node(x), *place = find_place(x);
		if (place->key < x)
		{
			place->right = n;
			n->parent = place;
			fix_h(n);
			while (place != nullptr)
			{
				balance(place);
				place = place->parent;
			}

			size++;
			return true;
		}
		place->left = n;
		n->parent = place;
		fix_h(n);

		while (place != nullptr)
		{
			balance(place);
			place = place->parent;
		}
		size++;
		return true;
	}
	bool erase(T x)
	{
		if (!is_in(x)) return false;
		node<T>* ptr = root;
		while (ptr != nullptr)
		{
			if (ptr->key < x)
				ptr = ptr->right;
			else if (ptr->key == x)
				break;
			else ptr = ptr->left;
		}
		//если удаляем лист или одна из поддеревьев пустое, то все просто- удаляем, ставим на это место вершину поддерева, балансируем
		//сложности, если есть оба ребенка- тогда(например), в правом поддереве находим минимальный элемент и ставим на место удаляемой вершины
		//в правом поддереве у минимального элемента нет левого ребенка- иначе он имел меньший ключ, а из сбалансированности справа максимум один элемент
		//еще надо рассмотреть случай когда удаляем корень
		node<T> *par = ptr->parent, *A = ptr->left, *B = ptr->right, *m = min(B), *prev = (m == nullptr) ? nullptr : m->parent, *r = (m == nullptr) ? nullptr : m->right;
		bool is_root = (ptr == root), is_left = (ptr->left == nullptr), is_right = (ptr->right == nullptr), left_par = (par == nullptr) ? true : (par->key < x), sovp = (ptr == prev);


		if (is_left&&is_right)
		{
			if (is_root)
			{
				delete root;
				root = nullptr;
				size--;
				return true;
			}
			if (left_par)
			{
				par->right = nullptr;
				delete ptr;
				size--;
				fix_h(par);
				balance(par);
				return true;
			}
			par->left = nullptr;
			delete ptr;
			size--;
			fix_h(par);
			balance(par);
			return true;
		}
		if (!is_left&&is_right)
		{
			if (is_root)
			{
				root = A;
				A->parent = nullptr;
				size--;
				delete ptr;
				return true;
			}
			A->parent = par;
			if (left_par) par->right = A;
			else par->left = A;
			delete ptr;
			size--;
			fix_h(A);
			balance(A);
			return true;
		}
		if (is_left&& !is_right)
		{
			if (is_root)
			{
				root = B;
				B->parent = nullptr;
				size--;
				delete ptr;
				return true;
			}
			if (left_par)
			{
				par->right = B;
				B->parent = par;
				delete ptr;
				size--;
				fix_h(B);
				balance(B);
				return true;
			}
			par->left = B;
			B->parent = par;
			delete ptr;
			size--;
			fix_h(B);
			balance(B);
			return true;
		}

		//оба родителя есть

		if (is_root)
		{
			root = m;
			m->parent = nullptr;
			if (!sovp && (r == nullptr))
			{
				m->left = A;
				A->parent = m;
				m->right = B;
				B->parent = m;


				prev->left = nullptr;


				delete ptr;
				size--;
				fix_h(prev);
				balance(prev);
				return true;
			}
			if (!sovp && (r != nullptr))
			{
				m->left = A;
				A->parent = m;
				m->right = B;
				B->parent = m;
				prev->left = r;
				delete ptr;
				size--;
				r->parent = prev;
				fix_h(prev);
				balance(prev);
				return true;
			}
			if (sovp && (r == nullptr))
			{
				m->left = A;

				A->parent = m;

				delete ptr;
				size--;
				fix_h(m);
				balance(m);
				return true;
			}
			m->left = A;

			A->parent = m;

			delete ptr;
			size--;
			fix_h(m);
			balance(m);
			return true;
		}

		//оба ребенка, ptr не в корне
		m->parent = par;
		if (!sovp && (r == nullptr))
		{
			m->left = A;
			A->parent = m;
			m->right = B;
			B->parent = m;
			delete ptr;
			size--;
			fix_h(prev);
			balance(prev);
			return true;
		}
		if (!sovp && (r != nullptr))
		{
			m->left = A;
			A->parent = m;
			m->right = B;
			B->parent = m;
			delete ptr;
			size--;
			r->parent = prev;
			fix_h(prev);
			balance(prev);
			return true;
		}
		if (sovp && (r == nullptr))
		{
			m->left = A;
			delete ptr;
			size--;
			fix_h(m);
			balance(m);
			return true;
		}
		m->left = A;
		delete ptr;
		size--;
		fix_h(m);
		balance(m);
		return true;

	}
	void print(node<T> *r)
	{
		cout << r->key << " ";
		if (r->left != nullptr) print(r->left);
		if (r->right != nullptr) print(r->right);
	}
	~avl_tree()
	{
		int s = size;
		for (int i = 0; i<s; i++)
		{
			erase(root->key);
		}
	}
};

template<class T>
class my_set : private avl_tree<T>
{
public:

	/*bool is_in(T x);
	T get_min();
	T get_max();
	bool insert(T x);
	bool erase(T x);
	*/

	bool is_in(T x)
	{
		return avl_tree<T>::is_in(x);
	}
	T get_min()
	{
		return avl_tree<T>::get_min();
	}
	T get_max()
	{
		return avl_tree<T>::get_max();
	}
	bool insert(T x)
	{
		return avl_tree<T>::insert(x);
	}
	bool erase(T x)
	{
		return avl_tree<T>::erase(x);
	}

};

template<class T, class R>
class my_pair
{
public:
	T first;
	R second;
	bool operator <(const my_pair<T, R>& x) const
	{
		return first<x.first;
	}
	bool operator ==(const my_pair<T, R>& x) const
	{
		return first == x.first;
	}
	bool operator <=(const my_pair<T, R>& x) const
	{
		return first <= x.first;
	}
	bool operator !=(const my_pair<T, R>& x) const
	{
		return first != x.first;
	}

};

template<class T, class R>
my_pair<T, R> make_pair(T x, R y)
{
	my_pair<T, R> res;
	res.first = x;
	res.second = y;
	return res;
}

template<class T, class R>
class my_map : private avl_tree<my_pair<T, R>>
{
public:

	/*bool is_in(T x, R y);
	my_pair<T,R> get_min();
	my_pair<T,R> get_max();
	bool insert(T x, R y);
	bool erase(T x, R y);
	*/

	bool is_in(T x, R y)
	{
		return is_in(make_pair<T, R>(x, y));
	}
	bool is_in(my_pair<T, R> x)
	{
		return avl_tree<my_pair<T, R>>::is_in(x) && avl_tree<my_pair<T, R>>::find_place(x)->key.second == x.second;
	}
	my_pair<T, R> get_min()
	{
		return avl_tree<my_pair<T, R>>::get_min();
	}
	my_pair<T, R> get_max()
	{
		return avl_tree<my_pair<T, R>>::get_max();
	}
	bool insert(my_pair<T, R> x)
	{
		return avl_tree<my_pair<T, R>>::insert(x);
	}
	bool insert(T x, R y)
	{
		return insert(make_pair<T, R>(x, y));
	}
	bool erase(my_pair<T, R> x)
	{
		return avl_tree<my_pair<T, R>>::erase(x);
	}
	bool erase(T x, R y)
	{
		return erase(make_pair<T, R>(x, y));
	}

};

template<class T>
class my_multiset : private avl_tree<my_pair<T, int>>
{
public:

	/*int is_in(T x);
	my_pair<T,int> get_min();
	my_pair<T,int> get_max();
	bool insert(T x);
	bool erase(T x);
	*/

	int is_in(T x)
	{
		return (!avl_tree<my_pair<T, int>>::is_in(make_pair<T, int>(x, 1))) ? (0) : avl_tree<my_pair<T, int>>::find_place(make_pair<T, int>(x, 1))->key.second;
	}
	my_pair<T, int> get_min()
	{
		return avl_tree<my_pair<T, int>>::get_min();
	}
	my_pair<T, int> get_max()
	{
		return avl_tree<my_pair<T, int>>::get_max();
	}
	bool insert(T x)
	{
		if (!avl_tree<my_pair<T, int>>::is_in(make_pair<T, int>(x, 1)))
			return avl_tree<my_pair<T, int>>::insert(make_pair<T, int>(x, 1));
		avl_tree<my_pair<T, int>>::find_place(make_pair<T, int>(x, 1))->key.second++;
		return true;
	}
	bool erase(T x)
	{
		if (avl_tree<my_pair<T, int>>::find_place(make_pair<T, int>(x, 1))->key.second == 1)
			return avl_tree<my_pair<T, int>>::erase(make_pair<T, int>(x, 1));
		avl_tree<my_pair<T, int>>::find_place(make_pair<T, int>(x, 1))->key.second--;
		return true;
	}
};

template<class T>
class node2
{
public:
	node2<T> *next;
	T key;
	node2(T x)
	{
		key = x;
		next = nullptr;
	}
};

template<class T>
class my_list
{
public:
	node2<T> *start = nullptr;
	my_list()
	{
		start = nullptr;
	}
	my_list(T x)
	{
		start = new node2<T>(x);
	}
	void insert(T x)
	{
		node2<T> *tmp = new node2<T>(x);
		tmp->next = start;
		start = tmp;
	}
	bool erase(T x)
	{
		node2<T> *tmp = start, *prev;
		if (tmp != nullptr && tmp->key != x)
		{
			prev = tmp;
			tmp = tmp->next;
		}
		else
		{
			if (tmp == nullptr) return false;
			node2<T> *new_start = tmp->next;
			delete tmp;
			start = new_start;
			return true;
		}
		while (tmp != nullptr && tmp->key != x)
		{
			tmp = tmp->next;
			prev = prev->next;
		}
		if (tmp == nullptr) return false;
		node2<T> *new_next = tmp->next;
		delete tmp;
		prev->next = new_next;
		return true;
	}
	int is_in(T x)
	{
		int res = 0;
		node2<T> *tmp = start;
		while (tmp != nullptr)
		{
			if (tmp->key == x) res++;
			tmp = tmp->next;
		}
		return res;
	}
	~my_list()
	{
		if (start == nullptr) return;
		node2<T> *tmp = start;
		node2<T> *nex = tmp->next;
		while (tmp != nullptr)
		{
			delete tmp;
			tmp = nex;
			if (nex == nullptr)
			{
				break;
			}
			nex = nex->next;
		}

	}

};

template<class R>
my_list<R> *make_list(R x)
{
	my_list<R> *res = new my_list<R>(x);
	return res;
}

template<class T, class R>
class my_multimap : private avl_tree<my_pair<T, my_list<R>*>>
{
public:

	/*int is_in(T x);
	my_list<R> get_min();
	my_list<R> get_max();
	bool insert(T x, R y);
	bool erase(T x, R y);
	*/

	int is_in(T x, R y)
	{
		my_list<R> *l = make_list<R>(y);
		my_pair<T, my_list<R>*> p = make_pair(x, l);
		return (!avl_tree<my_pair<T, my_list<R>*>>::is_in(p)) ? 0 : avl_tree<my_pair<T, my_list<R>*>>::find_place(p)->key.second->is_in(y);
	}
	my_list<R> *get_min()
	{
		return avl_tree<my_pair<T, my_list<R>>>::get_min().second;
	}
	my_list<R> *get_max()
	{
		return avl_tree<my_pair<T, my_list<R>>>::get_max().second;
	}
	bool insert(T x, R y)
	{
		my_list<R> *l = make_list<R>(y);
		my_pair<T, my_list<R>*> p = make_pair(x, l);
		if (!avl_tree<my_pair<T, my_list<R>*>>::is_in(p))
			return avl_tree<my_pair<T, my_list<R>*>>::insert(p);
		avl_tree<my_pair<T, my_list<R>*>>::find_place(p)->key.second->insert(y);
		return true;
	}
	bool erase(T x, R y)
	{
		if (!is_in(x, y)) return false;
		my_list<R> *l = make_list<R>(y);
		my_pair<T, my_list<R>*> p = make_pair(x, l);
		return avl_tree<my_pair<T, my_list<R>*>>::find_place(p)->key.second->erase(y);
	}
};

int main()
{
	my_set<int> A;
	int check = 200000;
	std::vector<int> randarray;
	randarray.push_back(0);
	A.insert(0);
	for (int i = 1; i < check; i++)
	{
		randarray.push_back((-1 + 2 * (rand() % 2))*(1 + abs(randarray[i - 1]) + rand() % 10));
		A.insert(randarray[i]);
	}
	std::sort(randarray.begin(), randarray.end());
	for (int i = 0; i < check; i++)
	{
		int min = A.get_min();
		if (min != randarray[i])
		{
			cout << "it doesn't work!" << endl;
			return -1;
		}
		A.erase(min);
	}
	cout << "it works!" << endl;
	return 0;
}
