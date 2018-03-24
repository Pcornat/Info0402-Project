#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#include <functional>
#include <initializer_list>

using namespace std;

/**
 * Implémentation par un arbre binaire équilibré (implémentation officielle du GNU : utilise arbre rouge-noir, pas
 * obligatoire ici)
 * @authors Florent Denef et Thomas Ducrot
 * @tparam Key type de donnée présent dans set
 * @tparam Compare fonctor de comparaison
 * @version 0.1
 */
template<class Key, class Compare=less<Key>>
class set {
public:
	// Tous les membres de la classe.
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
	set();

	/**
	 * Crée un set vide avec le comp correspondant.
	 * @param comp
	 */
	explicit set(const key_compare& comp = key_compare());

	/**
	 * Constructeur par copie
	 * @param s set à copier
	 */
	set(const set& s);

	/**
	 * Constructeur par déplacement
	 * @param s set à déplacer (voler) les données
	 */
	set(set&& s) noexcept;

	/**
	 * Constructeur par liste d'initialisation
	 * @param [in] il liste d'initialisation
	 * @param [in]comp comparateur
	 */
	set(initializer_list<value_type> il, const key_compare& comp = key_compare());

	/**
	 * Détruis l'objet.
	 */
	~set();

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
	set& operator=(const set& x);

	/**
	 * Assignation par déplacement.
	 * @param x objet dont on déplace les ressources.
	 * @return l'objet nouvellement créé.
	 */
	set& operator=(set&& x) noexcept;

	/**
	 * Assignation par liste.
	 * @param list liste à assigner
	 * @return l'objet créé par la liste.
	 */
	set& operator=(initializer_list<value_type> list);

	bool operator==(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs);
};

template<class Key, class Compare>
set<Key, Compare>::set() : size(0), racine(nullptr) {}

template<class Key, class Compare>
set<Key, Compare>::set(const key_compare& comp) : racine(nullptr), size(0), key_comp(comp), value_comp(comp) {

}

template<class Key, class Compare>
set<Key, Compare>::set(initializer_list<value_type> il, const key_compare& comp) : size(il.size()), key_comp(comp),
																				   value_comp(comp) {

}

template<class Key, class Compare>
set<Key, Compare>::set(set&& s) noexcept : size(s.size), racine(s.racine), key_comp(s.key_comp),
										   value_comp(s.value_comp) {
	s.size = 0;
	s.racine = nullptr;
	key_comp = nullptr;
	value_comp = nullptr;
}

template<class Key, class Compare>
set<Key, Compare>::set(const set& s) {

}

template<class Key, class Compare>
set& set<Key, Compare>::operator=(const set& x) {
	if (this != &x) {

	}
	return *this;
}

template<class Key, class Compare>
set& set<Key, Compare>::operator=(set&& x) noexcept {
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
set& set<Key, Compare>::operator=(initializer_list<value_type> list) {
	return *this;
}

template<class Key, class Compare>
set<Key, Compare>::~set() {

}

template<class Key, class Compare>
set::node* set<Key, Compare>::parent(const set::node* n) {
	return n->parent;
}

template<class Key, class Compare>
set::node* set<Key, Compare>::gauche(const set::node* n) {
	return n->gauche;
}

template<class Key, class Compare>
set::node* set<Key, Compare>::droit(const set::node* n) {
	return n->droit;
}

template<class Key, class Compare>
bool set<Key, Compare>::empty() const noexcept {
	return (size == 0);
}

template<class Key, class Compare>
reference set<Key, Compare>::find(const key_type& key) {
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
bool set<Key, Compare>::insert(const value_type& value) {
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
bool set<Key, Compare>::operator==(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) {
	if (lhs.size != rhs.size)
		return false;

	return true;
}


#endif //PROJET_SET_HPP
