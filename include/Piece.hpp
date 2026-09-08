#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "Palette.h"

// Les 7 formes Tetris classiques
enum class FormePiece {
    I, O, T, L, J, S, Z
};

// Coordonnées relatives (colonne, ligne) des 4 blocs de la pièce
using FormeCoords = std::array<sf::Vector2i, 4>;

class Piece {
private:
    FormePiece m_forme;       // Forme de la pièce
    sf::Color  m_couleur;     // Couleur élémentaire associée à la forme
    int        m_ligne;       // Ligne de l'origine de la pièce dans la grille
    int        m_colonne;     // Colonne de l'origine de la pièce dans la grille
    int        m_orientation; // Orientation (0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°)

    // Renvoie les 4 coordonnées relatives associées à la forme et à son orientation
    FormeCoords calculerCoords() const;

    static sf::Color CouleurselonForme(FormePiece forme);

public:
    // Construction d'une pièce de la forme donnée, positionnée en haut
    explicit Piece(FormePiece forme, int colonneDepart = 3, int ligneDepart = 0);

    // Déplace la pièce de (dl) lignes et (dc) colonnes
    void deplacer(int dl, int dc);

    // Effectue une rotation dans le sens horaire
    void tournerHoraire();

    // Effectue une rotation dans le sens anti-horaire
    void tournerAntiHoraire();

    // Renvoie les positions absolues (colonne, ligne) des 4 blocs
    std::array<sf::Vector2i, 4> positionAbs() const;

    // Dessine la pièce réelle dans la fenêtre de jeu
    void dessiner(sf::RenderWindow& fenetre, int tailleCase) const;

    // Dessine la pièce fantôme (ombre semi-transparente au sol)
    void dessinerFantome(sf::RenderWindow& fenetre, int tailleCase) const;

    // Dessine la pièce centrée dans un encadré d'aperçu (ex. zone NEXT PIECE)
    void dessinerAPosition(sf::RenderWindow& fenetre, float xOrigin, float yOrigin, int tailleCase) const;

    FormePiece getForme()       const { return m_forme; }
    sf::Color  getCouleur()     const { return m_couleur; }
    int        getLigne()       const { return m_ligne; }
    int        getColonne()     const { return m_colonne; }
    int        getOrientation() const { return m_orientation; }
};