#include "grille.hpp"

Grille::Grille() {
    reinitialiser();
}

void Grille::reinitialiser() {
    for (int l = 0; l < LIGNES; ++l) {
        for (int c = 0; c < COLONNES; ++c) {
            m_grille[l][c] = 0; // 0 represente une case vide
        }
    }
}

void Grille::dessiner(sf::RenderWindow& fenetre) {
    sf::RectangleShape caseGrille(sf::Vector2f({static_cast<float>(TAILLE_CASE - 1), static_cast<float>(TAILLE_CASE - 1)}));

    for (int l = 0; l < LIGNES; ++l) {
        for (int c = 0; c < COLONNES; ++c) {
            caseGrille.setPosition({static_cast<float>(c * TAILLE_CASE), static_cast<float>(l * TAILLE_CASE)});

            if (m_grille[l][c] == 0) {
                caseGrille.setFillColor(COULEUR_GRILLE); // Dessine le fond des cases vides
            } else {
                caseGrille.setFillColor(sf::Color::Blue);
            }

            fenetre.draw(caseGrille);
        }
    }
}