#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <string>
#include <SFML/Graphics.hpp>

// ---------------------------------------------------------------
//  Classe Grille : plateau Tetris 10 colonnes x 20 lignes
//
//  Adaptation Option A (fenetre paysage 800 x 600) :
//    TAILLE_CASE = 28 px (au lieu de 40)
//    => plateau = 10 x 28 = 280 px de large
//              = 20 x 28 = 560 px de haut
//    La fenetre fait 600 px de haut => marge de 20 px haut/bas
//    Il reste 800 - 280 = 520 px en largeur pour
//    le score, la piece suivante, le niveau, etc.
// ---------------------------------------------------------------
class Grille {

    private:
        static const int LIGNES    = 20;  // nombre de lignes du plateau
        static const int COLONNES  = 10;  // nombre de colonnes du plateau

        // TAILLE_CASE reduit de 40 a 28 pour tenir dans 600 px de haut
        // (20 lignes x 28 px = 560 px < 600 px)
        static const int TAILLE_CASE = 28;

        // Couleur des cases vides de la grille (bleu clair discret)
        static inline sf::Color COULEUR_GRILLE = sf::Color(238, 241, 248);

        int m_grille[LIGNES][COLONNES]; // 0 = case vide

    public:
        Grille();

        void reinitialiser();
        void dessiner(sf::RenderWindow& fenetre);
};

#endif // GRILLE_HPP
