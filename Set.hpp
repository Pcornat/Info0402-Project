#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#define CATCH_CONFIG_MAIN

#include <catch/catch.hpp>
#include <functional>
#include <initializer_list>
#include "SetIter.hpp"

using namespace std;

/**
 * Implémentation par un arbre binaire équilibré (implémentation officielle du GNU : utilise arbre rouge-noir, pas
 * obligatoire ici)
 * @authors Florent Denef, Thomas Ducrot
 * @tparam Key type de donnée présent dans set
 * @tparam Compare fonctor de comparaison
 * @version 0.1
 */
template<class Key, class Compare=less<Key>>
class Set {
	friend class SetIter<Key, Compare>;

public:
	// Tous les membres de la classe.
	typedef SetIter<Key, Compare> iterator;
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
	typedef Key* pointer;
	typedef const Key* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

	key_compare key_comp;
	value_compare value_comp;
private:
	/**
	 * Structure de nœud pour l'arbre binaire de recherche (en interne pour éviter de s'en servir en dehors de la classe)
	 */
	typedef struct node_t {
		key_type key;
		value_type value;
		struct node_t* parent = nullptr;
		struct node_t* gauche = nullptr;
		struct node_t* droit = nullptr;
	} node;

	/**
	 * Donne le parent du nœud courant
	 * @param n le nœud courant
	 * @return pointeur sur le nœud correspondant au parent
	 */
	node* parent(const node* n);

	/**
	 * Donne le fils gauche du nœud courant
	 * @param n le nœud courant
	 * @return pointeur sur le nœud correspondant au fils gauche
	 */
	node* gauche(const node* n);

	/**
	 * Donne le fils droit du nœud courant
	 * @param n le nœud courant
	 * @return pointeur sur le nœud correspondant au fils droit
	 */
	node* droit(const node* n);

	node* racine;
	size_type size;

public:
	/**
	 * Crée un set vide
	 */
	Set();

	/**
	 * Crée un set vide avec le comp correspondant.
	 * @param comp
	 */
	explicit Set(const key_compare& comp = key_compare());

	/**
	 * Constructeur par copie
	 * @param s set à copier
	 */
	Set(const Set& s);

	/**
	 * Constructeur par déplacement
	 * @param s set à déplacer (voler) les données
	 */
	Set(Set&& s) noexcept;

	/**
	 * Constructeur par liste d'initialisation
	 * @param [in] il liste d'initialisation
	 * @param [in]comp comparateur
	 */
	Set(initializer_list<value_type> il, const key_compare& comp = key_compare());

	/**
	 * Détruis l'objet.
	 */
	~Set();

	/**
	 * Vérifie si le conteneur est vide.
	 * @return true si il est vide.
	 */
	bool empty() const noexcept;

	/**
	 * Méthode permettant de rechercher un élément dans le set.
	 * @param key clé à trouver.
	 * @return normalement retourne un itérateur sur l'élément
	 */
	reference find(const key_type& key);

	/**
	 * Implémentation pas sûre, notamment en utilisant le comparateur (key_comp)
	 * @param key
	 * @return
	 */
	bool insert(const value_type& value);

	/**
	 * Assignation par copie.
	 * @param x objet à copier.
	 * @return l'objet copié
	 */
	Set& operator=(const Set& x);

	/**
	 * Assignation par déplacement.
	 * @param x objet dont on déplace les ressources.
	 * @return l'objet nouvellement créé.
	 */
	Set& operator=(Set&& x) noexcept;

	/**
	 * Assignation par liste.
	 * @param list liste à assigner
	 * @return l'objet créé par la liste.
	 */
	Set& operator=(initializer_list<value_type> list);

	bool operator==(const Set<Key, Compare>& lhs, const Set<Key, Compare>& rhs);
};

template<class Key, class Compare>
Set<Key, Compare>::Set() : size(0), racine(nullptr) {}

template<class Key, class Compare>
Set<Key, Compare>::Set(const key_compare& comp) : racine(nullptr), size(0), key_comp(comp), value_comp(comp) {

}

template<class Key, class Compare>
Set<Key, Compare>::Set(initializer_list<value_type> il, const key_compare& comp) : size(il.size()), key_comp(comp),
																				   value_comp(comp) {

}

template<class Key, class Compare>
Set<Key, Compare>::Set(Set&& s) noexcept : size(s.size), racine(s.racine), key_comp(s.key_comp),
										   value_comp(s.value_comp) {
	s.size = 0;
	s.racine = nullptr;
	key_comp = nullptr;
	value_comp = nullptr;
}

template<class Key, class Compare>
Set<Key, Compare>::Set(const Set& s) {

}

template<class Key, class Compare>
Set& Set<Key, Compare>::operator=(const Set& x) {
	if (this != &x) {

	}
	return *this;
}

template<class Key, class Compare>
Set& Set<Key, Compare>::operator=(Set&& x) noexcept {
	if (this != &x) {
		size = x.size;
		racine = x.racine;
		key_comp = x.key_comp;
		value_comp = x.value_comp;
		x.value_comp = x.key_comp = x.racine = nullptr;
		x.size = 0;
	}
	return *this;
}

template<class Key, class Compare>
Set& Set<Key, Compare>::operator=(initializer_list<value_type> list) {
	return *this;
}

template<class Key, class Compare>
Set<Key, Compare>::~Set() {

}

template<class Key, class Compare>
Set::node* Set<Key, Compare>::parent(const Set::node* n) {
	return n->parent;
}

template<class Key, class Compare>
Set::node* Set<Key, Compare>::gauche(const Set::node* n) {
	return n->gauche;
}

template<class Key, class Compare>
Set::node* Set<Key, Compare>::droit(const Set::node* n) {
	return n->droit;
}

template<class Key, class Compare>
bool Set<Key, Compare>::empty() const noexcept {
	return (size == 0);
}

template<class Key, class Compare>
reference Set<Key, Compare>::find(const key_type& key) {
	node* x = racine;
	while (x != nullptr && key != x->key) {
		if (key_comp(key, x->key))
			x = gauche(x);
		else
			x = droit(x);
	}
	return x->key; //Normalement renvoie un itérateur et non une référence sur la clé.
}

template<class Key, class Compare>
bool Set<Key, Compare>::insert(const value_type& value) {
	if (find(value) != nullptr) {
		return false;
	}
	node* y = nullptr, * x = racine, * z = new node;
	z->key = z->value = value;
	z->parent = z->droit = z->gauche = nullptr;
	while (x != nullptr) {
		y = x;
		if (key_comp(z->key, x->key))
			x = gauche(x);
		else
			x = droit(x);
	}
	z->parent = y;
	if (y == nullptr)
		racine = z;
	else {
		if (key_comp(z->key, y->key))
			y->gauche = z;
		else
			y->droit = z;
	}
	++size;
	return true;
}

template<class Key, class Compare>
bool Set<Key, Compare>::operator==(const Set<Key, Compare>& lhs, const Set<Key, Compare>& rhs) {
	if (lhs.size != rhs.size)
		return false;

	return true;
}


#endif //PROJET_SET_HPP
