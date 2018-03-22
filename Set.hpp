#ifndef PROJET_SET_HPP
#define PROJET_SET_HPP

/**
 * Aller, c'est partiiii :D
 * Construction d'abord sans template, test ? go template : debug.
 * Implémentation par un arbre rouge-noir (snif :'()
 */
class set {
private:
	typedef int value_type;
	typedef int* pointer;
	typedef value_type& reference;
	typedef enum {
		noir, rouge
	} couleur;

	/**
	 * Structure de nœud pour l'arbre (en interne pour éviter de s'en servir en dehors de la classe)
	 */
	typedef struct {
		value_type value;
		couleur color;
		pointer parent;
		pointer gauche;
		pointer droit;
	} node;

public:
	typedef node* iterator; //Eest-ce bon ? :/
};


#endif //PROJET_SET_HPP
