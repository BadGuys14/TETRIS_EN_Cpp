#ifndef JEU_HPP
#define JEU_HPP

#include <optional>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include "grille.hpp"
#include "Menu.h"
#include "Palette.h"
#include "Piece.hpp"

// ---------------------------------------------------------------
//  Classe Jeu : boucle principale, fenetre, evenements et logique
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

        sf::RenderWindow        m_fenetre;
        sf::Font                m_police;
        Menu                    m_menu;
        Grille                  m_grille;
        EtatJeu                 m_etat{ EtatJeu::Menu };

        // --- Gestion des pieces et tirage 7-bag ---
        std::vector<FormePiece> m_sac;           // Sac de 7 formes pour garantir un tirage equitable
        std::mt19937            m_generator;     // Generateur aleatoire (Mersenne Twister)
        std::optional<Piece>    m_pieceCourante; // Piece actuellement controlees et en chute
        Piece                   m_pieceSuivante; // Prochaine piece qui entrera en jeu

        // --- Horloge de chute automatique ---
        sf::Clock               m_horlogeChute;  // Horloge mesurant l'intervalle de descente
        float                   m_delaiChute{ 0.5f }; // Temps en secondes entre deux chutes (0.5s)

        // --- Methodes privees de la boucle de jeu ---
        void gesEvenements();
        void miseAJour();
        void affichage();

        // --- Methodes privees de gestion des pieces ---

        // Remplit m_sac avec les 7 formes Tetris et les melange
        void remplirSac();

        // Extrait une forme du sac (le remplit s'il est vide)
        FormePiece piocherForme();

        // Reinitialise le jeu et genere la piece courante et la piece suivante
        void demarrerNouvellePartie();

        // Tente de deplacer la piece courante de (dl) lignes et (dc) colonnes si valide
        bool essayerDeplacerPiece(int dl, int dc);

        // Verrouille la piece courante dans la grille, nettoie les lignes et passe a la suivante
        void verrouillerPiece();

    public:
        Jeu();
        void executer();
};

#endif // JEU_HPP
