#ifndef PROJET_SETITER_HPP
#define PROJET_SETITER_HPP

#include "Set.hpp"

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


#endif //PROJET_SETITER_HPP
