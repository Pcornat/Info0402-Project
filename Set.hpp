#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#include <functional>
#include <initializer_list>
#include <memory>


using namespace std;

template<typename Key, typename Compare>
class SetIter;

/**
 * Implémentation par un arbre binaire équilibré, ici rouge-noir.
 * @authors Florent Denef, Thomas Ducrot
 * @tparam Key Type de donnée présent dans set
 * @tparam Compare Type de la fonction de comparaison
 * @version 0.3
 */
template<typename Key, typename Compare=less<Key>>
class Set {
	friend class SetIter<Key, Compare>;
/**
 * @publicsection
 */
public:

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
/**
 * @privatesection
 */
private:
	using color = enum {
		noir, rouge
	};

	/**
	 * Structure de nœud pour l'arbre binaire de recherche (en privée pour éviter de s'en servir en dehors de la classe)
	 */
	using node = struct node_t {
	private:
		shared_ptr<node_t> gauche;
		shared_ptr<node_t> droit;
		shared_ptr<node_t> parent;

	public:
		//key_type* key = nullptr;
		unique_ptr<key_type> key;
		color couleur = noir;

		/**
		 * Constructeur par défaut : shared_ptr à nullptr, couleur à noir. Utilisé uniquement par tnil.
		 */
		node_t() noexcept = default;

		/**
		 * Constructeur le plus utilisé.
		 * @param [in]key Clé pour le nœud.
		 * @param [in]parent Le parent du nœud.
		 * @param [in]couleur Couleur du nœud.
		 */
		explicit node_t(key_type key, shared_ptr<node_t> parent, color couleur = noir) :
				key(new key_type(key)), couleur(couleur), parent(parent), gauche(parent), droit(parent) {}

		/**
		 * Constructeur par copie du nœud.
		 * @param [in]n nœud à copier.
		 */
		explicit node_t(const node_t& n) : key(n.key), couleur(n.couleur), parent(nullptr),
										   gauche(nullptr),
										   droit(nullptr) {}

		/**
		 * Constructeur par déplacement d'un nœud.
		 * @param [in]n nœud à déplacer.
		 */
		explicit node_t(node_t&& n) noexcept : key(move(n.key)), couleur(move(n.couleur)),
											   parent(move(n.parent)),
											   gauche(move(n.gauche)), droit(move(n.droit)) {}

		/**
		 * Destructeur (appellera automatiquement les destructeurs des unique_ptr et shared_ptr.
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
		shared_ptr<node_t>& filsDroit() {
			return this->droit;
		}

		/**
		 * Donne le fils gauche du nœud courant
		 * @return pointeur sur le nœud correspondant au fils gauche
		 */
		shared_ptr<node_t>& filsGauche() {
			return this->gauche;
		}

		/**
		 * Permet d'avoir le grand-parent du nœud.
		 * @return le grand-parent du nœud courant.
		 */
		shared_ptr<node_t>& grandParent() {
			return this->pere()->pere();
		}
	};

	/**
	 * Insertion du nœud dans l'arbre, algorithme prit dans le livre <a href="https://fr.wikipedia.org/wiki/Introduction_%C3%A0_l%27algorithmique">"Introduction to algorithm, third edition".</a>
	 * @param [in]z nœud à insérer.
	 */
	void insert_rd_tree(shared_ptr<node>& z) {
		shared_ptr<node> x(this->racine), y(this->tnil);
		while (x != this->tnil) {
			y = x;
			if (keyComp(*z->key, *x->key))
				x = x->filsGauche();
			else
				x = x->filsDroit();
		}
		z->pere() = y;
		if (y == this->tnil)
			this->racine = z;
		else if (keyComp(*z->key, *y->key))
			y->filsGauche() = z;
		else
			y->filsDroit() = z;
		z->filsGauche() = this->tnil;
		z->filsDroit() = this->tnil;
		z->couleur = rouge;
		insert_repair_tree(z);
	}

	/**
	 * Algorithme provenant du livre "Introduction to algorithm, third edition".
	 * @param [in]z Le nœud courant à partir duquel on répare l'arbre.
	 */
	void insert_repair_tree(shared_ptr<node>& z) {
		while (z->pere()->couleur == rouge) {
			//z = nullptr signifie que c'est une feuille, or les feuilles sont noire !
			if (z->pere() == z->grandParent()->filsGauche()) {
				shared_ptr<node> y = z->grandParent()->filsDroit();
				if (y->couleur == rouge) {
					//y = nullptr signifie que c'est une feuille, or les feuilles sont noire !
					z->pere()->couleur = noir;
					y->couleur = noir;
					z->grandParent()->couleur = rouge;
					z = z->grandParent();
				} else {
					if (z == z->pere()->filsDroit()) {
						z = z->pere();
						rotate_left(z);
					}
					z->pere()->couleur = noir;
					z->grandParent()->couleur = rouge;
					rotate_right(z->grandParent());
				}
			} else {
				shared_ptr<node> y = z->grandParent()->filsGauche();
				if (y->couleur == rouge) {
					//y = nullptr signifie que c'est une feuille, or les feuilles sont noire !
					z->pere()->couleur = noir;
					y->couleur = noir;
					z->grandParent()->couleur = rouge;
					z = z->grandParent();
				} else {
					if (z == z->pere()->filsGauche()) {
						z = z->pere();
						rotate_right(z);
					}
					z->pere()->couleur = noir;
					z->grandParent()->couleur = rouge;
					rotate_left(z->grandParent());
				}
			}
		}
		this->racine->couleur = noir;
	}

	/**
	 * Rotation gauche. La condition supposée de l'algorithme du livre est que x.right != T.nil.
	 * @param [in]x Nœud à partir duquel la rotation se fait.
	 */
	void rotate_left(shared_ptr<node>& x) {
		if (x->filsDroit() != this->tnil) {
			shared_ptr<node> y = x->filsDroit();
			x->filsDroit() = y->filsGauche();
			if (y->filsGauche() != this->tnil)
				y->filsGauche()->pere() = x;
			y->pere() = x->pere();
			if (x->pere() == this->tnil)
				this->racine = y;
			else if (x == x->pere()->filsGauche())
				x->pere()->filsGauche() = y;
			else
				x->pere()->filsDroit() = y;
			y->filsGauche() = x;
			x->pere() = y;
		}
	}

	/**
	 * Rotation droite. Même chose que la rotation gauche sauf que tous les mots "gauche" sont remplacés par droit, et vice-versa.
	 * @param [in]x Nœud à partir duquel la rotation se fait.
	 */
	void rotate_right(shared_ptr<node>& x) {
		if (x->filsGauche() != this->tnil) {
			shared_ptr<node> y = x->filsGauche();
			x->filsGauche() = y->filsDroit();
			if (y->filsDroit() != this->tnil)
				y->filsDroit()->pere() = x;
			y->pere() = x->pere();
			if (x->pere() == this->tnil)
				this->racine = y;
			else if (x == x->pere()->filsDroit())
				x->pere()->filsDroit() = y;
			else
				x->pere()->filsGauche() = y;
			y->filsDroit() = x;
			x->pere() = y;
		}
	}

	key_compare keyComp;
	value_compare valueComp;
	shared_ptr<node> tnil{new node};
	shared_ptr<node> racine{tnil};
	size_type size{};
/**
 * @publicsection
 */
public:
	/**
	 * Constructeur par défaut.
	 */
	Set() = default;

	/**
	 * Crée un set vide avec le comp correspondant.
	 * @param [in]comp
	 */
	explicit Set(const key_compare& comp) : keyComp(comp), valueComp(comp) {}

	/**
	 * Construit un set avec le nombre d'élément compris entre [first, last) avec chaque élément construit emplace
	 * correspondant à son élement dans [first, last)
	 * @tparam [in]InputIterator
	 * @param [in]first
	 * @param [in]last
	 * @param [in]comp
	 */
	template<typename InputIterator>
	Set(InputIterator first, InputIterator last, key_compare& comp = key_compare()) : size(0), keyComp(comp),
																					  valueComp(comp) {

	}

	/**
	 * Constructeur par copie.
	 * @param [in]s Set compatible à copier.
	 */
	Set(const Set& s) : size(s.getSize()), keyComp(s.keyComp), valueComp(valueComp) {

	}

	/**
	 * Constructeur par déplacement
	 * @param [in]s set à déplacer (voler) les données
	 */
	Set(Set&& s) noexcept : size(move(s.size)), racine(move(s.racine)), key_compare(move(s.keyComp)),
							valueComp((move(s.valueComp))) {}

	/**
	 * Constructeur par liste d'initialisation
	 * @param [in] il liste d'initialisation
	 * @param [in]comp comparateur
	 */
	Set(initializer_list<value_type> il, const key_compare& comp = key_compare()) : size(il.size()), keyComp(comp),
																					valueComp(comp) {}

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
	 * @return [out] Itérateur sur le dernier nœud.
	 */
	iterator end() noexcept {
		return iterator(*this, nullptr); //Normalement renverra l'itérateur sur le dernier noeud.
	}

	/**
	 * Vérifie si le conteneur est vide.
	 * @return [out] True si il est vide.
	 */
	bool empty() const noexcept {
		return (size == 0);
	}

	/**
	 * Getter de size
	 * @return [out] Une référence constante sur la taille de l'objet.
	 */
	size_type getSize() const noexcept {
		return this->size;
	}

	/**
	 * Méthode permettant de rechercher un élément dans le set.
	 * @param [in]key Clé à trouver.
	 * @return [out] Normalement retourne un itérateur sur l'élément
	 */
	iterator find(const key_type& key) {
		shared_ptr<node> x = this->racine;
		while (x != this->tnil && ((keyComp(key, *x->key) || keyComp(*x->key, key)))) {
			if (keyComp(key, *x->key))
				x = x->filsGauche();
			else
				x = x->filsDroit();
		}
		return iterator(*this, x.get());
	}

	/**
	 * Implémentation sûr. Si le comparateur est > au lieu de <, l'arbre sera juste inversée.
	 * @param [in]value
	 * @return [out]Une paire avec l'itérateur sur le nœud et un booléen si l'opération a réussi ou non.
	 */
	pair<iterator, bool> insert(const value_type& value) {
		if (find(value).currentNode != this->tnil.get()) {
			return pair<iterator, bool>(iterator(*this), false);
		}
		shared_ptr<node> n(new node(value, this->tnil));
		insert_rd_tree(n);
		++this->size;
		return pair<iterator, bool>(iterator(*this, n.get()), true);
	}

	pair<iterator, bool> insert(value_type&& value) {
		if (find(value).currentNode != this->tnil.get()) {
			return pair<iterator, bool>(iterator(*this), false);
		}
		shared_ptr<node> n(new node(value, this->tnil));
		insert_rd_tree(n);
		++this->size;
		return pair<iterator, bool>(iterator(*this, n.get()), true);
	}

	inline key_compare key_comp() const { return keyComp; }

	inline value_compare value_comp() const { return valueComp; }

	/**
	 * Assignation par copie.
	 * @param [in]x Objet à copier.
	 * @return [out] L'objet copié
	 */
	Set& operator=(const Set& x) {
		if (this != &x) {

		}
		return *this;
	}

	/**
	 * Assignation par déplacement.
	 * @param [in]x Objet dont on déplace les ressources.
	 * @return [out] L'objet nouvellement créé.
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
	 * @param [in]list Liste à assigner
	 * @return [out] L'objet créé par la liste.
	 */
	Set& operator=(initializer_list<value_type> list) { //Not tested yet.
		for (auto&& item  : list) {
			this->insert(item);
		}
		return *this;
	}

	/**
	 * Opérateur de comparaison. La comparaison est valide uniquement si les deux Set possèdent les mêmes valeurs au
	 * même endroit de l'arbre.
	 * @param [in]rhs Set à comparer avec *this
	 * @return [out]True : les deux sont égaux
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
 * @tparam [in]Key type de clé
 * @tparam [in]Compare foncteur de comparaison
 */
template<typename Key, typename Compare>
class SetIter {
	friend class Set<Key, Compare>;
/**
 * @privatesection
 */
private:
	Set<Key, Compare>& myset;
	size_t size{};
	typename Set<Key, Compare>::node* currentNode;
	typename Set<Key, Compare>::node* lastNode;

/**
 * @publicsection
 */
public:
	/**
	 *Constructeur par "défaut"
	 * @param [in]myset
	 */
	explicit SetIter(Set<Key, Compare>& myset) : myset(myset), currentNode(myset.racine.get()),
												 size(myset.getSize()) {
		if (currentNode != nullptr) {
			this->lastNode = currentNode->pere().get();
		}
	}

	/**
 	 * Constructeur de l'itérateur de Set.
	 * @param [in]myset
	 * @param [in]noeud
	 */
	explicit SetIter(Set<Key, Compare>& myset, typename Set<Key, Compare>::node* noeud) : myset(myset),
																						  size(myset.getSize()),
																						  currentNode(noeud) {
		if (currentNode != nullptr) {
			this->lastNode = currentNode->pere().get();
		}
	}

	/**
	 * Constructeur par copie.
	 * @param [in]setIter
	 */
	SetIter(const SetIter<Key, Compare>& setIter) : myset(setIter.myset), size(setIter.size),
													currentNode(setIter.currentNode) {}

	/**
	 * Destructeur d'itérateur de Set.
	 */
	virtual ~SetIter() = default;

	/**
	 * Opérateur de comparaison d'itérateur.
	 * @param [in]rhs Itérateur à comparer avec.
	 * @return [out] True s'ils sont égaux, sinon false.
	 */
	bool operator==(const SetIter& rhs) const {
		return this->currentNode == rhs.currentNode;
	}

	/**
	 * Opérateur de comparaison d'itérateur.
	 * @param [in]rhs Itérateur à comparer avec.
	 * @return [out] True s'ils sont différents, sinon false.
	 */
	bool operator!=(const SetIter& rhs) const {
		return (rhs.currentNode != this->currentNode) && (rhs.lastNode != this->lastNode);
	}

	typename Set<Key, Compare>::reference operator*() const {
		return *this->currentNode->key;
	}

	typename Set<Key, Compare>::iterator operator++() {

	}

	typename Set<Key, Compare>::iterator operator++(int) {

	}
};


#endif //PROJET_SET_HPP
