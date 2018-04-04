#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#include "test-set.hpp"
#include <functional>
#include <initializer_list>

using namespace std;

template<class Key, class Compare=less<Key>>
class SetIter;

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

public:
	friend class SetIter<Key, Compare>;

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
	typedef enum {
		noir, rouge
	} color;
	/**
	 * Structure de nœud pour l'arbre binaire de recherche (en interne pour éviter de s'en servir en dehors de la classe)
	 */
	typedef struct node_t {
		key_type key;
		value_type value;
		color couleur;
		unique_ptr<struct node_t> parent = nullptr;
		unique_ptr<struct node_t> gauche = nullptr;
		unique_ptr<struct node_t> droit = nullptr;

		node_t() noexcept = default;

		explicit node_t(key_type key, struct node_t* parent = nullptr, color couleur = noir) : key(key),
																							   couleur(couleur),
																							   parent(parent) {
		}

		/**
		 * Constructeur par copie du nœud.
		 * @param n nœud à copier.
		 */
		node_t(const node_t& n) : key(n.key), value(n.value), couleur(n.couleur), parent(nullptr),
								  gauche(nullptr),
								  droit(nullptr) {
		}

		/**
		 * Destructeur (appellera automatiquement les destructeurs des unique_ptr.
		 */
		~node_t() noexcept = default;

		/**
		 * Donne le parent du nœud courant
		 * @return pointeur sur le nœud correspondant au parent
		 */
		unique_ptr<struct node_t>& pere() {
			return this->parent;
		}

		/**
		 * Donne le fils droit du nœud courant
		 * @return pointeur sur le nœud correspondant au fils droit
		 */
		unique_ptr<struct node_t>& filsDroit() {
			return this->droit;
		}

		/**
		 * Donne le fils gauche du nœud courant
		 * @return pointeur sur le nœud correspondant au fils gauche
		 */
		unique_ptr<struct node_t>& filsGauche() {
			return this->gauche;
		}

		/**
		 * Permet d'avoir le grand-parent du nœud.
		 * @return le grand-parent du nœud courant.
		 */
		unique_ptr<struct node_t>& grandParent() {
			return this->parent->parent;
		}

		/**
		 * Permet d'avoir l'oncle du nœud courant.
		 * @return l'oncle du nœud courant.
		 */
		unique_ptr<struct node_t>& oncle() {
			struct node_t* x = this->grandParent().get();
			if (x->filsGauche() == this->pere())
				return x->filsDroit();
			else if (x->filsDroit() == this->pere())
				return x->filsGauche();
		}
	} node;

	unique_ptr<node> racine;
	size_type size;

public:
	/**
	 * Crée un set vide
	 */
	Set() : size(0), racine() {}

	/**
	 * Crée un set vide avec le comp correspondant.
	 * @param comp
	 */
	explicit Set(const key_compare& comp = key_compare()) : racine(nullptr), size(0), key_comp(comp), value_comp(comp) {

	}

	/**
	 * Construit un set avec le nombre d'élément compris entre [first, last) avec chaque élément construit emplace
	 * correspondant à son élement dans [first, last)
	 * @tparam InputIterator
	 * @param first
	 * @param last
	 * @param comp
	 */
	template<class InputIterator>
	Set(InputIterator first, InputIterator last, key_compare& comp = key_compare()) : size(0), key_comp(comp),
																					  value_comp(comp) {

	}

	/**
	 * Constructeur par copie.
	 * @param s Set compatible à copier.
	 */
	Set(const Set<Key, Compare>& s) : size(s.getSize()), key_comp(s.key_comp), value_comp(value_comp) {

	}

	/**
	 * Constructeur par déplacement
	 * @param s set à déplacer (voler) les données
	 */
	Set(Set<Key, Compare>&& s) noexcept : size(s.size), racine(move(s.racine)), key_comp(s.key_comp),
										  value_comp(s.value_comp) {
		s.size = 0;
		s.racine = nullptr;
		key_comp = nullptr;
		value_comp = nullptr;
	}

	/**
	 * Constructeur par liste d'initialisation
	 * @param [in] il liste d'initialisation
	 * @param [in]comp comparateur
	 */
	Set(initializer_list<value_type> il, const key_compare& comp = key_compare()) : size(il.size()), key_comp(comp),
																					value_comp(comp) {

	}

	/**
	 * Détruis l'objet.
	 */
	~Set() noexcept = default;

	/**
	 * Itérateur de début du Set.
	 * @return retourne un itérateur sur le début du Set.
	 */
	iterator begin() noexcept {
		return iterator(*this);
	}

	/**
	 * Itérateur de fin du Set.
	 * @return itérateur sur le dernier nœud.
	 */
	iterator end() noexcept {
		return iterator(*this, racine.get()); //Normalement renverra l'itérateur sur le dernier noeud.
	}

	/**
	 * Vérifie si le conteneur est vide.
	 * @return true si il est vide.
	 */
	bool empty() const noexcept {
		return (size == 0);
	}

	/**
	 * Getter de size
	 * @return une référence constante sur la taille de l'objet.
	 */
	size_t& getSize() const noexcept {
		return this->size;
	}

	/**
	 * Méthode permettant de rechercher un élément dans le set.
	 * @param key clé à trouver.
	 * @return normalement retourne un itérateur sur l'élément
	 */
	iterator find(const key_type& key) {
		node* x = racine.get();
		while (x != nullptr && key != x->key) {
			if (key_comp(key, x->key))
				x = x->filsGauche().get();
			else
				x = x->filsDroit().get();
		}
		return x->key; //Normalement renvoie un itérateur et non une référence sur la clé.
	}

	/**
	 * Implémentation pas sûre, notamment en utilisant le comparateur (key_comp)
	 * @param key
	 * @return true : réussite de l'insertion.
	 */
	bool insert(const value_type& value) {
		if (find(value).currentNode != nullptr) {
			return false;
		}
		node* y = nullptr, * x = racine.get(), * z = new node(value);
		while (x != nullptr) {
			y = x;
			if (key_comp(z->key, x->key))
				x = x->filsGauche().get();
			else
				x = x->filsDroit().get();
			x = x->filsDroit().get();
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

	/**
	 * Assignation par copie.
	 * @param x objet à copier.
	 * @return l'objet copié
	 */
	Set& operator=(const Set& x) {
		if (this != &x) {

		}
		return *this;
	}

	/**
	 * Assignation par déplacement.
	 * @param x objet dont on déplace les ressources.
	 * @return l'objet nouvellement créé.
	 */
	Set& operator=(Set&& x) noexcept {
		if (this != &x) {
			size = x.size;
			racine = move(x.racine);
			key_comp = x.key_comp;
			value_comp = x.value_comp;
			x.value_comp = x.key_comp = nullptr;
			x.size = 0;
		}
		return *this;
	}

	/**
	 * Assignation par liste.
	 * @param list liste à assigner
	 * @return l'objet créé par la liste.
	 */
	Set& operator=(initializer_list<value_type> list) {
		return *this;
	}

	/**
	 * Opérateur de comparaison. La comparaison est valide uniquement si les deux Set possèdent les mêmes valeurs au
	 * même endroit de l'arbre.
	 * @param rhs Set à comparer avec *this
	 * @return true : les deux sont égaux
	 */
	bool operator==(const Set<Key, Compare>& rhs) {
		if (this->size != rhs.size)
			return false;

		return true;
	}
};

/**
 * Itérateur de Set.
 * @authors Florent Denef, Thomas Ducrot
 * @tparam Key type de clé
 * @tparam Compare foncteur de comparaison
 */
template<class Key, class Compare>
class SetIter {
	friend class Set<Key, Compare>;

private:
	Set<Key, Compare>& myset;
	size_t size;
	typename Set<Key, Compare>::node* currentNode;
public:
	/**
	 * Constructeur par "défaut"
	 * @param myset
	 */
	SetIter(Set<Key, Compare>& myset) : myset(myset), currentNode(myset.racine.get()), size(myset.getSize()) {}

	/**
	 * Constructeur de l'itérateur de Set.
	 * @param myset
	 * @param noeud
	 */
	SetIter(Set<Key, Compare>& myset, typename Set<Key, Compare>::node* noeud) : myset(myset), size(myset.getSize()),
																				 currentNode(noeud) {
	}

	/**
	 * Destructeur d'itérateur de Set.
	 */
	virtual ~SetIter() {
	}

	/**
	 * Opérateur de comparaison d'itérateur.
	 * @param rhs
	 * @return
	 */
	bool operator==(const SetIter& rhs) const {
		return this->currentNode == rhs.currentNode;
	}

	bool operator!=(const SetIter& rhs) const {
		return !(rhs == *this);
	}

	typename Set<Key, Compare>::value_type& operator*() const {
		return currentNode->value;
	};
};

#endif //PROJET_SET_HPP
