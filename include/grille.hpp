#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <array>
#include <SFML/Graphics.hpp>
#include "Piece.hpp"

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
        static const int LIGNES      = 20;  // nombre de lignes du plateau
        static const int COLONNES    = 10;  // nombre de colonnes du plateau
        static const int TAILLE_CASE = 28;  // cote d'une case en pixels (20x28=560 < 600)

        // Couleur des cases vides de la grille (bleu clair discret)
        static inline sf::Color COULEUR_GRILLE = sf::Color(238, 241, 248);

        int       m_grille  [LIGNES][COLONNES]; // 0 = vide, 1 = occupe
        sf::Color m_couleurs[LIGNES][COLONNES]; // couleur de chaque case fixee

    public:
        Grille();

        void reinitialiser();
        void dessiner(sf::RenderWindow& fenetre);

        // -------------------------------------------------------
        //  Requetes de collision
        // -------------------------------------------------------

        // Vrai si la case (ligne, colonne) est dans les limites ET vide.
        // Utilise en premier filtre avant positionValide.
        bool caseLibre(int ligne, int colonne) const;

        // Vrai si les 4 blocs de la piece (positions absolues) sont tous libres.
        // A appeler avant tout deplacement ou apparition pour valider la position.
        bool positionValide(const std::array<sf::Vector2i, 4>& positions) const;

        // -------------------------------------------------------
        //  Modification de la grille
        // -------------------------------------------------------

        // Ecrit les 4 blocs de la piece dans m_grille (1) et m_couleurs,
        // appelee quand la piece ne peut plus descendre (verrouillage).
        void fixerPiece(const Piece& piece);

        // Parcourt la grille de bas en haut, supprime chaque ligne entierement
        // occupee et fait descendre toutes celles qui sont au-dessus.
        // Renvoie le nombre de lignes supprimees (0-4).
        int supprimerLignesCompletes();

        // -------------------------------------------------------
        //  Getters pour Jeu (dimensions partagees avec Piece::dessiner)
        // -------------------------------------------------------
        int getLignes()     const { return LIGNES; }
        int getColonnes()   const { return COLONNES; }
        int getTailleCase() const { return TAILLE_CASE; }
};

#endif // GRILLE_HPP
