#include <bits/stdc++.h>

template<class ValueMonoid, class OperatorMonoid,
	class Modifier, ::std::size_t NODE_NUM=1000000>
class Treap{
public:
	using value_structure = ValueMonoid;
	using value_type = typename value_structure::value_type;
	using operator_structure = OperatorMonoid;
	using operator_type = typename operator_structure::value_type;
	using modifier = Modifier;
	using size_type = ::std::size_t;

private:
	class Node{
	public:
//		using pointer = ::std::shared_ptr<Node>;
		using pointer = Node*;
		value_type v, acc;
		operator_type lazy;
		pointer lch, rch;
		size_type size, pri;

		Node()
			: v(value_structure::identity()), lazy(operator_structure::identity()),
				pri(0), size(1), acc(value_structure::identity()){}

		Node(value_type v, size_type pri):
			v(v), lazy(operator_structure::identity()), pri(pri), size(1), acc(v){
			lch = rch = nullptr;
		}
	};

	static size_type xorshift(){
		static size_type x = 123456789, y = 362436069, z = 521288629, w = 88675123;
		size_type t = (x ^ (x << 11));
		x = y; y = z; z = w;
		return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
	}

public:
//	using pointer = ::std::shared_ptr<Node>;
	using pointer = Node*;

private:
	pointer root;

	static void apply(pointer p, const operator_type x){
		p->lazy = operator_structure::operation(p->lazy, x);
		p->acc = modifier::operation(p->acc, p->lazy, count(p));
	}

public:
	Treap() : root(nullptr) {}
	Treap(pointer p) : root(p) {}

	static size_type count(pointer p){
		return p == nullptr ? 0 : p->size;
	}
	static value_type query(pointer p){
		return p == nullptr ? value_structure::identity() : p->acc;
	}

	static pointer eval(pointer p){
		if(p->lazy != operator_structure::identity()){
			p->v = modifier::operation(p->v, p->lazy, 1);
			if(p->lch != nullptr){
				apply(p->lch, p->lazy);
			}
			if(p->rch != nullptr){
				apply(p->rch, p->lazy);
			}
			p->lazy = operator_structure::identity();
		}
		return update(p);
	}

	static pointer update(pointer p){
		p->size = count(p->lch) + count(p->rch) + 1;
		p->acc = value_structure::operation(
				value_structure::operation(query(p->lch), p->v),
				query(p->rch));
		return p;
	}

	static pointer merge(pointer l, pointer r){
		if(l == nullptr || r == nullptr)
			return l == nullptr ? r : l;
		if(l->pri > r->pri){
			l = eval(l);
			l->rch = merge(l->rch, r);
			return update(l);
		}else{
			r = eval(r);
			r->lch = merge(l, r->lch);
			return update(r);
		}
	}

	static ::std::pair<pointer, pointer> split(pointer p, size_type k){
		if(p == nullptr) return ::std::make_pair(nullptr, nullptr);

		p = eval(p);
		if(k <= count(p->lch)){
			::std::pair<pointer, pointer> ret = split(p->lch, k);
			p->lch = ret.second;
			return ::std::make_pair(ret.first, update(p));
		}else{
			::std::pair<pointer, pointer> ret = split(p->rch, k - count(p->lch) - 1);
			p->rch = ret.first;
			return ::std::make_pair(update(p), ret.second);
		}
	}

	static Treap merge(Treap l, Treap r){
		return Treap(merge(l.root, r.root));
	}

	::std::pair<Treap, Treap> split(size_type k){
		::std::pair<pointer, pointer> ret = split(root, k);
		return ::std::make_pair(Treap(ret.first), Treap(ret.second));
	}

	void insert(size_type k, const value_type v, size_type pri=xorshift()){
		if(root == nullptr){
			root = create_node(v, pri);
			return;
		}
		pointer l, r;
		::std::tie(l, r) = split(root, k);
		root = merge(l, create_node(v, pri));
		root = merge(root, r);
	}

	value_type erase(size_type k){
		Treap l, _, r;
		::std::tie(l, r) = split(k+1);
		::std::tie(l, _) = l.split(k);
		root = merge(l.root, r.root);
		return _.root->v;
	}

	value_type kth_element(const size_type k){
		Treap l, _, r;
		value_type ret;
		::std::tie(l, r) = split(k);
		::std::tie(l, _) = l.split(k-1);
		ret = query(_.root);
		root = merge(l.root, merge(_.root, r.root));
		return ret;
	}

	value_type query(const size_type first, const size_type last){
		Treap l, m, r;
		value_type ret;
		::std::tie(l, r) = split(last);
		::std::tie(l, m) = l.split(first);
		ret = query(m.root);
		root = merge(l.root, merge(m.root, r.root));
		return ret;
	}

	void update(const size_type first, const size_type last, const operator_type a){
		Treap l, m, r;
		::std::tie(l, r) = split(last);
		::std::tie(l, m) = l.split(first);
		apply(m.root, a);
		root = merge(l.root, merge(m.root, r.root));
	}

	template<class InputIterator>
	void insert_from_iterator(InputIterator first, InputIterator last, size_type k=-1){
		for(InputIterator itr = first; itr != last ; ++itr){
			if(k == -1){
				insert(count(root), *itr);
			}else{
				insert(k++, *itr);
			}
		}
	}
/*
	pointer create_node(const value_type v, size_type pri){
		return ::std::make_shared<Node>(Node(v, pri));
	}
	*/

	static pointer create_node(const value_type v, size_type pri){
		static Node pool[NODE_NUM];
		static size_type ptr = 0;
		return &(pool[ptr++]=Node(v, pri));
	}

	void dump(pointer p, typename ::std::vector<value_type>::iterator itr){
		if(p == nullptr) return;
		p = eval(p);
		dump(p->lch, itr);
		*(itr+count(p->lch)) = p->v;
		dump(p->rch, itr+count(p->lch)+1);
	}

  ::std::vector<value_type> dump(pointer p=nullptr){
		if(p == nullptr) p = root;
    ::std::vector<value_type> v(count(p));
    dump(p,v.begin());
    return v;
  }
};

class min_monoid{
public:
	using value_type = int;
	static value_type identity(){return (1LL<<31)-1;}
	static value_type operation(const value_type a, const value_type b){
		return ::std::min(a, b);
	}
};

class update_monoid{
public:
	using value_type = int;
	static value_type identity(){return -1;}
	static value_type operation(const value_type a, const value_type b){
		if(b == identity())
			return a;
		return b;
	}
};

class modifier{
public:
	using size_type = ::std::size_t;
	static min_monoid::value_type operation(min_monoid::value_type a,
			update_monoid::value_type b, size_type sz){
		if(b == update_monoid::identity())
			return a;
		return b;
	}
};

int main(void){
	int n, q;
	scanf("%d %d", &n, &q);
	::std::vector<int> v(n);
	for(int i = 0;i < n;i++)
		::std::cin >> v[i];
	Treap<min_monoid, update_monoid, modifier> t;
	t.insert_from_iterator(v.begin(), v.end());
	for(int i = 0;i < q;i++){
		int x, l, r;
		scanf("%d %d %d", &x, &l, &r);
		if(x == 0){
			int val = t.erase(r);
			t.insert(l, val);
		}else if(x == 1){
			printf("%d\n", t.query(l, r+1));
		}else{
			t.update(l, l+1, r);
		}
	}
}

/*
verify: http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3177276#1

template<ValueMonoid, OperatorMonoid, Modifier, NODE_NUM>
class Treap

ValueMonoid
	- 役割
		- 扱う要素の値
	-	要求
		- value_type
		- identity() -> value_type : 単位元を返す
		- operation(value_type, value_type) -> value_type : 演算結果を返す

OperatorMonoid
	- 役割
		- 扱う要素に適用させる値
	- 要求
		- value_type
		- identity() -> value_type : 単位元を返す
		- operation(value_type, value_type) -> value_type : 作用素を結合する

Modifier<ValueMonoid, OperatorMonoid>
	- 役割
		- OperatorMonoidをValueMonoidに適用させる
	- 要求
		- operation(value_type, operator_type, size_type) -> value_type
			: (size)個の要素の結合に作用素を適用させた結果を返す

NODE_NUM
	- 使用するノードの数

Treap
	- 提供
		- query(size_type l, size_type r) -> value_type
			- 計算量 O(log N)
			- [l, r)までの計算結果

		- update(size_type l, size_type r, operator_type x)
			- 計算量 O(log N)
			- [l, r)にxを適用させた結果に変更する

		- insert(size_type k, value_type v)
			- 計算量 O(log N)
			- k番目にvを挿入する

		- erase(size_type k)
			- 計算量 O(log N)
			- k番目の要素を削除する

		- dump(size_type l, size_type r) -> vector<value_type>
			- 計算量 O(| [l, r) |)
			- [l, r)の要素を列として返す

		- insert_from_iterator(InputIterator first, InputIterator last, size_type k)
			- 計算量 O((last-first) log N)
			- iteratorのfirstからlastまでの要素をk番目から挿入する
			- kが未指定の場合は末尾

		- kth_element(size_type k) -> value_type
			- 計算量 O(log N)
			- k番目の要素を返す


		* 未実装
		- search(function f) -> size_type
			- 計算量 O(log N)?
			- f([0, k)) is true and f([0, k+1)) is falseとなるkを返す
*/
