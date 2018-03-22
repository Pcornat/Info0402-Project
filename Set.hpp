#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#include <functional>
#include <initializer_list>

using namespace std;

/**
 * Construction d'abord sans template, test ? go template : debug.
 * Implémentation par un arbre binaire équilibré (implémentation officielle dans g++ : utilise arbre rouge-noir, pas
 * obligatoire ici)
 * @authors Florent Denef et Thomas Ducrot
 * @tparam T type de donnée présent dans set
 * @tparam Compare fonctor de comparaison
 * @version 0.1
 */
template<class T, class Compare=less<T>>
class set {
public:
	// Tous les membres de la classe.
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
private:
	/**
	 * Structure de nœud pour l'arbre (en interne pour éviter de s'en servir en dehors de la classe)
	 */
	typedef struct node_t {
		key_type key;
		value_type value;
		struct node_t* parent;
		struct node_t* gauche;
		struct node_t* droit;
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
	size_type size{};
	key_compare key_comp;
	value_compare value_comp;

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
};

template<class T, class Compare>
set<T, Compare>::set() : size(0), racine(nullptr) {}

template<class T, class Compare>
set<T, Compare>::set(const Compare& comp) : racine(nullptr), size(0), key_comp(comp), value_comp(comp) {

}

template<class T, class Compare>
set<T, Compare>::set(initializer_list<value_type> il, const Compare& comp) : size(il.size()), key_comp(comp),
																			 value_comp(comp) {

}

template<class T, class Compare>
set<T, Compare>::set(set&& s) noexcept : size(s.size), racine(s.racine), key_comp(s.key_comp),
										 value_comp(s.value_comp) {
	s.size = 0;
	s.racine = nullptr;
	key_comp = nullptr;
	value_comp = nullptr;
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
		size = x.size;
		racine = x.racine;
		key_comp = x.key_comp;
		value_comp = x.value_comp;
		x.value_comp = x.key_comp = x.racine = nullptr;
		x.size = 0;
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

template<class T, class Compare>
bool set<T, Compare>::empty() const noexcept {
	return (size == 0);
}


#endif //PROJET_SET_HPP
