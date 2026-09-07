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
    m_etat(EtatJeu::Menu)
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

        // Touche Echap : bascule entre EnJeu et Pause
        if (const auto* e = event->getIf<sf::Event::KeyPressed>()) {
            if (e->code == sf::Keyboard::Key::Escape) {
                if (m_etat == EtatJeu::EnJeu)
                    m_etat = EtatJeu::Pause;
                else if (m_etat == EtatJeu::Pause)
                    m_etat = EtatJeu::EnJeu;
            }
        }

        if (m_etat == EtatJeu::Menu) {
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
        else if (m_etat == EtatJeu::Pause) {
            if (const auto* e = event->getIf<sf::Event::KeyPressed>())
                m_menu.gererTouchePause(e->code);

            if (const auto* e = event->getIf<sf::Event::MouseMoved>())
                m_menu.gererSourisPause({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
                if (e->button == sf::Mouse::Button::Left)
                    m_menu.gererClicPause({e->position.x, e->position.y});

            if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
                m_menu.gererMolettePause(e->delta);
        }
    }
}

void Jeu::miseAJour() {
    if (m_etat == EtatJeu::Menu) {
        if (m_menu.demandeLancementJeu()) {
            m_etat = EtatJeu::EnJeu;
            m_menu.resetLancementJeu();
        }
    }
    else if (m_etat == EtatJeu::Pause) {
        // Actions demandées depuis le menu pause
        if (m_menu.demandeReprise()) {
            m_etat = EtatJeu::EnJeu;
            m_menu.resetReprise();
        }
        else if (m_menu.demandeRecommencer()) {
            m_grille = Grille();           // Réinitialise la partie
            m_etat   = EtatJeu::EnJeu;
            m_menu.resetRecommencer();
        }
        else if (m_menu.demandeQuitter()) {
            m_etat = EtatJeu::Menu;
            m_grille = Grille();           // Réinitialise la partie
            m_menu.resetQuitter();
        }
    }
}

void Jeu::affichage() {
    m_fenetre.clear(COULEUR_FENETRE);

    if (m_etat == EtatJeu::Menu) {
        m_menu.afficher(m_fenetre);
    }
    else if (m_etat == EtatJeu::EnJeu) {
        m_grille.dessiner(m_fenetre);
    }
    else if (m_etat == EtatJeu::Pause) {
        m_grille.dessiner(m_fenetre);      // Grille en arrière-plan
        m_menu.afficherPause(m_fenetre);   // Overlay pause par-dessus
    }

    m_fenetre.display();
}
