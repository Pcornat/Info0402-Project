#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

#include <functional>
#include <initializer_list>
#include <memory>
#include <iostream>


//using namespace std;

template<typename Key, typename Compare>
class SetIter;

/**
 * @class Set
 * Implémentation par un arbre binaire équilibré, ici rouge-noir.
 * @authors Florent Denef, Thomas Ducrot
 * @tparam Key Type de donnée présent dans set
 * @tparam Compare Type de la fonction de comparaison
 * @version 0.3
 * @todo Tout ce qui concerne la copie de l'objet. La suppression d'éléments. Le swap
 */
template<typename Key, typename Compare=std::less<Key>>
class Set {
	friend class SetIter<Key, Compare>;
/**
 * @publicsection Types publics.
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
 * @privatesection Structure de nœud et l'énumération pour la couleur.
 */
private:
	/**
	 * @enum color
	 * Enum pour la couleur des nœuds de l'arbre.
	 */
	using color = enum {
		noir, rouge
	};

	/**
	 * @struct node_t
	 * Structure de nœud pour l'arbre binaire de recherche.
	 */
	using node = struct node_t {
	public:
		node_t* filsGauche;
		node_t* filsDroit;
		node_t* pere;
		std::unique_ptr<key_type> key;
		color couleur = noir;

		/**
		 * Constructeur par défaut : shared_ptr à nullptr, couleur à noir. Utilisé uniquement par tnil.
		 */
		node_t() noexcept = default;

		/**
		 * Constructeur le plus utilisé.
		 * @param [in]key Clé pour le nœud.
		 * @param [in]pere Le parent du nœud.
		 * @param [in]couleur Couleur du nœud.
		 */
		explicit node_t(reference key, node_t* pere) :
				key(new key_type(key)), couleur(noir), pere(pere), filsGauche(pere), filsDroit(pere) {}

		/**
		 * Constructeur par copie du nœud.
		 * @param [in]n Nœud à copier.
		 */
		explicit node_t(const node_t& n) :
				key(n.key), couleur(n.couleur), pere(nullptr), filsGauche(nullptr), filsDroit(nullptr) {}

		/**
		 * Constructeur par déplacement d'un nœud.
		 * @param [in]n Nœud à déplacer.
		 */
		explicit node_t(node_t&& n) noexcept : key(std::move(n.key)), couleur(std::move(n.couleur)),
											   pere(std::move(n.pere)),
											   filsGauche(std::move(n.filsGauche)), filsDroit(std::move(n.filsDroit)) {
			n.filsGauche = n.filsDroit = n.pere = nullptr;
		}

		/**
		 * Destructeur (appellera automatiquement les destructeurs des unique_ptr et shared_ptr : garbage collector).
		 */
		~node_t() noexcept = default;

		/**
		 * Permet d'avoir le grand-parent du nœud.
		 * @return le grand-parent du nœud courant.
		 */
		node_t* grandParent() {
			return this->pere->pere;
		}
	};

	/**
	 * Permet de rechercher le minimum (si Compare = std::less) ou le maximum (Compare = std::greater)
	 * @param [in]x La racine du sous-arbre dont on doit trouver le "minimum"
	 * @return Le "minimum" de l'arbre
	 */
	node* tree_minimum(node* x) {
		while (x->filsGauche != this->tnil)
			x = x->filsGauche;
		return x;
	}

	node* tree_maximum(node* x) {
		while (x->filsDroit != this->tnil)
			x = x->filsDroit;
		return x;
	}

	/**
	 * Insertion du nœud dans l'arbre, algorithme prit dans le livre <a href="https://fr.wikipedia.org/wiki/Introduction_%C3%A0_l%27algorithmique">"Introduction to algorithm, third edition".</a>
	 * @param [in]z nœud à insérer.
	 */
	void insert_rd_tree(node* z) {
		node* x(this->racine), * y(this->tnil);
		while (x != this->tnil) {
			y = x;
			if (keyComp(*z->key, *x->key))
				x = x->filsGauche;
			else
				x = x->filsDroit;
		}
		z->pere = y;
		if (y == this->tnil)
			this->racine = z;
		else if (keyComp(*z->key, *y->key))
			y->filsGauche = z;
		else
			y->filsDroit = z;
		z->filsGauche = this->tnil;
		z->filsDroit = this->tnil;
		z->couleur = rouge;
		insert_repair_tree(z);
	}

	/**
	 * Algorithme provenant du livre "Introduction to algorithm, third edition".
	 * @param [in]z Le nœud courant à partir duquel on répare l'arbre.
	 * @bug segfault aléatoire dans la deuxième partie de l'algorithme.
	 */
	void insert_repair_tree(node* z) {
		node* y;
		while (z->pere->couleur == rouge) {
			if (z->pere == z->grandParent()->filsGauche) {
				y = z->grandParent()->filsDroit;
				if (y->couleur == rouge) {
					z->pere->couleur = noir;
					y->couleur = noir;
					z->grandParent()->couleur = rouge;
					z = z->grandParent();
				} else {
					if (z == z->pere->filsDroit) {
						z = z->pere;
						rotate_left(z);
					}
					z->pere->couleur = noir;
					z->grandParent()->couleur = rouge;
					rotate_right(z->grandParent());
				}
				/*
				 * Cette partie n'était pas donnée dans le livre. En partie de la devinette.
				 * Et c'est elle qui cause la segfault (rotation droite ou gauche)
				 */
			} else {
				y = z->grandParent()->filsGauche;
				if (y->couleur == rouge) {
					z->pere->couleur = noir;
					y->couleur = noir;
					z->grandParent()->couleur = rouge;
					z = z->grandParent();
				} else {
					if (z == z->pere->filsGauche) {
						z = z->pere;
						rotate_right(z);
					}
					z->pere->couleur = noir;
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
	void rotate_left(node* x) {
		if (x->filsDroit != this->tnil) {
			node* y = x->filsDroit;
			x->filsDroit = y->filsGauche;
			if (y->filsGauche != this->tnil)
				y->filsGauche->pere = x;
			y->pere = x->pere;
			if (x->pere == this->tnil)
				this->racine = y;
			else if (x == x->pere->filsGauche)
				x->pere->filsGauche = y;
			else
				x->pere->filsDroit = y;
			y->filsGauche = x;
			x->pere = y;
		}
	}

	/**
	 * Rotation droite. Même chose que la rotation gauche sauf que tous les mots "gauche" sont remplacés par droit, et vice-versa.
	 * @param [in]x Nœud à partir duquel la rotation se fait.
	 */
	void rotate_right(node* x) {
		if (x->filsGauche != this->tnil) {
			node* y = x->filsGauche;
			x->filsGauche = y->filsDroit;
			if (y->filsDroit != this->tnil)
				y->filsDroit->pere = x;
			y->pere = x->pere;
			if (x->pere == this->tnil)
				this->racine = y;
			else if (x == x->pere->filsDroit)
				x->pere->filsDroit = y;
			else
				x->pere->filsGauche = y;
			y->filsDroit = x;
			x->pere = y;
		}
	}

	/**
	 * Transplantation d'arbre
	 * @param [in]u Donneur
	 * @param [in]v Receveur
	 */
	void rb_transplant(node* u, node* v) {
		if (u->pere == this->tnil)
			this->racine = v;
		else if (u == u->pere->filsGauche)
			u->pere->filsGauche = v;
		else
			u->pere->filsDroit = v;
		v->pere = u->pere;
	}

	/**
	 * Réparation de l'arbre (si règles enfreintes) lors de la suppression d'un noeud.
	 * @param [in]x Noeud à partir duquel la réparation se fait.
	 */
	void rb_delete_fixup(node* x) {
		node* w;
		while (x != this->racine && x->couleur == noir) {
			if (x == x->pere->filsGauche) {
				w = x->pere->filsDroit;
				if (w->couleur == rouge) {
					w->couleur = noir;
					x->pere->couleur = rouge;
					rotate_left(x->pere);
					w = x->pere->filsDroit;
				}
				if (w->filsGauche->couleur == noir && w->filsDroit->couleur == noir) {
					w->couleur = rouge;
					x = x->pere;
				} else {
					if (w->filsDroit->couleur == noir) {
						w->filsGauche->couleur = noir;
						w->couleur = rouge;
						rotate_right(w);
						w = x->pere->filsDroit;
					}
					w->couleur = x->pere->couleur;
					x->pere->couleur = noir;
					w->filsDroit->couleur = noir;
					rotate_left(x->pere);
					x = this->racine;
				}
			} else {
				w = x->pere->filsGauche;
				if (w->couleur == rouge) {
					w->couleur = noir;
					x->pere->couleur = rouge;
					rotate_right(x->pere);
					w = x->pere->filsGauche;
				}
				if (w->filsGauche->couleur == noir && w->filsDroit->couleur == noir) {
					w->couleur = rouge;
					x = x->pere;
				} else {
					if (w->filsGauche->couleur == noir) {
						w->filsDroit->couleur = noir;
						w->couleur = rouge;
						rotate_left(w);
						w = x->pere->filsGauche;
					}
					w->couleur = x->pere->couleur;
					x->pere->couleur = noir;
					w->filsGauche->couleur = noir;
					rotate_right(x->pere);
					x = this->racine;
				}
			}
		}
		x->couleur = noir;
	}

	key_compare keyComp;
	value_compare valueComp;
	node* tnil;
	node* racine;
	size_type size;
/**
 * @publicsection
 */
public:
	/**
	 * Constructeur par défaut.
	 */
	Set() : tnil(new node()), racine(tnil), size(0) {}

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
	Set(InputIterator first, InputIterator last, key_compare& comp = key_compare()) : keyComp(comp), valueComp(comp) {

	}

	/**
	 * Constructeur par copie.
	 * @param [in]s Set compatible à copier.
	 */
	Set(const Set& s) : size(s.size), keyComp(s.keyComp), valueComp(s.valueComp) {

	}

	/**
	 * Constructeur par déplacement
	 * @param [in]s Set à déplacer (voler) les données
	 */
	Set(Set&& s) noexcept : size(std::move(s.size)), racine(std::move(s.racine)), tnil(std::move(s.tnil)),
							keyComp(std::move(s.keyComp)),
							valueComp(std::move(s.valueComp)) {
		s.keyComp = s.valueComp = s.racine = s.tnil = nullptr;
	}

	/**
	 * Constructeur par liste d'initialisation
	 * @param [in] il liste d'initialisation
	 * @param [in]comp comparateur
	 */
	Set(std::initializer_list<value_type> il, const key_compare& comp = key_compare()) : keyComp(comp),
																						 valueComp(comp) {}

	/**
	 * Détruis l'objet.
	 */
	~Set() noexcept {
		
	}

	/**
	 * Itérateur de début du Set.
	 * @return retourne un itérateur sur le début du Set.
	 */
	iterator begin() noexcept {
		return iterator(*this, tree_minimum(this->racine));
	}

	/**
	 * Itérateur de fin du Set.
	 * @return [out] Itérateur sur le dernier nœud : le maximum.
	 */
	iterator end() noexcept {
		return iterator(*this, this->tnil);
	}

	/**
	 * Vérifie si le conteneur est vide.
	 * @return [out] True si il est vide.
	 */
	bool empty() const noexcept {
		return (this->size == 0);
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
	iterator find(const reference key) {
		node* x = this->racine;
		while (x != this->tnil && ((keyComp(key, *x->key) || keyComp(*x->key, key)))) {
			if (keyComp(key, *x->key))
				x = x->filsGauche;
			else
				x = x->filsDroit;
		}
		return iterator(*this, x);
	}

	/**
	 * Implémentation sûre. Si le comparateur est > au lieu de <, l'arbre sera juste inversée.
	 * @param [in]value
	 * @return [out]Une paire avec l'itérateur sur le nœud et un booléen si l'opération a réussi ou non.
	 */
	std::pair<iterator, bool> insert(const reference value) {
		if (find(value).currentNode != this->tnil) {
			return std::pair<iterator, bool>(iterator(*this), false);
		}
		node* n;
		try {
			n = new node(value, this->tnil);//Peut throw bad_alloc
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return std::pair<iterator, bool>(iterator(*this), false);
		}
		insert_rd_tree(n);
		++this->size;
		return std::pair<iterator, bool>(iterator(*this, n), true);
	}

	/**
	 * Insère une rvalue.
	 * @param [in]value Rvalue à insérer dans l'arbre.
	 * @return [out] Une pair avec first = iterator, second = true/false.
	 */
	std::pair<iterator, bool> insert(value_type&& value) {
		if (find(value).currentNode != this->tnil) {
			return std::pair<iterator, bool>(iterator(*this), false);
		}
		node* n;
		try {
			n = new node(value, this->tnil);//Peut throw bad_alloc
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return std::pair<iterator, bool>(iterator(*this), false);
		}
		insert_rd_tree(n);
		++this->size;
		return std::pair<iterator, bool>(iterator(*this, n), true);
	}

	/**
	 * Insertion à partir de deux itérateurs.
	 * @tparam [in]InputIt Type de l'itérateur utilisé pour insérer un élément.
	 * @param [in]first Itérateur sur le premier élement.
	 * @param [in]last Itérateur sur le dernier élément.
	 */
	template<class InputIt>
	void insert(InputIt first, InputIt last) {
		for (auto& it = first; it != last; ++it) {
			this->insert(*it);
		}
	}

	/**
	 * Insertion à partir d'une liste de type value_type.
	 * @param [in]il Liste à insérer dans l'arbre.
	 */
	void insert(std::initializer_list<value_type> il) {
		for (auto&& item : il) {
			this->insert(item);
		}
	}

	/**
	 * Permets d'effacer un noeud de l'arbre.
	 * @param [in]key Valeur contenue dans le noeud à effacer.
	 * @return [out] Nombre d'élément d'enlever (ici 0 ou 1, chaque élément étant unique).
	 * @todo Finir cette méthode ainsi que les fonctions qui lui sont nécessaires.
	 */
	size_type erase(const reference key) {
		node* z(find(key).currentNode), * y(z), * x;
		if (z != this->tnil) {
			return 0;
		}
		color y_original = y->couleur;
		if (z->filsGauche == this->tnil) {
			x = z->filsDroit;
			rb_transplant(z, z->filsDroit);
		} else if (z->filsDroit == this->tnil) {
			x = z->filsGauche;
			rb_transplant(z, z->filsGauche);
		} else {
			y = tree_minimum(z->filsDroit);
			y_original = y->couleur;
			x = y->filsDroit;
			if (y->pere == z)
				x->pere = y;
			else {
				rb_transplant(y, y->filsDroit);
				y->filsDroit = z->filsDroit;
				y->filsDroit->pere = y;
			}
			rb_transplant(z, y);
			y->filsGauche = z->filsGauche;
			y->filsGauche->pere = y;
			y->couleur = z->couleur;
		}
		if (y_original == noir)
			rb_delete_fixup(x);
		--this->size;
		return 1; //Si élément présent
	}

	/**
	 * Renvoie la fonction de comparaison des clés.
	 * @return [out] Fonction de comparaison de l'objet courant.
	 */
	inline key_compare key_comp() const { return keyComp; }

	/**
	 * Renvoie la fonction de comparaison des valeurs.
	 * @return [out] Fonction de comparaison de l'objet courant.
	 */
	inline value_compare value_comp() const { return valueComp; }

	/**
	 * Assignation par copie. Utilisation de l'itérateur.
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
			size = std::move(x.size);
			racine = std::move(x.racine);
			tnil = std::move(x.tnil);
			keyComp = std::move(x.keyComp);
			valueComp = std::move(x.valueComp);
			x.tnil = x.racine = x.valueComp = x.keyComp = nullptr;
		}
		return *this;
	}

	/**
	 * Assignation par liste.
	 * @param [in]list Liste à assigner
	 * @return [out] L'objet créé par la liste.
	 */
	Set& operator=(std::initializer_list<value_type> list) { //Not tested yet.
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
 * @todo L'opérateur d'avancée (++)
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
	explicit SetIter(Set<Key, Compare>& myset) : myset(myset), currentNode(myset.racine),
												 size(myset.getSize()) {
		if (currentNode != myset.tnil) {
			this->lastNode = currentNode->pere;
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
		if (currentNode != myset.tnil) {
			this->lastNode = currentNode->pere;
		}
	}

	/**
	 * Constructeur par copie.
	 * @param [in]setIter
	 */
	SetIter(const SetIter<Key, Compare>& setIter) : myset(setIter.myset), size(setIter.size),
													currentNode(setIter.currentNode), lastNode(setIter.lastNode) {}

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
		return (this->currentNode == rhs.currentNode) && (this->lastNode == rhs.lastNode);
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
		/*
		 * Parcours toujours vers la droite.
		 */
		if (this->currentNode->filsDroit != this->myset.tnil) {
			this->currentNode = this->currentNode->filsDroit;
			while (this->currentNode->filsGauche != this->myset.tnil)
				this->currentNode = this->currentNode->filsGauche;
		} else {
			typename Set<Key, Compare>::node* pere = this->currentNode->pere;
			while (this->currentNode == pere->filsDroit) {
				this->currentNode = pere;
				pere = pere->pere;
			}
			if (this->currentNode->filsDroit != pere)
				this->currentNode = pere;
		}
	}

	typename Set<Key, Compare>::iterator operator++(int) {

	}
};


#endif //PROJET_SET_HPP
