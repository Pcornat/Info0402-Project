#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

//#include "test-set.hpp"
#include <functional>
#include <initializer_list>
#include <memory>

using namespace std;

template <class Key, class Compare=less<Key>>
class SetIter;

/**
 * Implémentation par un arbre binaire équilibré (implémentation officielle du GNU : utilise arbre rouge-noir, pas
 * obligatoire ici)
 * @authors Florent Denef, Thomas Ducrot
 * @tparam Key type de donnée présent dans set
 * @tparam Compare fonctor de comparaison
 * @version 0.1
 */
template <class Key, class Compare=less<Key>>
class Set {

public:
	friend class SetIter<Key, Compare>;

	// Tous les membres de la classe.
	using iterator =  SetIter<Key, Compare>;
	using key_type = Key;
	using value_type = Key;
	using key_compare = Compare;
	using value_compare = Compare;
	using pointer = Key*;
	using const_pointer = const Key*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = ptrdiff_t;
	using size_type = size_t;

private:
	using color = enum {
		noir, rouge
	};

	/**
	 * Structure de nœud pour l'arbre binaire de recherche (en interne pour éviter de s'en servir en dehors de la classe)
	 */
	using node = struct node_t {
		key_type key;
		value_type value;
		color couleur = noir;
		unique_ptr<node_t> gauche = nullptr;
		unique_ptr<node_t> droit = nullptr;
		shared_ptr<node_t> parent = nullptr;

		/**
		 * Constructeur par défaut : unque_ptr à nullptr, couleur à noir et les autres à 0.
		 */
		node_t() noexcept = default;

		explicit node_t(key_type key, node_t* parent = nullptr, color couleur = noir) : key(key),
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
		shared_ptr<node_t>& pere() {
			return this->parent;
		}

		/**
		 * Donne le fils droit du nœud courant
		 * @return pointeur sur le nœud correspondant au fils droit
		 */
		unique_ptr<node_t>& filsDroit() {
			return this->droit;
		}

		/**
		 * Donne le fils gauche du nœud courant
		 * @return pointeur sur le nœud correspondant au fils gauche
		 */
		unique_ptr<node_t>& filsGauche() {
			return this->gauche;
		}

		/**
		 * Permet d'avoir le grand-parent du nœud.
		 * @return le grand-parent du nœud courant.
		 */
		shared_ptr<node_t>& grandParent() {
			return this->parent->parent;
		}

		/**
		 * Permet d'avoir l'oncle du nœud courant.
		 * @return l'oncle du nœud courant.
		 */
		unique_ptr<node_t>& oncle() {
			node_t* x = this->grandParent().get();
			if (x->filsGauche().get() == this->pere())
				return x->filsDroit();
			else if (x->filsDroit().get() == this->pere())
				return x->filsGauche();
		}
	};

	/**
	 * Insertion récursive du nœud dans l'arbre.
	 * @param root pointeur sur la racine courante du sous-arbre.
	 * @param n nœud à insérer.
	 */
	void insertRecurse(unique_ptr<node>& root, node* n) {
		// recursively descend the tree until a leaf is found
		if (root != nullptr && n->key < root->key) {
			if (root->gauche != nullptr) {
				insertRecurse(root->gauche, n);
				return;
			} else
				root->gauche = n;
		} else if (root != nullptr) {
			if (root->droit != nullptr) {
				insertRecurse(root->droit, n);
				return;
			} else
				root->droit = n;
		}

		// insert new node n
		n->parent = root.get();
		n->gauche = nullptr;
		n->droit = nullptr;
		n->couleur = rouge;
	}

	key_compare keyComp;
	value_compare valueComp;
	unique_ptr<node> racine = nullptr;
	size_type size = 0;

public:
	/**
	 * Constructeur par défaut.
	 */
	Set() = default;

	/**
	 * Crée un set vide avec le comp correspondant.
	 * @param comp
	 */
	explicit Set(const key_compare& comp) : keyComp(comp), valueComp(comp) {}

	/**
	 * Construit un set avec le nombre d'élément compris entre [first, last) avec chaque élément construit emplace
	 * correspondant à son élement dans [first, last)
	 * @tparam InputIterator
	 * @param first
	 * @param last
	 * @param comp
	 */
	template <typename InputIterator>
	Set(InputIterator first, InputIterator last, key_compare& comp = key_compare()) : size(0), keyComp(comp),
																					  valueComp(comp) {

	}

	/**
	 * Constructeur par copie.
	 * @param s Set compatible à copier.
	 */
	Set(const Set& s) : size(s.getSize()), keyComp(s.keyComp), valueComp(valueComp) {

	}

	/**
	 * Constructeur par déplacement
	 * @param s set à déplacer (voler) les données
	 */
	Set(Set&& s) noexcept : size(s.size), racine(move(s.racine)), key_compare(s.keyComp),
							valueComp(s.valueComp) {
		s.size = 0;
		s.racine = nullptr;
		keyComp = nullptr;
		valueComp = nullptr;
	}

	/**
	 * Constructeur par liste d'initialisation
	 * @param [in] il liste d'initialisation
	 * @param [in]comp comparateur
	 */
	Set(initializer_list<value_type> il, const key_compare& comp = key_compare()) : size(il.size()), keyComp(comp),
																					valueComp(comp) {

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
	size_type getSize() const noexcept {
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
			if (key_compare(key, x->key))
				x = x->filsGauche().get();
			else
				x = x->filsDroit().get();
		}
		return x->key; //Normalement renvoie un itérateur et non une référence sur la clé.
	}

	/**
	 * Implémentation pas sûre, notamment en utilisant le comparateur (key_comp)
	 * @param value
	 * @return une paire avec l'itérateur sur le nœud et un booléen si l'opération a réussi ou non.
	 */
	pair<iterator, bool> insert(const value_type& value) {
		if (find(value).currentNode != nullptr) {
			return pair<iterator, bool>(iterator(*this, nullptr), false);
		}
		node* y = nullptr, * x = racine.get(), * z = new node(value);
		while (x != nullptr) {
			y = x;
			if (keyComp(z->key, x->key))
				x = x->filsGauche().get();
			else
				x = x->filsDroit().get();
			x = x->filsDroit().get();
		}
		z->parent = y;
		if (y == nullptr)
			racine = z;
		else {
			if (keyComp(z->key, y->key))
				y->gauche = z;
			else
				y->droit = z;
		}
		++size;
		return pair<iterator, bool>(iterator(*this, z), true);
	}

	inline key_compare key_comp() const { return keyComp; }

	inline value_compare value_comp() const { return valueComp; }

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
			keyComp = x.keyComp;
			valueComp = x.valueComp;
			//x.valueComp = x.keyComp = nullptr; //Pas nécessaire.
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
template <class Key, class Compare>
class SetIter {
	friend class Set<Key, Compare>;

private:
	Set<Key, Compare>& myset;
	size_t size{};
	typename Set<Key, Compare>::node* currentNode;
public:
	/**
	 * Constructeur par "défaut"
	 * @param myset
	 */
	explicit SetIter(Set<Key, Compare>& myset) : myset(myset), currentNode(myset.racine.get()), size(myset.getSize()) {}

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
