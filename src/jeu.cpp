#include "jeu.hpp"
#include <iostream>

Jeu::Jeu()
    : m_fenetre(
        sf::VideoMode({
            static_cast<unsigned int>(LARGEUR_FENETRE),
            static_cast<unsigned int>(LONGUEUR_FENETRE)
        }),
        "Tetris en C++",
        sf::Style::Default
    ),
    m_police(),
    m_menu(),
    m_grille(),
    m_enJeu(false)
{
    // 1. Charger la police en premier
    if (!m_police.openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        if (!m_police.openFromFile("assets/fonts/PixelifySans-VariableFont_wght.ttf")) {
            std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
        }
    }

    // 2. Initialiser le Menu APRES le chargement de la police pour que getLocalBounds() fonctionne correctement
    m_menu.init(static_cast<float>(LARGEUR_FENETRE), static_cast<float>(LONGUEUR_FENETRE), m_police);
}

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

        if (!m_enJeu) {
            if (const auto* e = event->getIf<sf::Event::KeyPressed>())
                m_menu.gererTouche(e->code);

            if (const auto* e = event->getIf<sf::Event::MouseMoved>())
                m_menu.gererSouris({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
                if (e->button == sf::Mouse::Button::Left)
                    m_menu.gererClic({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
                m_menu.gererMolette(e->delta);
        }
    }
}

void Jeu::miseAJour() {
    if (!m_enJeu) {
        if (m_menu.demandeLancementJeu()) {
            m_enJeu = true;
            m_menu.resetLancementJeu();
        }
    }
}

void Jeu::affichage() {
    m_fenetre.clear(COULEUR_FENETRE);

    if (!m_enJeu) {
        m_menu.afficher(m_fenetre);
    } else {
        m_grille.dessiner(m_fenetre);
    }

    m_fenetre.display();
}
