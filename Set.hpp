#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#include <functional>
#include <initializer_list>

using namespace std;

/**
 * Aller, c'est partiiii :D
 * Construction d'abord sans template, test ? go template : debug.
 * Implémentation par un arbre rouge-noir (snif :'()
 */
template<class T, class Compare=less<T>>
class set {
private:
	/**
	 * Tous les membres de la classe.
	 */
	typedef T key_type;
	typedef T value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	//difference_type
	typedef size_t size_type;
	typedef enum {
		noir, rouge
	} couleur;

	/**
	 * Structure de nœud pour l'arbre (en interne pour éviter de s'en servir en dehors de la classe)
	 */
	typedef struct node_t {
		key_type key;
		value_type value;
		couleur color;
		struct node_t* parent;
		struct node_t* gauche;
		struct node_t* droit;
	} node;

	node* parent(const node* n);

	node* gauche(const node* n);

	node* droit(const node* n);

	node* racine;
	size_type size;
	key_compare key_comp;
	value_compare value_comp;

public:
	explicit set(const key_compare& comp = key_compare());

	set(const set& s);

	set(set&& s) noexcept;

	set(initializer_list<value_type> il, const key_compare& comp = key_compare());

	~set();

	set& operator=(const set& x);

	set& operator=(set&& x) noexcept;

	set& operator=(initializer_list<value_type> list);
};

template<class T, class Compare>
set<T, Compare>::set(const Compare& comp) : racine(nullptr), size(0), key_comp(comp), value_comp(comp) {

}

template<class T, class Compare>
set<T, Compare>::set(initializer_list<value_type> il, const Compare& comp) {

}

template<class T, class Compare>
set<T, Compare>::set(set&& s) noexcept {

}

template<class T, class Compare>
set<T, Compare>::set(const set& s) {

}

template<class T, class Compare>
set& set<T, Compare>::operator=(const set& x) {
	if (this != &x) {

	}
	return *this;
}

template<class T, class Compare>
set& set<T, Compare>::operator=(set&& x) noexcept {
	if (this != &x) {

	}
	return *this;
}

template<class T, class Compare>
set& set<T, Compare>::operator=(initializer_list<value_type> list) {
	return *this;
}

template<class T, class Compare>
set<T, Compare>::~set() {

}

template<class T, class Compare>
set::node* set<T, Compare>::parent(const set::node* n) {
	return n->parent;
}

template<class T, class Compare>
set::node* set<T, Compare>::gauche(const set::node* n) {
	return n->gauche;
}

template<class T, class Compare>
set::node* set<T, Compare>::droit(const set::node* n) {
	return n->droit;
}


#endif //PROJET_SET_HPP
