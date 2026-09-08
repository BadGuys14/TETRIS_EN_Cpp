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
enum class EtatJeu {
    Menu,   // Menus principaux (titre + accueil)
    EnJeu,  // Partie en cours
    Pause   // Menu pause
};

class Jeu {
    private:
        static const int LARGEUR_FENETRE  = 800; // Largeur fenêtre (axe X)
        static const int LONGUEUR_FENETRE = 600; // Hauteur fenêtre (axe Y)

        static inline sf::Color COULEUR_FENETRE = sf::Color(30, 32, 40);

        sf::RenderWindow        m_fenetre;
        sf::Font                m_police;
        Menu                    m_menu;
        Grille                  m_grille;
        EtatJeu                 m_etat{ EtatJeu::Menu };

        // --- Gestion des pieces et tirage 7-bag ---
        std::vector<FormePiece> m_sac;           // Sac de 7 formes pour garantir un tirage équitable
        std::mt19937            m_generator;     // Générateur aléatoire (Mersenne Twister)
        std::optional<Piece>    m_pieceCourante; // Pièce actuellement contrôlée
        Piece                   m_pieceSuivante; // Prochaine pièce qui entrera en jeu

        // --- Horloge de chute automatique ---
        sf::Clock               m_horlogeChute;  // Horloge mesurant l'intervalle de descente
        float                   m_delaiChute{ 0.5f }; // Temps en secondes entre deux chutes (0.5s)

        // --- Scores, Combos et Statistiques ---
        int                     m_score{ 0 };
        int                     m_meilleurScore{ 0 };
        int                     m_combo{ 0 };
        int                     m_lignesTotales{ 0 };

        // --- Méthodes privées de la boucle de jeu ---
        void gesEvenements();
        void miseAJour();
        void affichage();

        // --- Méthodes privées de gestion des pièces ---
        void remplirSac();
        FormePiece piocherForme();
        void demarrerNouvellePartie();
        bool essayerDeplacerPiece(int dl, int dc);
        void tournerPieceCourante();
        void hardDropPiece();
        void verrouillerPiece();
        Piece calculerGhostPiece() const;

        // --- Sauvegarde & Rendu de l'Interface ---
        void chargerMeilleurScore();
        void sauvegarderMeilleurScore();
        void dessinerPanneauLateral();

    public:
        Jeu();
        void executer();
};

#endif // JEU_HPP
