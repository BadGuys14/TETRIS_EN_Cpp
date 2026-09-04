#include "jeu.hpp"

Jeu::Jeu() : m_fenetre(sf::VideoMode({LARGEUR_FENETRE, LONGUEUR_FENETRE }), "Tetris en C++") {}

void Jeu::executer() {
    while (m_fenetre.isOpen()) {
        gesEvenements();
        miseAJour();
        affichage();
    }
}

void Jeu::gesEvenements() {
    while (const std::optional event = m_fenetre.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_fenetre.close();
        }
    }
}

void Jeu::miseAJour() {
    // Logique de mise à jour du jeu (ex: déplacement des pièces, vérification des lignes complètes, etc.)
}

void Jeu::affichage() {
    m_fenetre.clear(sf::Color(COULEUR_FENETRE)); // Couleur de fond de la fenêtre
    // Logique d'affichage du jeu (ex: dessiner les pièces, le plateau, etc.)
    m_grille.dessiner(m_fenetre);
    m_fenetre.display();

}