#ifndef GRILLE_HPP
#define GRILLE_HPP
#include <string>

#include <SFML/Graphics.hpp>

class Grille {

    private:
        static const int LIGNES = 20;
        static const int COLONNES = 10;
        static const int TAILLE_CASE = 40; // Taille d'une case en pixels
        static inline sf::Color COULEUR_GRILLE = sf::Color(238, 241, 248); // Couleur de la grille (bleu clair)
        int m_grille[LIGNES][COLONNES]; // 20 lignes et 10 colonnes pour la grille de jeu

    public:
        Grille();

        void reinitialiser();
        void dessiner(sf::RenderWindow& fenetre);

};

#endif // GRILLE_HPP