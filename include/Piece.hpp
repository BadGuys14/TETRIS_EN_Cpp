#pragma once

#include <SFML/Graphics.hpp>
#include <array> //explication 
#include "Palette.h"

// Les 7 formes Tetris classiques
enum class FormePiece {
    I, O, T, L, J, S, Z
};

//coordonnees relative (colonne, ligne) des 4 blocs de la piece
using FormeCoords = std::array<sf::Vector2i, 4>; //explication

class Piece {
    FormePiece m_forme; //forme de la piece
    sf::Color m_couleur; // couleur elementaire associé à une forme
    int m_ligne; // ligne de l'origine de la piece dans la grille
    int m_colonne; // colonne de l'origine de la piece dans la grille

    //Renvoie les 4 coordonnees relatives associées a la forme
    FormeCoords calculerCoords() const;

    static sf::Color CouleurselonForme(FormePiece forme);

    public:
    //constuction d'une piece de la forme donnee, positionnee en haut
    explicit Piece (FormePiece forme, int colonneDepart = 3, int ligneDepart = 0);

    //Deplace la piece de (dl et dc) ligne/colonnes
    void deplacer(int dl, int dc);

    //renvoie les positions absolues (colonne, ligne) des 4 blocs
    std::array<sf::Vector2i, 4> positionAbs() const;

    //Dessine la piece dans la fenetre(tailleCase c'est la dtaille d'une case de la grille)
    void dessiner(sf::RenderWindow& fenetre, int tailleCase) const;

    FormePiece getForme() const {return m_forme;}//Explication
    sf::Color getCouleur() const {return m_couleur;}//Explication
    int getLigne () const {return m_ligne;}//Explication
    int getColonne () const {return m_colonne;} //Explication
};