#ifndef JEU_HPP
#define JEU_HPP

#include <optional>
#include <SFML/Graphics.hpp>
#include "grille.hpp"
#include "Menu.h"
#include "Palette.h"

// ---------------------------------------------------------------
//  Classe Jeu : boucle principale, fenetre, evenements
// ---------------------------------------------------------------
// Etats possibles de la boucle de jeu
enum class EtatJeu {
    Menu,   // Menus principaux (titre + accueil)
    EnJeu,  // Partie en cours
    Pause   // Menu pause
};

class Jeu {
    private:
        static const int LARGEUR_FENETRE  = 800; // largeur  (axe X)
        static const int LONGUEUR_FENETRE = 600; // longueur (axe Y)

        static inline sf::Color COULEUR_FENETRE = sf::Color(30, 32, 40);

        sf::RenderWindow m_fenetre;
        sf::Font         m_police;
        Menu             m_menu;
        Grille           m_grille;
        EtatJeu          m_etat  { EtatJeu::Menu };

        void gesEvenements();
        void miseAJour();
        void affichage();

    public:
        Jeu();
        void executer();
};

#endif // JEU_HPP
