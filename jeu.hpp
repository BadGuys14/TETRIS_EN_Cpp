#ifndef JEU_HPP
#define JEU_HPP
#include <optional>
#include <SFML/Graphics.hpp>
#include "grille.hpp"

class Jeu {
    private:
        static const int LONGUEUR_FENETRE = 800;
        static const int LARGEUR_FENETRE = 600;
        static inline sf::Color COULEUR_FENETRE = sf::Color::White;
        sf::RenderWindow m_fenetre;
        Grille m_grille;

        void gesEvenements();
        void miseAJour();
        void affichage();

    public:
        Jeu();
        void executer();
};

#endif // JEU_HPP